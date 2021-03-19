
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/geom/Math.h"

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/Constants.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/LocalizationUtils.h"

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/CollisionStage.h"
#include <algorithm>
#include <unordered_map>
#include <utility>
#include "../geom/Location.h"
#include "../geom/Vector3D.h"
#include "SimpleWaypoint.h"

namespace carla {
namespace traffic_manager {

using Point2D = bg::model::point<double, 2, bg::cs::cartesian>;
using TLS = carla::rpc::TrafficLightState;

using namespace constants::Collision;
using constants::WaypointSelection::JUNCTION_LOOK_AHEAD;

CollisionStage::CollisionStage(
  const std::vector<ActorId> &vehicle_id_list,
  const SimulationState &simulation_state,
  const BufferMap &buffer_map,
  const TrackTraffic &track_traffic,
  const Parameters &parameters,
  CollisionFrame &output_array,
  cc::DebugHelper &debug_helper,
  RandomGeneratorMap &random_devices)
  : vehicle_id_list(vehicle_id_list),
    simulation_state(simulation_state),
    buffer_map(buffer_map),
    track_traffic(track_traffic),
    parameters(parameters),
    output_array(output_array),
    debug_helper(debug_helper),
    random_devices(random_devices) 
{

}

void CollisionStage::Update(const unsigned long index) 
{
  ActorId obstacle_id = 0u;//声明存在路障
  bool collision_hazard = false;
  float available_distance_margin = std::numeric_limits<float>::infinity();

  const ActorId ego_actor_id = vehicle_id_list.at(index);
  if (simulation_state.ContainsActor(ego_actor_id)) 
  {
    const cg::Location ego_location = simulation_state.GetLocation(ego_actor_id);
    const Buffer &ego_buffer = buffer_map.at(ego_actor_id);
    const unsigned long look_ahead_index = GetTargetWaypoint(ego_buffer, JUNCTION_LOOK_AHEAD).second;

    ActorIdSet overlapping_actors = track_traffic.GetOverlappingVehicles(ego_actor_id);
    std::vector<ActorId> collision_candidate_ids;

    // Run through vehicles with overlapping paths and filter them;
    float collision_radius_square = SQUARE(MAX_COLLISION_RADIUS);
    for (ActorId overlapping_actor_id : overlapping_actors) {
      // If actor is within maximum collision avoidance and vertical overlap range.
      const cg::Location &overlapping_actor_location = simulation_state.GetLocation(overlapping_actor_id);
      if (overlapping_actor_id != ego_actor_id
          && cg::Math::DistanceSquared(overlapping_actor_location, ego_location) < collision_radius_square
          && std::abs(ego_location.z - overlapping_actor_location.z) < VERTICAL_OVERLAP_THRESHOLD) {
        collision_candidate_ids.push_back(overlapping_actor_id);
      }
    }

    // Sorting collision candidates in accending order of distance to current vehicle.
    std::sort(collision_candidate_ids.begin(), collision_candidate_ids.end(),
              [this, &ego_location](const ActorId &a_id_1, const ActorId &a_id_2) {
                const cg::Location &e_loc = ego_location;
                const cg::Location &loc_1 = simulation_state.GetLocation(a_id_1);
                const cg::Location &loc_2 = simulation_state.GetLocation(a_id_2);
                return (cg::Math::DistanceSquared(e_loc, loc_1) < cg::Math::DistanceSquared(e_loc, loc_2));
              });

    // Check every actor in the vicinity if it poses a collision hazard.
    for (auto iter = collision_candidate_ids.begin();
         iter != collision_candidate_ids.end() && !collision_hazard;
         ++iter) {
      const ActorId other_actor_id = *iter;
      const ActorType other_actor_type = simulation_state.GetType(other_actor_id);

      if (parameters.GetCollisionDetection(ego_actor_id, other_actor_id)
          && buffer_map.find(ego_actor_id) != buffer_map.end()
          && simulation_state.ContainsActor(other_actor_id)) {
        std::pair<bool, float> negotiation_result = NegotiateCollision(ego_actor_id,
                                                                       other_actor_id,
                                                                       look_ahead_index);
        if (negotiation_result.first) {
          if ((other_actor_type == ActorType::Vehicle
               && parameters.GetPercentageIgnoreVehicles(ego_actor_id) <= random_devices.at(ego_actor_id).next())
              || (other_actor_type == ActorType::Pedestrian
                  && parameters.GetPercentageIgnoreWalkers(ego_actor_id) <= random_devices.at(ego_actor_id).next())) {
            collision_hazard = true;
            obstacle_id = other_actor_id;
            available_distance_margin = negotiation_result.second;
          }
        }
      }
    }
  }

  CollisionHazardData &output_element = output_array.at(index);
  output_element.hazard_actor_id = obstacle_id;
  output_element.hazard = collision_hazard;
  output_element.available_distance_margin = available_distance_margin;
}

void CollisionStage::RemoveActor(const ActorId actor_id) {
  collision_locks.erase(actor_id);
}

void CollisionStage::Reset() {
  collision_locks.clear();
}

/* 计算指定载具前方包围盒的长度;*/
float CollisionStage::GetBoundingBoxExtention(const ActorId actor_id) {

  const float velocity = cg::Math::Dot(simulation_state.GetVelocity(actor_id), simulation_state.GetHeading(actor_id));//拿到入参actor的速度和前向分量的点积
  float bbox_extension;
  // Using a linear function to calculate boundary length.//用一个线性方程计算边界盒子长度
  bbox_extension = BOUNDARY_EXTENSION_RATE * velocity + BOUNDARY_EXTENSION_MINIMUM;
  // If a valid collision lock present, change boundary length to maintain lock.
  if (collision_locks.find(actor_id) != collision_locks.end()) {//持续查找碰撞锁定表
    const CollisionLock &lock = collision_locks.at(actor_id);//拿到入参actor的碰撞锁定结构体
    float lock_boundary_length = static_cast<float>(lock.distance_to_lead_vehicle + LOCKING_DISTANCE_PADDING);
    // Only extend boundary track vehicle if the leading vehicle
    // if it is not further than velocity dependent extension by MAX_LOCKING_EXTENSION.//
    if ((lock_boundary_length - lock.initial_lock_distance) < MAX_LOCKING_EXTENSION) {
      bbox_extension = lock_boundary_length;
    }
  }

  return bbox_extension;
}

/* 计算指定载具boundingbox附近环绕的多边形点集*/
LocationVector CollisionStage::GetBoundary(const ActorId actor_id) 
{
  const ActorType actor_type = simulation_state.GetType(actor_id);//拿到3D对象种类
  const cg::Vector3D heading_vector = simulation_state.GetHeading(actor_id);//拿到3D对象朝向(单位向量)

  float forward_extension = 0.0f;
  if (actor_type == ActorType::Pedestrian) {
    // Extend the pedestrians bbox to "predict" where they'll be and avoid collisions.
    // 扩展行人bbox，以“预测”他们将在哪里，并避免碰撞。
    // 拿到行人速率乘以行走时间.得到一段行走距离
    forward_extension = simulation_state.GetVelocity(actor_id).Length() * WALKER_TIME_EXTENSION;
  }

  cg::Vector3D dimensions = simulation_state.GetDimensions(actor_id);//拿到3D对象(想象成一个立方体)的中心点

  float bbox_x = dimensions.x;//载具x坐标
  float bbox_y = dimensions.y;//载具y坐标

  const cg::Vector3D x_boundary_vector = heading_vector * (bbox_x + forward_extension);//
  const auto perpendicular_vector = cg::Vector3D(-heading_vector.y, heading_vector.x, 0.0f).MakeSafeUnitVector(EPSILON);
  const cg::Vector3D y_boundary_vector = perpendicular_vector * (bbox_y + forward_extension);

  // Four corners of the vehicle in top view clockwise order (left-handed system).
  const cg::Location location = simulation_state.GetLocation(actor_id);
  LocationVector bbox_boundary = {
      location + cg::Location(x_boundary_vector - y_boundary_vector),
      location + cg::Location(-1.0f * x_boundary_vector - y_boundary_vector),
      location + cg::Location(-1.0f * x_boundary_vector + y_boundary_vector),
      location + cg::Location(x_boundary_vector + y_boundary_vector),
  };

  return bbox_boundary;
}

/* 在车辆的路径边界上构造多边形点*/
LocationVector CollisionStage::GetGeodesicBoundary(const ActorId actor_id) 
{
  LocationVector geodesic_boundary;//位置数组，构建一个测量用边界集合

  //持续查表<ActorID,Locations>映射表 
  if (geodesic_boundary_map.find(actor_id) != geodesic_boundary_map.end()) 
  {
    geodesic_boundary = geodesic_boundary_map.at(actor_id);//边界合集 更新为 对指定actor的位置数组
  } 
  // 假如表里没找到actor的Location数组
  else 
  {
    const LocationVector bbox = GetBoundary(actor_id);//先拿到围绕载具盒子附近的点集

    //持续查找<carla::ActorId, 简易路径点队列>映射表
    if (buffer_map.find(actor_id) != buffer_map.end()) 
    {
      float bbox_extension = GetBoundingBoxExtention(actor_id);//拿到载具前方的包围盒长度
      const float specific_lead_distance = parameters.GetDistanceToLeadingVehicle(actor_id);//查到前车与本车距离
      bbox_extension = std::max(specific_lead_distance, bbox_extension);
      const float bbox_extension_square = SQUARE(bbox_extension);//拿到包围盒的平方

      LocationVector left_boundary;//左边界
      LocationVector right_boundary;//右边界
      cg::Vector3D dimensions = simulation_state.GetDimensions(actor_id);//拿到actor的3d中心
      const float width = dimensions.y;
      const float length = dimensions.x;

      const Buffer &waypoint_buffer = buffer_map.at(actor_id);//本载具查表得到的对应路点集，是一个deque
      // 目标路径点信息, 是一对std::pair<SimpleWaypointPtr, uint64_t>
      // 依据length决定是否开启向前扫描路径点;把路径点缓存都扫描一遍取出路径点信息
      const TargetWPInfo target_wp_info = GetTargetWaypoint(waypoint_buffer, length);

      const SimpleWaypointPtr boundary_start = target_wp_info.first;//具体的路径点
      const uint64_t boundary_start_index = target_wp_info.second;//索引

      // At non-signalized junctions, we extend the boundary across the junction and in all other situations, boundary length is velocity-dependent.
      // 在非信号十字路口，扩展边界穿过交叉口，在所有其他情况下，边界长度是与速度相关联的。  
      SimpleWaypointPtr boundary_end = nullptr;//边界末尾，暂设为空
      SimpleWaypointPtr current_point = waypoint_buffer.at(boundary_start_index);//当前接近的路径点更新为路径缓存里边界起始点

      bool reached_distance = false;
      for (uint64_t j = boundary_start_index; !reached_distance && (j < waypoint_buffer.size()); ++j) //从边界起始点到路径点最后开始遍历每一个路径点索引
      {
        if (boundary_start->DistanceSquared(current_point) > bbox_extension_square //若边界起点到当前载具要接近的路点的距离 大于 载具前方的包围盒长度
            || j == waypoint_buffer.size() - 1                                     // 或者已经查到了最后一个路点  
            ) 
        {
          reached_distance = true;//满足上述任意一种情况就开启已探索开关
        }

        if (boundary_end == nullptr//若还未抵达最后一个路径点 
            || cg::Math::Dot(boundary_end->GetForwardVector(), current_point->GetForwardVector()) < COS_10_DEGREES//若载具驾驶方向偏航角度小于10°
            || reached_distance
            ) 
        {
          /* 多种边界盒子的多边形点集的对于垂直于中间边界的盒子机制*/
          const cg::Vector3D heading_vector = current_point->GetForwardVector();//当前驾驶方向
          const cg::Location location = current_point->GetLocation();//接近点的坐标
          //拿到在x轴上的对称驾驶朝向并规范化
          cg::Vector3D perpendicular_vector = cg::Vector3D(-heading_vector.y, heading_vector.x, 0.0f);
          perpendicular_vector = perpendicular_vector.MakeSafeUnitVector(EPSILON);
          // Direction determined for the left-handed system.
          // 在左手坐标系下执行 载具行驶边界估算
          const cg::Vector3D scaled_perpendicular = perpendicular_vector * width;
          left_boundary.push_back(location + cg::Location(scaled_perpendicular));
          right_boundary.push_back(location + cg::Location(-1.0f * scaled_perpendicular));

          boundary_end = current_point;//并更新最后一个路点为当前要接近的路点
        }

        current_point = waypoint_buffer.at(j);//更新要接近的路点为 j计数
      }
      
      /* 反转右边界;这是因为左右边界数组都有离载具最近的点，也就是右边界的begin()，我们想要从最远的点开始，以顺时针方向走。*/
      std::reverse(right_boundary.begin(), right_boundary.end());
      geodesic_boundary.insert(geodesic_boundary.end(), right_boundary.begin(), right_boundary.end());
      geodesic_boundary.insert(geodesic_boundary.end(), bbox.begin(), bbox.end());
      geodesic_boundary.insert(geodesic_boundary.end(), left_boundary.begin(), left_boundary.end());
    } 
    // 若Buffer表里没找到本载具的Buffer; 就把测边界集合 更新为最初 "围绕载具盒子附近的点集"
    else 
    {
      geodesic_boundary = bbox;
    }

    // 在经过上面IF-ELSE循环条件检查后 往大表<ActorId, LocationVector>里存值,更新映射表
    geodesic_boundary_map.insert({actor_id, geodesic_boundary});
  }

  return geodesic_boundary;
}

Polygon CollisionStage::GetPolygon(const LocationVector &boundary)
{

  traffic_manager::Polygon boundary_polygon;
  for (const cg::Location &location : boundary) {
    bg::append(boundary_polygon.outer(), Point2D(location.x, location.y));
  }
  bg::append(boundary_polygon.outer(), Point2D(boundary.front().x, boundary.front().y));

  return boundary_polygon;
}

GeometryComparison CollisionStage::GetGeometryBetweenActors(const ActorId reference_vehicle_id,
                                                            const ActorId other_actor_id) 
{
  std::pair<ActorId, ActorId> key_parts;
  if (reference_vehicle_id < other_actor_id) {
    key_parts = {reference_vehicle_id, other_actor_id};
  } else {
    key_parts = {other_actor_id, reference_vehicle_id};
  }

  uint64_t actor_id_key = 0u;
  actor_id_key |= key_parts.first;
  actor_id_key <<= 32;
  actor_id_key |= key_parts.second;

  GeometryComparison comparision_result{-1.0, -1.0, -1.0, -1.0};

  if (geometry_cache.find(actor_id_key) != geometry_cache.end()) {

    comparision_result = geometry_cache.at(actor_id_key);
    double mref_veh_other = comparision_result.reference_vehicle_to_other_geodesic;
    comparision_result.reference_vehicle_to_other_geodesic = comparision_result.other_vehicle_to_reference_geodesic;
    comparision_result.other_vehicle_to_reference_geodesic = mref_veh_other;
  } else {

    const Polygon reference_polygon = GetPolygon(GetBoundary(reference_vehicle_id));
    const Polygon other_polygon = GetPolygon(GetBoundary(other_actor_id));

    const Polygon reference_geodesic_polygon = GetPolygon(GetGeodesicBoundary(reference_vehicle_id));

    const Polygon other_geodesic_polygon = GetPolygon(GetGeodesicBoundary(other_actor_id));

    const double reference_vehicle_to_other_geodesic = bg::distance(reference_polygon, other_geodesic_polygon);
    const double other_vehicle_to_reference_geodesic = bg::distance(other_polygon, reference_geodesic_polygon);
    const auto inter_geodesic_distance = bg::distance(reference_geodesic_polygon, other_geodesic_polygon);
    const auto inter_bbox_distance = bg::distance(reference_polygon, other_polygon);

    comparision_result = {reference_vehicle_to_other_geodesic,
              other_vehicle_to_reference_geodesic,
              inter_geodesic_distance,
              inter_bbox_distance};

    geometry_cache.insert({actor_id_key, comparision_result});
  }

  return comparision_result;
}

std::pair<bool, float> CollisionStage::NegotiateCollision(const ActorId reference_vehicle_id,
                                                          const ActorId other_actor_id,
                                                          const uint64_t reference_junction_look_ahead_index) 
{
  /// 这两个是函数的输出值<是否会撞车,可提供的距离边界>
  bool hazard = false;//是否具备危险
  float available_distance_margin = std::numeric_limits<float>::infinity();//可提供距离为无限远

  const cg::Location reference_location = simulation_state.GetLocation(reference_vehicle_id);//取车1的Location
  const cg::Location other_location = simulation_state.GetLocation(other_actor_id);          //取车2的Location

  // Ego and other vehicle heading.
  const cg::Vector3D reference_heading = simulation_state.GetHeading(reference_vehicle_id);//车1驾驶方向前向分量
  // Vector from ego position to position of the other vehicle.
  cg::Vector3D reference_to_other = other_location - reference_location;//车1到车2的向量
  reference_to_other = reference_to_other.MakeSafeUnitVector(EPSILON);//规范化这个向量

  //=======================================================================

  // Other vehicle heading.
  const cg::Vector3D other_heading = simulation_state.GetHeading(other_actor_id);//拿到车2的驾驶方向分量
  // Vector from other vehicle position to ego position.
  cg::Vector3D other_to_reference = reference_location - other_location;//车2到车1的向量
  other_to_reference = other_to_reference.MakeSafeUnitVector(EPSILON);//规范化这个向量

  //=======================================================================

  float reference_vehicle_length = simulation_state.GetDimensions(reference_vehicle_id).x * SQUARE_ROOT_OF_TWO;//车1车身长
  float other_vehicle_length = simulation_state.GetDimensions(other_actor_id).x * SQUARE_ROOT_OF_TWO;//车2车身长

  float inter_vehicle_distance = cg::Math::DistanceSquared(reference_location, other_location);//两车距离²
  float ego_bounding_box_extension = GetBoundingBoxExtention(reference_vehicle_id);//车1前方bounding box长度 
  float other_bounding_box_extension = GetBoundingBoxExtention(other_actor_id);     //车2前方boundingbox长度

  // 计算两车之间的最小距离，要考虑碰撞
  float inter_vehicle_length = reference_vehicle_length + other_vehicle_length;//车1半车长+车2半车长
  float ego_detection_range = SQUARE(ego_bounding_box_extension + inter_vehicle_length);//车1探测距离范围²
  float cross_detection_range = SQUARE(ego_bounding_box_extension + inter_vehicle_length + other_bounding_box_extension);

  // ======以下都是考虑到存在两车撞到一起的情形===========
  bool other_vehicle_in_ego_range = inter_vehicle_distance < ego_detection_range;
  bool other_vehicles_in_cross_detection_range = inter_vehicle_distance < cross_detection_range;
  float reference_heading_to_other_dot = cg::Math::Dot(reference_heading, reference_to_other);
  bool other_vehicle_in_front = reference_heading_to_other_dot > 0;//车1朝向和车12向量点积大于0表明车2在前，车1在后
  const Buffer &reference_vehicle_buffer = buffer_map.at(reference_vehicle_id);//拿到车1的路点队列
  SimpleWaypointPtr closest_point = reference_vehicle_buffer.front();//拿到车1附近最近的一个路点
  bool ego_inside_junction = closest_point->CheckJunction();//用车1附近最近路点来判定车1是否是位于十字路口
  TrafficLightState reference_tl_state = simulation_state.GetTLS(reference_vehicle_id);//归属车1的交通灯结构体
  bool ego_at_traffic_light = reference_tl_state.at_traffic_light;//判断车1所在的十字路口有没有红绿灯
  bool ego_stopped_by_light = reference_tl_state.tl_state != TLS::Green && reference_tl_state.tl_state != TLS::Off;//利用红绿灯颜色,判断处在十字路口车1是否处于等待
  SimpleWaypointPtr look_ahead_point = reference_vehicle_buffer.at(reference_junction_look_ahead_index);//取车1在十字路口要走的下一个路径点
  bool ego_at_junction_entrance = !closest_point->CheckJunction() && look_ahead_point->CheckJunction();//一种情形，即车处于十字路口入口处;车最近的点在十字路口外，但是下下个目标点位于十字路口内侧

  // 处理一些情况下两车相撞时候的应对机制
  if (!(ego_at_junction_entrance && ego_at_traffic_light && ego_stopped_by_light)//没有接近红绿灯和十字路口
      && ((ego_inside_junction && other_vehicles_in_cross_detection_range)
          || (!ego_inside_junction && other_vehicle_in_front && other_vehicle_in_ego_range))//车2领先
      ) 
  {
    GeometryComparison geometry_comparison = GetGeometryBetweenActors(reference_vehicle_id, other_actor_id);//比较两车的行驶边界和各自的碰撞盒子

    // 会撞到的一些情形
    bool geodesic_path_bbox_touching = geometry_comparison.inter_geodesic_distance < 0.1;//两车的路径测量边界盒子接触到了
    bool vehicle_bbox_touching = geometry_comparison.inter_bbox_distance < 0.1;//两车的boundingbox接触到了
    bool ego_path_clear = geometry_comparison.other_vehicle_to_reference_geodesic > 0.1;//车2贴车1路线太近了;准备清除车1路线
    bool other_path_clear = geometry_comparison.reference_vehicle_to_other_geodesic > 0.1;//车1贴车2路线太近了;准备清除车2路线
    bool ego_path_priority = geometry_comparison.reference_vehicle_to_other_geodesic < geometry_comparison.other_vehicle_to_reference_geodesic;//车1到车2边界测量范围小于车2到车1边界范围就让车1先行
    bool other_path_priority = geometry_comparison.reference_vehicle_to_other_geodesic > geometry_comparison.other_vehicle_to_reference_geodesic;//车2到车1边界测量范围大于车2到车1边界范围就让车2先走;点积push，暂存一系列
    bool ego_angular_priority = reference_heading_to_other_dot< cg::Math::Dot(other_heading, other_to_reference);

    // Whichever vehicle's path is farthest away from the other vehicle gets priority to move.
    // 距离车2最远的车辆优先移动。分四个优先级
    bool lower_priority = !ego_path_priority && (other_path_priority || !ego_angular_priority);
    bool blocked_by_other_or_lower_priority = !ego_path_clear || (other_path_clear && lower_priority);
    bool yield_pre_crash = !vehicle_bbox_touching && blocked_by_other_or_lower_priority;
    bool yield_post_crash = vehicle_bbox_touching && !ego_angular_priority;

    // 若存在两车边界盒子重叠了 且追尾或者车速过慢被追尾
    if (geodesic_path_bbox_touching && (yield_pre_crash || yield_post_crash)) 
    {

      hazard = true;//标记危险

      const float reference_lead_distance = parameters.GetDistanceToLeadingVehicle(reference_vehicle_id);//拿到领先距离
      const float specific_distance_margin = std::max(reference_lead_distance, BOUNDARY_EXTENSION_MINIMUM);//计算指定边距
      available_distance_margin = static_cast<float>(std::max(geometry_comparison.reference_vehicle_to_other_geodesic - static_cast<double>(specific_distance_margin), 0.0));//

      ///////////////////////////////////// Collision locking mechanism /////////////////////////////////
      // The idea is, when encountering a possible collision,
      // we should ensure that the bounding box extension doesn't decrease too fast and loose collision tracking.
      // This enables us to smoothly approach the lead vehicle.

      // When possible collision found, check if an entry for collision lock present.
      // 查找车1碰撞锁是否有提供
      if (collision_locks.find(reference_vehicle_id) != collision_locks.end()) 
      {
        CollisionLock &lock = collision_locks.at(reference_vehicle_id);//取车1的碰撞锁
        // Check if the same vehicle is under lock.
        if (other_actor_id == lock.lead_vehicle_id) 
        {
          // If the body of the lead vehicle is touching the reference vehicle bounding box.检查是否存在领先车身接触了车1碰撞盒子
          if (geometry_comparison.other_vehicle_to_reference_geodesic < 0.1) 
          {
            // Distance between the bodies of the vehicles.//更新碰撞锁安全车距为 比较结构体的车距
            lock.distance_to_lead_vehicle = geometry_comparison.inter_bbox_distance;
          } 
          else 
          {
            // Distance from reference vehicle body to other vehicle path polygon.//更新碰撞锁里的安全车距为 车1到车2测量范围点集的距离
            lock.distance_to_lead_vehicle = geometry_comparison.reference_vehicle_to_other_geodesic;
          }
        } 
        else 
        {
          // If possible collision with a new vehicle, re-initialize with new lock entry.
          lock = {
              geometry_comparison.inter_bbox_distance,//比较结构体的车距
              geometry_comparison.inter_bbox_distance,//比较结构体的车距 
              other_actor_id                            //别的车的车号
          };
        }
      } 
      else //若碰撞锁表没查到则初始化一个
      {
        // Insert and initialize lock entry if not present.
        collision_locks.insert(
            {   reference_vehicle_id,
                {                   
                    geometry_comparison.inter_bbox_distance,
                    geometry_comparison.inter_bbox_distance,
                    other_actor_id
                }
            });
      }
    }

  }

  // If no collision hazard detected, then flush collision lock held by the vehicle.
  if (!hazard && collision_locks.find(reference_vehicle_id) != collision_locks.end())//没外车碰撞危险就把车1从碰撞表里抹除
  {
    collision_locks.erase(reference_vehicle_id);
  }

  return {hazard, available_distance_margin};
}

void CollisionStage::ClearCycleCache() {
  geodesic_boundary_map.clear();
  geometry_cache.clear();
}

void CollisionStage::DrawBoundary(const LocationVector &boundary) {
  cg::Location one_meter_up(0.0f, 0.0f, 1.0f);
  for (uint64_t i = 0u; i < boundary.size(); ++i) {
    debug_helper.DrawLine(
        boundary[i] + one_meter_up,
        boundary[i+1 == boundary.size()? 0: i+1] + one_meter_up,
        0.1f, {255u, 255u, 0u}, 0.05f);
  }
}

} // namespace traffic_manager
} // namespace carla

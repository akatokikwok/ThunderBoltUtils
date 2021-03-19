
#pragma once

#include <memory>

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/geometries.hpp"
#include "boost/geometry/geometries/point_xy.hpp"
#include "boost/geometry/geometries/polygon.hpp"

#include "carla/client/DebugHelper.h"

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/DataStructures.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/Parameters.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/RandomGenerator.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/SimulationState.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/Stage.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/client/DebugHelper.h"
#include "D:\Downloads\Carla\carla-0.9.11\LibCarla\source\carla\trafficmanager\Parameters.h"
#include "DataStructures.h"
#include "../../../../../../../Work/UE426/Engine/Plugins/AI/UE4ML/Source/UE4ML/Public/4MLSession.h"
#include "Constants.h"
#include "TrackTraffic.h"
#include "RandomGenerator.h"
#include "Stage.h"
#include <d3d11.h>

namespace carla {
namespace traffic_manager {

struct GeometryComparison {
  double reference_vehicle_to_other_geodesic;//车1车身到车2路径多边形点集
  double other_vehicle_to_reference_geodesic;//车2车身到车1路径多边形点集
  double inter_geodesic_distance;            //两车各自的路线多边形点集交互距离
  double inter_bbox_distance;                //两车盒子交互距离   
};

struct CollisionLock {
  double distance_to_lead_vehicle;//本车离前车距离
  double initial_lock_distance;   //初始安全锁定距离
  ActorId lead_vehicle_id;        //前车ID 
};
using CollisionLockMap = std::unordered_map<ActorId, CollisionLock>;//<ActorID,与前车碰撞锁定结构体>映射表

namespace cc = carla::client;
namespace bg = boost::geometry;

using Buffer = std::deque<std::shared_ptr<SimpleWaypoint>>;//是简易路径点的队列
using BufferMap = std::unordered_map<carla::ActorId, Buffer>;//<carla::ActorId, 简易路径点队列>映射表
using LocationVector = std::vector<cg::Location>;//是Location型数组
using GeodesicBoundaryMap = std::unordered_map<ActorId, LocationVector>;//<ActorID,位置数组>映射表 
using GeometryComparisonMap = std::unordered_map<uint64_t, GeometryComparison>;//<uint64_t, 几何对比结构体>映射表
using Polygon = bg::model::polygon<bg::model::d2::point_xy<double>>;

/// This class has functionality to detect potential collision with a nearby actor.
class CollisionStage : Stage 
{
private:
  const std::vector<ActorId> &vehicle_id_list;//一系列的注册过的载具ID
  const SimulationState &simulation_state;  //3D对象的状态
  const BufferMap &buffer_map;//<carla::ActorId, 简易路径点队列>映射表
  const TrackTraffic &track_traffic;//跟踪所有角色的路径点占用情况。
  const Parameters &parameters;//Parameters型对象，Parameters里含有一大堆表(变道，危险检测什么的)
  CollisionFrame &output_array;//CollisionHazardData结构体数组
  cc::DebugHelper &debug_helper;//绘制几何体的助手类
  // Structure keeping track of blocking lead vehicles.
  CollisionLockMap collision_locks;//<ActorID,碰撞锁定结构体>映射表
  // Structures to cache geodesic boundaries of vehicle and comparision between vehicle boundaries to avoid repeated computation within a cycle.
  // //缓存载具测量边界和车辆边界比较的结构，以避免在一个周期内重复计算。
 
  GeometryComparisonMap geometry_cache;//<uint64_t, 几何对比结构体>映射表
  GeodesicBoundaryMap geodesic_boundary_map;//<ActorID,位置数组>映射表 
  RandomGeneratorMap &random_devices;//<ActorID,随机数生成器类>映射表

  // Method to determine if a vehicle is on a collision path to another.
  /// 判断一辆车是否在通往另一辆车的碰撞路径上。
  std::pair<bool, float> NegotiateCollision(const ActorId reference_vehicle_id,
                                            const ActorId other_actor_id,
                                            const uint64_t reference_junction_look_ahead_index);

  /* 
  * Method to calculate bounding box extention length ahead of the vehicle.
  * 计算指定载具前方包围盒的长度;
  */
  float GetBoundingBoxExtention(const ActorId actor_id);

  // Method to calculate polygon points around the vehicle's bounding box.
  // 计算指定载具boundingbox附近环绕的多边形点集
  LocationVector GetBoundary(const ActorId actor_id);

  // Method to construct polygon points around the path boundary of the vehicle.
  // 在车辆的路径边界上构造多边形点；这些路径点围起来形成一个多边形
  LocationVector GetGeodesicBoundary(const ActorId actor_id);

  Polygon GetPolygon(const LocationVector &boundary);

  // Method to compare path boundaries, bounding boxes of vehicles and cache the results for reuse in current update cycle.
  // //比较行驶路线边界，车辆的边界框，并缓存结果，以便在当前更新周期中重用。
  GeometryComparison GetGeometryBetweenActors(const ActorId reference_vehicle_id,
                                              const ActorId other_actor_id);

  // Method to draw path boundary.
  void DrawBoundary(const LocationVector &boundary);

public:
  CollisionStage(const std::vector<ActorId> &vehicle_id_list,
                 const SimulationState &simulation_state,
                 const BufferMap &buffer_map,
                 const TrackTraffic &track_traffic,
                 const Parameters &parameters,
                 CollisionFrame &output_array,
                 cc::DebugHelper& debug_helper,
                 RandomGeneratorMap &random_devices);

  void Update (const unsigned long index) override;

  void RemoveActor(const ActorId actor_id) override;

  void Reset() override;

  // Method to flush cache for current update cycle.
  void ClearCycleCache();
};

} // namespace traffic_manager
} // namespace carla

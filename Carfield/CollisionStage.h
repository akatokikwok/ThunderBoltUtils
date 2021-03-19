
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
  double reference_vehicle_to_other_geodesic;//��1������2·������ε㼯
  double other_vehicle_to_reference_geodesic;//��2������1·������ε㼯
  double inter_geodesic_distance;            //�������Ե�·�߶���ε㼯��������
  double inter_bbox_distance;                //�������ӽ�������   
};

struct CollisionLock {
  double distance_to_lead_vehicle;//������ǰ������
  double initial_lock_distance;   //��ʼ��ȫ��������
  ActorId lead_vehicle_id;        //ǰ��ID 
};
using CollisionLockMap = std::unordered_map<ActorId, CollisionLock>;//<ActorID,��ǰ����ײ�����ṹ��>ӳ���

namespace cc = carla::client;
namespace bg = boost::geometry;

using Buffer = std::deque<std::shared_ptr<SimpleWaypoint>>;//�Ǽ���·����Ķ���
using BufferMap = std::unordered_map<carla::ActorId, Buffer>;//<carla::ActorId, ����·�������>ӳ���
using LocationVector = std::vector<cg::Location>;//��Location������
using GeodesicBoundaryMap = std::unordered_map<ActorId, LocationVector>;//<ActorID,λ������>ӳ��� 
using GeometryComparisonMap = std::unordered_map<uint64_t, GeometryComparison>;//<uint64_t, ���ζԱȽṹ��>ӳ���
using Polygon = bg::model::polygon<bg::model::d2::point_xy<double>>;

/// This class has functionality to detect potential collision with a nearby actor.
class CollisionStage : Stage 
{
private:
  const std::vector<ActorId> &vehicle_id_list;//һϵ�е�ע������ؾ�ID
  const SimulationState &simulation_state;  //3D�����״̬
  const BufferMap &buffer_map;//<carla::ActorId, ����·�������>ӳ���
  const TrackTraffic &track_traffic;//�������н�ɫ��·����ռ�������
  const Parameters &parameters;//Parameters�Ͷ���Parameters�ﺬ��һ��ѱ�(�����Σ�ռ��ʲô��)
  CollisionFrame &output_array;//CollisionHazardData�ṹ������
  cc::DebugHelper &debug_helper;//���Ƽ������������
  // Structure keeping track of blocking lead vehicles.
  CollisionLockMap collision_locks;//<ActorID,��ײ�����ṹ��>ӳ���
  // Structures to cache geodesic boundaries of vehicle and comparision between vehicle boundaries to avoid repeated computation within a cycle.
  // //�����ؾ߲����߽�ͳ����߽�ȽϵĽṹ���Ա�����һ���������ظ����㡣
 
  GeometryComparisonMap geometry_cache;//<uint64_t, ���ζԱȽṹ��>ӳ���
  GeodesicBoundaryMap geodesic_boundary_map;//<ActorID,λ������>ӳ��� 
  RandomGeneratorMap &random_devices;//<ActorID,�������������>ӳ���

  // Method to determine if a vehicle is on a collision path to another.
  /// �ж�һ�����Ƿ���ͨ����һ��������ײ·���ϡ�
  std::pair<bool, float> NegotiateCollision(const ActorId reference_vehicle_id,
                                            const ActorId other_actor_id,
                                            const uint64_t reference_junction_look_ahead_index);

  /* 
  * Method to calculate bounding box extention length ahead of the vehicle.
  * ����ָ���ؾ�ǰ����Χ�еĳ���;
  */
  float GetBoundingBoxExtention(const ActorId actor_id);

  // Method to calculate polygon points around the vehicle's bounding box.
  // ����ָ���ؾ�boundingbox�������ƵĶ���ε㼯
  LocationVector GetBoundary(const ActorId actor_id);

  // Method to construct polygon points around the path boundary of the vehicle.
  // �ڳ�����·���߽��Ϲ������ε㣻��Щ·����Χ�����γ�һ�������
  LocationVector GetGeodesicBoundary(const ActorId actor_id);

  Polygon GetPolygon(const LocationVector &boundary);

  // Method to compare path boundaries, bounding boxes of vehicles and cache the results for reuse in current update cycle.
  // //�Ƚ���ʻ·�߽߱磬�����ı߽�򣬲����������Ա��ڵ�ǰ�������������á�
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

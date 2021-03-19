/// 


#pragma once

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/DataStructures.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/Parameters.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/RandomGenerator.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/SimulationState.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/Stage.h"
#include "../client/Timestamp.h"
#include "../client/World.h"
#include <unordered_map>

namespace carla {
namespace traffic_manager {

/// This class has functionality for responding to traffic lights
/// and managing entry into non-signalized junctions.
private:
  const std::vector<ActorId> &vehicle_id_list;//载具列表
  const SimulationState &simulation_state;//当前3D世界的模拟状态
  const BufferMap &buffer_map;//载具--简易路点集映射表
  const Parameters &parameters;
  const cc::World &world;
  /// Map containing the time ticket issued for vehicles.
  std::unordered_map<ActorId, cc::Timestamp> vehicle_last_ticket;
  /// Map containing the previous time ticket issued for junctions.
  std::unordered_map<JunctionID, cc::Timestamp> junction_last_ticket;
  /// Map containing the previous junction visited by a vehicle.
  std::unordered_map<ActorId, JunctionID> vehicle_last_junction;
  TLFrame &output_array;
  RandomGeneratorMap &random_devices;
  cc::Timestamp current_timestamp;

  bool HandleNonSignalisedJunction(const ActorId ego_actor_id, const JunctionID junction_id,
                                   cc::Timestamp timestamp);

public:
  TrafficLightStage(const std::vector<ActorId> &vehicle_id_list,
                    const SimulationState &Simulation_state,
                    const BufferMap &buffer_map,
                    const Parameters &parameters,//自动规则一些参数
                    const cc::World &world,
                    TLFrame &output_array,
                    RandomGeneratorMap &random_devices);

  void Update(const unsigned long index) override;

  void RemoveActor(const ActorId actor_id) override;

  void Reset() override;
};

} // namespace traffic_manager
} // namespace carla

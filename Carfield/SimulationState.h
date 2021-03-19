
#pragma once

#include <unordered_set>

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/trafficmanager/DataStructures.h"
#include <unordered_map>

namespace carla {
namespace traffic_manager {

enum ActorType {
  Vehicle,
  Pedestrian,
  Any
};
/* 载具的 位置、朝向、速度、限速、物理效果的结构体*/
struct KinematicState {
  cg::Location location;
  cg::Rotation rotation;
  cg::Vector3D velocity;
  float speed_limit;
  bool physics_enabled;
};
/* <ActorID-KinematicState>映射表*/
using KinematicStateMap = std::unordered_map<ActorId, KinematicState>;

struct TrafficLightState {
  TLS tl_state;
  bool at_traffic_light;
};
using TrafficLightStateMap = std::unordered_map<ActorId, TrafficLightState>;

struct StaticAttributes {
  ActorType actor_type;
  float half_length;
  float half_width;
  float half_height;
};
using StaticAttributeMap = std::unordered_map<ActorId, StaticAttributes>;

/// This class holds the state of all the vehicles in the simlation.
/// 包含所有处于模拟控制下的3D对象的状态 的类;提供一系列结构以供外部获取各个状态
class SimulationState {

private:
  // Structure to hold ids of all actors in the simulation.
  // ActorID集合
  std::unordered_set<ActorId> actor_set;
  // Structure containing dynamic motion related state of actors.
  // <ActorId, 载具运动状态>的映射表;
  KinematicStateMap kinematic_state_map;
  // Structure containing static attributes of actors.
  // <ActorId, Actors静态属性>的映射表
  StaticAttributeMap static_attribute_map;
  // Structure containing dynamic traffic light related state of actors.
  // <ActorID, 交通信号结构体>的映射表
  TrafficLightStateMap tl_state_map;

public :
  SimulationState();

  // Method to add an actor to the simulation state.
  // 将指定的Actor添加进模拟状态，同时设置他们的各个结构体数据
  void AddActor(ActorId actor_id,
                KinematicState kinematic_state,
                StaticAttributes attributes,
                TrafficLightState tl_state);

  // Method to verify if an actor is present currently present in the simulation state.
  // 检查指定的actor是否位于模拟控制中
  bool ContainsActor(ActorId actor_id) const;

  // Method to remove an actor from simulation state.
  // 将指定的actor移除出模拟控制
  void RemoveActor(ActorId actor_id);

  // Method to flush all states and actors.
  // 将所有的actor和映射表都清空
  void Reset();

  /* 用指定的结构体更新<ActorId, 载具运动状态>的映射表;*/
  void UpdateKinematicState(ActorId actor_id, KinematicState state);

  /* 用指定的交通灯结构体更新<ActorID, 交通信号结构体>的映射表*/
  void UpdateTrafficLightState(ActorId actor_id, TrafficLightState state);
  /* 拿指定actor的位置*/
  cg::Location GetLocation(const ActorId actor_id) const;
  /* 拿指定actor的朝向*/
  cg::Rotation GetRotation(const ActorId actor_id) const;
  /* 拿指定actor的前向向量*/
  cg::Vector3D GetHeading(const ActorId actor_id) const;
  /* 拿指定actor的速度*/
  cg::Vector3D GetVelocity(const ActorId actor_id) const;
  /* 拿指定actor的限速*/
  float GetSpeedLimit(const ActorId actor_id) const;
  /* 拿指定actor的物理启用状态*/
  bool IsPhysicsEnabled(const ActorId actor_id) const;
  /* 拿指定actor的交通灯结构体*/
  TrafficLightState GetTLS(const ActorId actor_id) const;
  /* 拿到指定actor所属的种类类型(载具还是行人还是both)*/
  ActorType GetType(const ActorId actor_id) const;
  /* 拿到指定actor的属性的三个分量构成一个vector*//* */
  cg::Vector3D GetDimensions(const ActorId actor_id) const;

};

} // namespace traffic_manager
} // namespace carla


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
/* �ؾߵ� λ�á������ٶȡ����١�����Ч���Ľṹ��*/
struct KinematicState {
  cg::Location location;
  cg::Rotation rotation;
  cg::Vector3D velocity;
  float speed_limit;
  bool physics_enabled;
};
/* <ActorID-KinematicState>ӳ���*/
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
/// �������д���ģ������µ�3D�����״̬ ����;�ṩһϵ�нṹ�Թ��ⲿ��ȡ����״̬
class SimulationState {

private:
  // Structure to hold ids of all actors in the simulation.
  // ActorID����
  std::unordered_set<ActorId> actor_set;
  // Structure containing dynamic motion related state of actors.
  // <ActorId, �ؾ��˶�״̬>��ӳ���;
  KinematicStateMap kinematic_state_map;
  // Structure containing static attributes of actors.
  // <ActorId, Actors��̬����>��ӳ���
  StaticAttributeMap static_attribute_map;
  // Structure containing dynamic traffic light related state of actors.
  // <ActorID, ��ͨ�źŽṹ��>��ӳ���
  TrafficLightStateMap tl_state_map;

public :
  SimulationState();

  // Method to add an actor to the simulation state.
  // ��ָ����Actor��ӽ�ģ��״̬��ͬʱ�������ǵĸ����ṹ������
  void AddActor(ActorId actor_id,
                KinematicState kinematic_state,
                StaticAttributes attributes,
                TrafficLightState tl_state);

  // Method to verify if an actor is present currently present in the simulation state.
  // ���ָ����actor�Ƿ�λ��ģ�������
  bool ContainsActor(ActorId actor_id) const;

  // Method to remove an actor from simulation state.
  // ��ָ����actor�Ƴ���ģ�����
  void RemoveActor(ActorId actor_id);

  // Method to flush all states and actors.
  // �����е�actor��ӳ������
  void Reset();

  /* ��ָ���Ľṹ�����<ActorId, �ؾ��˶�״̬>��ӳ���;*/
  void UpdateKinematicState(ActorId actor_id, KinematicState state);

  /* ��ָ���Ľ�ͨ�ƽṹ�����<ActorID, ��ͨ�źŽṹ��>��ӳ���*/
  void UpdateTrafficLightState(ActorId actor_id, TrafficLightState state);
  /* ��ָ��actor��λ��*/
  cg::Location GetLocation(const ActorId actor_id) const;
  /* ��ָ��actor�ĳ���*/
  cg::Rotation GetRotation(const ActorId actor_id) const;
  /* ��ָ��actor��ǰ������*/
  cg::Vector3D GetHeading(const ActorId actor_id) const;
  /* ��ָ��actor���ٶ�*/
  cg::Vector3D GetVelocity(const ActorId actor_id) const;
  /* ��ָ��actor������*/
  float GetSpeedLimit(const ActorId actor_id) const;
  /* ��ָ��actor����������״̬*/
  bool IsPhysicsEnabled(const ActorId actor_id) const;
  /* ��ָ��actor�Ľ�ͨ�ƽṹ��*/
  TrafficLightState GetTLS(const ActorId actor_id) const;
  /* �õ�ָ��actor��������������(�ؾ߻������˻���both)*/
  ActorType GetType(const ActorId actor_id) const;
  /* �õ�ָ��actor�����Ե�������������һ��vector*//* */
  cg::Vector3D GetDimensions(const ActorId actor_id) const;

};

} // namespace traffic_manager
} // namespace carla

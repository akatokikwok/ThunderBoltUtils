// Copyright (c) 2020 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <memory.h>

#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/client/Waypoint.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/geom/Location.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/geom/Transform.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/geom/Vector3D.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/Memory.h"
#include "D:/Downloads/Carla/carla-0.9.11/LibCarla/source/carla/road/RoadTypes.h"

namespace carla {
namespace traffic_manager {

  namespace cc = carla::client;
  namespace cg = carla::geom;
  using WaypointPtr = carla::SharedPtr<cc::Waypoint>;
  using GeoGridId = carla::road::JuncId;

  /// This is a simple wrapper class on Carla's waypoint object.
  /// The class is used to represent discrete samples of the world map.
  /// ��ʾ��ͼ����ɢ����
  class SimpleWaypoint {

    using SimpleWaypointPtr = std::shared_ptr<SimpleWaypoint>;

  private:

    /// Pointer to Carla's waypoint object around which this class wraps around.
    /// 
    WaypointPtr waypoint;
    /// List of pointers to next connecting waypoints.
    std::vector<SimpleWaypointPtr> next_waypoints;
    /// List of pointers to previous connecting waypoints.
    std::vector<SimpleWaypointPtr> previous_waypoints;
    /// Pointer to left lane change waypoint.
    SimpleWaypointPtr next_left_waypoint;
    /// Pointer to right lane change waypoint.
    SimpleWaypointPtr next_right_waypoint;
    /// Integer placing the waypoint into a geodesic grid.
    GeoGridId geodesic_grid_id = 0;
    /// Boolean to hold if the waypoint belongs to a junction;����·���������һ��ʮ��·��
    bool _is_junction = false;

  public:

    SimpleWaypoint(WaypointPtr _waypoint);
    ~SimpleWaypoint();

    /// Returns the location object for this waypoint.;�õ�cc::Waypointd��λ��
    cg::Location GetLocation() const;

    /// Returns a carla::shared_ptr to carla::waypoint.;�õ�cc::Waypoint������ָ��
    WaypointPtr GetWaypoint() const;

    /// Returns the list of next waypoints.�õ���Աnext_waypoints
    std::vector<SimpleWaypointPtr> GetNextWaypoint() const;

    /// Returns the list of previous waypoints.�õ���Աprevious_waypoints
    std::vector<SimpleWaypointPtr> GetPreviousWaypoint() const;

    /// Returns the vector along the waypoint's direction.; 
    /// �õ���Աcc::Waypointd����ǰ����
    cg::Vector3D GetForwardVector() const;

    /// Returns the unique id for the waypoint.�õ�·�����Ψһʶ��ID
    uint64_t GetId() const;

    /// This method is used to set the next waypoints.��ָ����һ��·��ȥ���ó�Աnext_waypoints
    uint64_t SetNextWaypoint(const std::vector<SimpleWaypointPtr> &next_waypoints);

    /// This method is used to set the previous waypoints.��ָ����һ��·��ȥ���ó�Աprevious_waypoints
    uint64_t SetPreviousWaypoint(const std::vector<SimpleWaypointPtr> &next_waypoints);

    /// This method is used to set the closest left waypoint for a lane change.��ָ��·����������������·��(��xy�����Ȳ���㷨������)
    void SetLeftWaypoint(SimpleWaypointPtr &waypoint);

    /// This method is used to set the closest right waypoint for a lane change.��ָ��·��������������Ҳ�·��
    void SetRightWaypoint(SimpleWaypointPtr &waypoint);

    /// This method is used to get the closest left waypoint for a lane change.�õ������cc::waypoint���
    SimpleWaypointPtr GetLeftWaypoint();

    /// This method is used to get the closest right waypoint for a lane change.�õ������cc::waypoint�Ҳ�
    SimpleWaypointPtr GetRightWaypoint();

    /// Accessor methods for geodesic grid id. ��ָ��GeoGridId����ȥ�������Ա_geodesic_grid_id
    void SetGeodesicGridId(GeoGridId _geodesic_grid_id);
    /// ���ݵ�ǰ·��״̬�ж�ȡ·���junctionID����ȡĬ�ϵ����Ա_geodesic_grid_id
    GeoGridId GetGeodesicGridId();

    /// Method to retreive junction id of the waypoint.;�õ���ǰ·���junctionID
    GeoGridId GetJunctionId() const;

    /// Calculates the distance from the object's waypoint to the passed location. �������·���㵽ָ��λ�õľ���
    float Distance(const cg::Location &location) const;

    /// Calculates the distance the other SimpleWaypoint object.����ָ����·���㵽��·����ľ���
    float Distance(const SimpleWaypointPtr &other) const;

    /// Calculates the square of the distance to given location.
    float DistanceSquared(const cg::Location &location) const;

    /// Calculates the square of the distance to other waypoints.
    float DistanceSquared(const SimpleWaypointPtr &other) const;

    /// Returns true if the object's waypoint belongs to an intersection.//����·�����Ƿ������һ��ʮ��·��
    bool CheckJunction() const;

    /// This method is used to set whether the waypoint belongs to a junction.//�������Ա_is_junction
    void SetIsJunction(bool value);

    /// Returns true if the object's waypoint belongs to an intersection (Doesn't use OpenDrive).
    /// �ڲ�ʹ��opendrive����� ����Ƿ���next_waypoints·�����ǲ��Ǵ���1
    bool CheckIntersection() const;

    /// Return transform object for the current waypoint.
    cg::Transform GetTransform() const;
  };

} // namespace traffic_manager
} // namespace carla

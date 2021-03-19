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
  /// 表示地图的离散样本
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
    /// Boolean to hold if the waypoint belongs to a junction;检查此路径点归属于一个十字路口
    bool _is_junction = false;

  public:

    SimpleWaypoint(WaypointPtr _waypoint);
    ~SimpleWaypoint();

    /// Returns the location object for this waypoint.;拿到cc::Waypointd的位置
    cg::Location GetLocation() const;

    /// Returns a carla::shared_ptr to carla::waypoint.;拿到cc::Waypoint的智能指针
    WaypointPtr GetWaypoint() const;

    /// Returns the list of next waypoints.拿到成员next_waypoints
    std::vector<SimpleWaypointPtr> GetNextWaypoint() const;

    /// Returns the list of previous waypoints.拿到成员previous_waypoints
    std::vector<SimpleWaypointPtr> GetPreviousWaypoint() const;

    /// Returns the vector along the waypoint's direction.; 
    /// 拿到成员cc::Waypointd的向前分量
    cg::Vector3D GetForwardVector() const;

    /// Returns the unique id for the waypoint.拿到路径点的唯一识别ID
    uint64_t GetId() const;

    /// This method is used to set the next waypoints.用指定的一组路点去设置成员next_waypoints
    uint64_t SetNextWaypoint(const std::vector<SimpleWaypointPtr> &next_waypoints);

    /// This method is used to set the previous waypoints.用指定的一组路点去设置成员previous_waypoints
    uint64_t SetPreviousWaypoint(const std::vector<SimpleWaypointPtr> &next_waypoints);

    /// This method is used to set the closest left waypoint for a lane change.以指定路点来设置最近的左侧路点(以xy分量比差的算法来计算)
    void SetLeftWaypoint(SimpleWaypointPtr &waypoint);

    /// This method is used to set the closest right waypoint for a lane change.以指定路点来设置最近的右侧路点
    void SetRightWaypoint(SimpleWaypointPtr &waypoint);

    /// This method is used to get the closest left waypoint for a lane change.拿到最近的cc::waypoint左侧
    SimpleWaypointPtr GetLeftWaypoint();

    /// This method is used to get the closest right waypoint for a lane change.拿到最近的cc::waypoint右侧
    SimpleWaypointPtr GetRightWaypoint();

    /// Accessor methods for geodesic grid id. 用指定GeoGridId对象去设置类成员_geodesic_grid_id
    void SetGeodesicGridId(GeoGridId _geodesic_grid_id);
    /// 根据当前路点状态判断取路点的junctionID还是取默认的类成员_geodesic_grid_id
    GeoGridId GetGeodesicGridId();

    /// Method to retreive junction id of the waypoint.;拿到当前路点的junctionID
    GeoGridId GetJunctionId() const;

    /// Calculates the distance from the object's waypoint to the passed location. 计算这个路径点到指定位置的距离
    float Distance(const cg::Location &location) const;

    /// Calculates the distance the other SimpleWaypoint object.计算指定的路径点到本路径点的距离
    float Distance(const SimpleWaypointPtr &other) const;

    /// Calculates the square of the distance to given location.
    float DistanceSquared(const cg::Location &location) const;

    /// Calculates the square of the distance to other waypoints.
    float DistanceSquared(const SimpleWaypointPtr &other) const;

    /// Returns true if the object's waypoint belongs to an intersection.//检查此路径点是否归属于一个十字路口
    bool CheckJunction() const;

    /// This method is used to set whether the waypoint belongs to a junction.//设置类成员_is_junction
    void SetIsJunction(bool value);

    /// Returns true if the object's waypoint belongs to an intersection (Doesn't use OpenDrive).
    /// 在不使用opendrive情况下 检查是否是next_waypoints路点数是不是大于1
    bool CheckIntersection() const;

    /// Return transform object for the current waypoint.
    cg::Transform GetTransform() const;
  };

} // namespace traffic_manager
} // namespace carla

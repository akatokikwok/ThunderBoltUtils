/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: CarField交通流地图接口
 * Author: 陈名扬c00417849
 * Create: 2020-08-26
 */

#ifndef CARFIELD_MAP_API_H
#define CARFIELD_MAP_API_H

#include <..\ThirdParty\carfield_map_api\include\hdmap_data.h>
#include <unordered_map>
#include <queue>
#include <string>
#include <atomic>
#include <mutex>

namespace carfield_map {
enum ErrorCode : int32_t {
    CARFIELD_MAP_FAIL = -1,
    CARFIELD_MAP_OK = 0
};

enum class RelativeDirection {
    Clockwise = 0,
    Counterclockwise,
    Parallel,
    Vertical
};

enum class LaneTurnType {
    TURN_LEFT = 0,
    TURN_RIGHT,
    GO_STRAIGHT,
    U_TURN,
    NO_INTERSECTION
};

enum class CoordinateTransformation {
    UTM_TO_LLA = 0,
    LLA_TO_UTM
};

class CarfieldMapApiImpl;
class CarfieldMapApi {
public:
    explicit CarfieldMapApi();

    CarfieldMapApi(const CarfieldMapApi &) = delete;
    CarfieldMapApi &operator = (const CarfieldMapApi &) = delete;

    ~CarfieldMapApi();

    /* *
     * @brief    地图实例初始化
     *
     * @param     path       地图路径
     * @param     name       地图名称
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      初始化成功
     * @retval    CARFIELD_MAP_FAIL    初始化失败
     */
    int32_t Inwwdit(const std::string &path = ".", const std::string &name = "");

    // ##################################################################################
    // ************************************ 地图原始接口 **********************************
    // ##################################################################################
    /* *
     * @brief 根据坐标和半径生成范围,获取指定范围内的指定类别所有要素
     *
     * @param   point           坐标
     * @param   distance        半径
     * @param   type            属性类型
     * @param   features
     * @return  int32_t
     * @retval  CARFIELD_MAP_OK         查找成功
     * @retval  CARFIELD_MAP_FAIL       查找失败
     */
    int32_t GetFeaturesByPosition(const hdmap::Point &point, const double &distance, const hdmap::FeatureType &type,
        std::vector<hdmap::FeaturePtr> &features);

    /* *
     * @brief   根据属性id获取要素
     *
     * @param   featureId       属性id
     * @param   type            属性类型
     * @param   feature
     * @return  int32_t
     * @retval  CARFIELD_MAP_OK         查找成功
     * @retval  CARFIELD_MAP_FAIL       查找失败
     */
    int32_t GetFeatureByID(const uint64_t &featureId, const hdmap::FeatureType &type, hdmap::FeaturePtr &feature);

    /* *
     * @brief   根据坐标获取要素
     *
     * @param   point           坐标
     * @param   type            属性类型
     * @param   results
     * @return  int32_t
     * @retval  CARFIELD_MAP_OK         查找成功
     * @retval  CARFIELD_MAP_FAIL       查找失败
     */
    int32_t GetFeaturesByLocated(const hdmap::Point &point, const hdmap::FeatureType &type,
        std::vector<hdmap::FeaturePtr> &results);

    /* *
     * @brief   根据3D坐标获取要素
     *
     * @param   point           3D坐标
     * @param   type            属性类型
     * @param   results
     * @return  int32_t
     * @retval  CARFIELD_MAP_OK         查找成功
     * @retval  CARFIELD_MAP_FAIL       查找失败
     */
    int32_t GetFeaturesByLocated3D(const hdmap::Point &point, const hdmap::FeatureType &type,
        std::vector<hdmap::FeaturePtr> &results);

    /* *
     * @brief 获取数据版本
     */
    std::string GetDataVersion();

    // ##################################################################################
    // ************************************ 路径规划接口 **********************************
    // ##################################################################################
    /* *
     * @brief    根据起始点道路ID、坐标,在HD map中计算一条从起点到终点的道路
     * @param    start             点投影点坐标
     * @param    end               终点投影点坐标
     * @param    road_list         寻路结果
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t CalcRoutePath(const hdmap::Point &start, const hdmap::Point &end, std::vector<hdmap::MapId> &road_list);

    // ##################################################################################
    // ************************************ 道路/车道接口 *********************************
    // ##################################################################################
    /* *
     * @brief 根据坐标和两个半径生成非重叠的范围,获取指定范围内的指定类别所有要素
     *
     * @param   point           坐标
     * @param   maxDistance     大范围半径
     * @param   minDistance     小范围半径
     * @param   type            属性类型
     * @param   features
     * @return  int32_t
     * @retval  CARFIELD_MAP_OK         查找成功
     * @retval  CARFIELD_MAP_FAIL       查找失败
     */
    int32_t GetFeaturesByPosition(const hdmap::Point &point, const double &maxDistance, const double &minDistance,
        const hdmap::FeatureType &type, std::vector<hdmap::FeaturePtr> &features);

    /* *
     * @brief    获取feature点
     *
     * @param     feature        common feature
     * @param     featurePoints    feature geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetFeaturePoints(const hdmap::FeaturePtr &feature, std::vector<hdmap::Point> &featurePoints);

    /* *
     * @brief    根据道路ID获取道路feature
     *
     * @param     roadId       道路ID
     * @param     road         道路feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetRoad(const hdmap::MapId &roadId, hdmap::RoadPtr &road);

    /* *
     * @brief    根据道路ID获取道路feature
     */
    hdmap::RoadType GetRoadType(const hdmap::MapId &roadId);

    /* *
     * @brief    获取Road geometry点
     *
     * @param     road        road_feature
     * @param     roadPoints    road geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetRoadPoints(const hdmap::RoadPtr &road, std::vector<hdmap::Point> &roadPoints);

    /* *
     * @brief     根据指定位置查找所在道路
     * @note      如果指定位置在某个道路内，返回该道路ID，否则返回失败
     * @note      如果指定位置有多个道路（例如高架上下），返回所有道路ID
     *
     * @param     location     指定位置
     * @param     roadIds      道路ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      查找成功
     * @retval    CARFIELD_MAP_FAIL    查找失败
     */
    int32_t GetRoadIdsByLocated(const hdmap::Point &location, std::vector<hdmap::MapId> &roadIds);

    /* *
     * @brief     根据指定位置和道路查找所在车道
     *
     * @param     location     指定位置
     * @param     roadId       道路ID
     * @param     laneId       车道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      查找成功
     * @retval    CARFIELD_MAP_FAIL    查找失败
     */
    int32_t GetLaneIdByLocatedWithRoad(const hdmap::Point &location, hdmap::MapId roadId, hdmap::MapId &laneId);

    /* *
     * @brief    获取道路关联的车道ID
     * @note     返回的车道ID序列横向从左到右，纵向从后到前（沿道路通行方向）
     *
     * @param     roadId       道路ID
     * @param     lanesId      关联的车道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetRoadRelatedLanesId(const hdmap::MapId &roadId, std::vector<hdmap::MapId> &lanesId);

    /* *
     * @brief    根据车道ID获取车道feature
     *
     * @param     laneId       车道ID
     * @param     lane         车道feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLane(const hdmap::MapId &laneId, hdmap::LanePtr &lane);

    int32_t GetIntersectionLaneType(const hdmap::MapId &laneId, LaneTurnType &laneTurnType);

    /* *
     * @brief    根据lane ID判断该Lane是否是路口出口道
     */
    bool IsLaneIntersectionOutLane(const hdmap::MapId &laneId);
    bool IsLaneIntersectionInLane(const hdmap::MapId &laneId);

    /* *
     * @brief    根据lane ID判断该Lane是否是路口车道
     */
    bool IsLaneIntersectionLane(const hdmap::MapId &laneId);

    /* *
     * @brief     根据车道ID获取车道的限速值
     * @note      如果车道有多段时，取最小的maxSpeed作为限速值
     *
     * @param     laneId        车道ID
     * @param     speedLimit    限速值（km/h）
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLaneSpeedLimit(const hdmap::MapId &laneId, uint32_t &speedLimit);

    /* *
     * @brief     获取一系列lanes的总长度
     * @note      单位为米
     *
     * @param     laneIds       车道ids
     * @param     length       车道总长度
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLanesLength(const std::vector<hdmap::MapId> &laneIds, double &length);
    int32_t GetLanesLength(const std::deque<hdmap::MapId> &laneIds, double &length);

    /* *
     * @brief     获取lanesRoutes的第LaneIndex条lane上的第PointIndex个点到起始点的沿着道路方向的距离
     * @note      单位为米
     */
    double GetDistanceFromLanesBegin(const std::deque<hdmap::MapId> &lanesRoutes, int laneIndex, int pointIndex);

    /* *
     * @brief
     * 从指定起点（lanesRoutes的下标序号为startLaneIndex的lane的第startPointIndex个点）开始，寻找指定距离处的车道点坐标
     * 如果findDistance超过lanesRoutes的最后一个点，则返回最后一个点
     * @param     findDistance     寻找距离
     * @param     result           查找结果
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetPointOnLanesRoutes(const std::deque<hdmap::MapId> &lanesRoutes, unsigned int startLaneIndex,
        unsigned int startPointIndex, double findDistance, hdmap::Point &result);

    /* *
     * @brief     从车道起点开始，寻找指定距离处的车道点坐标
     *
     * @param     laneId           车道id
     * @param     findDistance     寻找距离
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetPointOnLane(hdmap::MapId lane, double findDistance, hdmap::Point &result);

    /* *
     * @brief     根据指定位置在车道坐标系下的坐标
     * @note      车道坐标系：沿车道方向为x,朝前为正；垂直于车道方向为y,朝左为正，表示同车道中心线的距离
     *
     * @param     points       车道中心线
     * @param     location     指定位置
     * @param     roadX        车道坐标系x
     * @param     roadY        车道坐标系y
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      计算成功
     * @retval    CARFIELD_MAP_FAIL    计算失败
     */
    int32_t GetLaneXyPos(const std::vector<hdmap::Point> &points, const hdmap::Point &location, double &roadX,
        double &roadY, unsigned int &nearestIndex);

    /* *
     * @brief     根据指定位置计算到所在车道起点的距离
     * @note      如果指定位置有多个车道（例如高架上下），返回距离所有车道起点的距离
     *
     * @param     location     指定位置
     * @param     distance     指定位置到车道起点的距离
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      计算成功
     * @retval    CARFIELD_MAP_FAIL    计算失败
     */
    int32_t CalcDistanceFromLaneBegin(const hdmap::Point &location, double &distance);

    /* *
     * @brief     根据车道ID获取车道长度
     * @note      单位为米
     *
     * @param     laneId       车道id
     * @param     length       车道长度，单位:m
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLaneLength(const hdmap::MapId &laneId, double &length);

    /* *
     * @brief    根据lane内的坐标点以及lane ID获取lane的宽度
     *
     * @param     location       车道坐标点
     * @param     laneId         车道ID
     * @param     distance       宽度
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLaneWidth(const hdmap::Point &location, const hdmap::MapId &laneId, double &width);

    /* *
     * @brief    获取车道的左侧车道ID
     *
     * @param     laneId        车道ID
     * @param     leftLaneId    左侧车道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLeftLaneId(const hdmap::MapId &laneId, hdmap::MapId &leftLaneId);

    /* *
     * @brief    获取车道的右侧车道ID
     *
     * @param     laneId         车道ID
     * @param     rightLaneId    右侧车道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK        获取成功
     * @retval    CARFIELD_MAP_FAIL      获取失败
     */
    int32_t GetRightLaneId(const hdmap::MapId &laneId, hdmap::MapId &rightLaneId);

    /* *
     * @brief    获取车道的前方相连车道集合
     */
    std::vector<hdmap::MapId> GetNextLaneIds(const hdmap::MapId &laneId);

    /* *
     * @brief    获取车道中心点
     *
     * @param     laneId        车道ID
     * @param     lanePoints    车道中心点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLanePoints(const hdmap::MapId &laneId, std::vector<hdmap::Point> &lanePoints);

    /* *
     * @brief    获取车道中心点
     *
     * @param     lane        车道feature
     * @param     lanePoints    车道中心点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLanePoints(const hdmap::LanePtr &lane, std::vector<hdmap::Point> &lanePoints);

    /* *
     * @brief    获取车道指定路点的车道行驶方向
     *
     * @param     lanePoints    车道坐标点
     * @param     index         路点序号
     * @param     direction     车道行驶方向
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLanePointDirection(const std::vector<hdmap::Point> &lanePoints, uint32_t index, double &direction);

    /* *
     * @brief    获取当前位置车道行驶方向
     *
     * @param     location    车道坐标点
     * @param     direction     车道行驶方向
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetLaneDirection(const hdmap::Point &location, double &direction);

    /* *
     * @brief    根据ID获取车道边界feature
     *
     * @param     boundaryId       车道边界ID
     * @param     boundary         车道边界feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetBoundary(const hdmap::MapId &boundaryId, hdmap::BoundaryPtr &boundary);

    /* *
     * @brief     获取boundary geometry点
     *
     * @param     boundary                 boundary feature
     * @param     boundaryPoints     boundary geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK                获取成功
     * @retval    CARFIELD_MAP_FAIL              获取失败
     */
    int32_t GetBoundaryPoints(const hdmap::BoundaryPtr &boundary, std::vector<hdmap::Point> &boundaryPoints);

    /* *
     * @brief     获取车道的边界
     *
     * @param     boundaryId             车道id
     * @param     Boundary               边界
     * @param     BoundaryPoints         边界点，非空表示需要获取左侧边界点
     * @param     LineType               车道边线的线型
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK                获取成功
     * @retval    CARFIELD_MAP_FAIL              获取失败
     */
    int32_t GetBoundary(const hdmap::MapId &boundaryId, hdmap::BoundaryPtr &boundary,
        std::vector<hdmap::Point> &boundaryPoints, hdmap::LineType &lineType);

    /* *
     * @brief    根据车道获取车道左右边界feature
     *
     * @param     lane       车道feature
     * @param     lBoundaryPtr         左侧车道边界feature
     * @param     rBoundaryPtr         右侧车道边界feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLaneBoundaries(const hdmap::LanePtr &lane, hdmap::BoundaryPtr &lBoundaryPtr,
        hdmap::BoundaryPtr &rBoundaryPtr);

    /* *
     * @brief     获取车道的左右边界点
     *
     * @param     laneId                 车道id
     * @param     leftBoundaryPoints     左侧边界点，非空表示需要获取左侧边界点
     * @param     rightBoundaryPoints    右侧边界点，非空表示需要获取右侧边界点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK                获取成功
     * @retval    CARFIELD_MAP_FAIL              获取失败
     */
    int32_t GetBoundaryPoints(const hdmap::MapId &laneId, std::vector<hdmap::Point> &leftBoundaryPoints,
        std::vector<hdmap::Point> &rightBoundaryPoints);

    /* *
     * @brief     获取车道左右边线的线型
     *
     * @param     laneId           车道id
     * @param     leftLineType     左侧车道边线的线型
     * @param     rightLineType    右侧车道边线的线型
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetBoundaryLineType(const hdmap::MapId &laneId, hdmap::LineType &leftLineType,
        hdmap::LineType &rightLineType);

    /* *
     * @brief     根据指定位置查找所在车道
     * @note      如果指定位置在某个车道内，返回该车道ID，否则返回失败
     * @note      如果指定位置有多个车道（例如高架上下），返回所有车道ID
     *
     * @param     location     指定位置
     * @param     laneIds      车道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      查找成功
     * @retval    CARFIELD_MAP_FAIL    查找失败
     */
    int32_t GetLocatedLane(const hdmap::Point &location, std::vector<hdmap::MapId> &laneIds);

    // ##################################################################################
    // ************************************ 其他Feature接口 *******************************
    // ##################################################################################
    /* *
     * @brief     获取车道关联的交通灯
     *
     * @param     laneId           车道ID
     * @param     trafficLights    关联的交通灯
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetLaneRelatedTrafficLights(const hdmap::MapId &laneId, std::vector<hdmap::TrafficLightPtr> &trafficLights);

    /* *
     * @brief     获取车道关联的交通标志
     * @note      map_node中TrafficSignType与map对应关系为：
     * map_node                                              map
     * TrafficSignType::UNKONWN                              FEATURE_TYPE_TRAFFIC_SIGN::SIGN_NONE
     * TrafficSignType::TRAFFICSIGN_TYPE_WAITINGAREA FEATURE_TYPE_TRAFFIC_SIGN::SIGN_TRAFFIC_STRAIGHT_WAITING_AREA
     * TrafficSignType::TRAFFICSIGN_TYPE_STOP                FEATURE_TYPE_STOP_SIGN
     * TrafficSignType::TRAFFICSIGN_CROSSBAR                 FEATURE_TYPE_CROSS_BAR
     *
     * @param     laneId          车道ID
     * @param     trafficSigns    关联的交通标志
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK         获取成功
     * @retval    CARFIELD_MAP_FAIL       获取失败
     */
    int32_t GetLaneRelatedTrafficSigns(const hdmap::MapId &laneId, std::vector<hdmap::TrafficSignPtr> &trafficSigns);

    /* *
     * @brief     根据roadid获取获取前方有停止线的lane集合,用于前方红绿灯的仿真构造
     *
     * @param     curRoadId                 当前道路ID
     * @return    std::vector<hdmap::MapId> laneid集合
     */
    std::vector<hdmap::MapId> GetNextIntersectionRoads(hdmap::MapId curRoadId);

    /* *
     * @brief     从路段内挨着停止线的lane出发获取路口的随机lane序列
     *
     * @param     randomdata              随机数
     * @param     laneIdNexttoStopline    路段内挨着停止线的lane
     */
    std::deque<hdmap::MapId> GetRandomLanesRouteFromStopline(int randomdata, const hdmap::MapId &laneIdNexttoStopline);

    /* *
     * @brief     从当前车道出发获取一条到停止线的随机lane序列
     *
     * @param     randomSeed              随机种子
     * @param     beginLaneId             起始车道laneId
     * @param     maxDepth                最大索引深度
     */
    std::deque<hdmap::MapId> GetRandomLanesRoutetoStopline(int randomSeed, const hdmap::MapId &beginLaneId,
        uint32_t maxDepth);

    /* *
     * @brief     判断车道是否关联停止线
     *
     * @param     laneId       车道ID
     * @return    bool
     * @retval    true      关联停止线
     * @retval    false     不关联停止线
     */
    bool IsLaneRelatedStopLines(const hdmap::MapId &laneId);

    /* *
     * @brief     获取车道关联的停止线
     *
     * @param     laneId       车道ID
     * @param     stopLines    关联的停止线
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetLaneRelatedStopLines(const hdmap::MapId &laneId, std::vector<hdmap::StopLinePtr> &stopLines);

    /* *
     * @brief    根据ID获取人行横道feature
     *
     * @param     crosswalkId       人行横道ID
     * @param     crosswalk         人行横道feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetCrosswalk(const hdmap::MapId &crosswalkId, hdmap::CrosswalkPtr &crosswalk);

    /* *
     * @brief    获取crosswalk geometry点
     *
     * @param     crosswalk          crosswalk_feature
     * @param     crosswalkPoints    crosswalk  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetCrosswalkPoints(const hdmap::CrosswalkPtr &crosswalk, std::vector<hdmap::Point> &crosswalkPoints);

    /* *
     * @brief    获取roadmarking  geometry点
     *
     * @param     roadmarking          roadmarking_feature
     * @param     roadmarkingPoints    roadmarking  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetRoadmarkingPoints(const hdmap::RoadMarkingPtr &roadmarking,
        std::vector<hdmap::Point> &roadmarkingPoints);

    /* *
     * @brief    获取Pole  geometry点
     *
     * @param     pole          pole_feature
     * @param     polePoints    pole  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetPolePoints(const hdmap::PolePtr &pole, std::vector<hdmap::Point> &polePoints);

    /* *
     * @brief    获取obstacle  geometry点
     *
     * @param     obstacle             obstacle feature
     * @param     obstaclePoints       obstacle  geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetObstaclePoints(const hdmap::ObstaclePtr &obstacle, std::vector<hdmap::Point> &obstaclePoints);

    /* *
     * @brief     获取交通灯
     *
     * @param     trafficLightId    交通灯ID
     * @param     trafficLight      交通灯
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK           获取成功
     * @retval    CARFIELD_MAP_FAIL         获取失败
     */
    int32_t GetTrafficLight(const hdmap::MapId &trafficLightId, hdmap::TrafficLightPtr &trafficLight);

    /* *
     * @brief    获取TrafficLight geometry点
     *
     * @param     trafficLight          trafficLight_feature
     * @param     trafficLightPoints    trafficLight  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetTrafficLightPoints(const hdmap::TrafficLightPtr &trafficLight,
        std::vector<hdmap::Point> &trafficLightPoints);

    /* *
     * @brief     获取交通灯类型
     *
     * @param     trafficLightId    交通灯ID
     * @param     trafficLightType  交通灯类型
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK           获取成功
     * @retval    CARFIELD_MAP_FAIL         获取失败
     */
    int32_t GetTrafficLightType(const hdmap::MapId &trafficLightId, hdmap::TrafficLightType &trafficLightType);

    /* *
     * @brief     获取交通标志
     *
     * @param     trafficSignId     交通标志ID
     * @param     trafficSign       交通标志
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK           获取成功
     * @retval    CARFIELD_MAP_FAIL         获取失败
     */
    int32_t GetTrafficSign(const hdmap::MapId &trafficSignId, hdmap::TrafficSignPtr &trafficSign);

    /* *
     * @brief    获取TrafficSign  geometry点
     *
     * @param     trafficSign          trafficSign_feature
     * @param     trafficSignPoints    trafficSign  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetTrafficSignPoints(const hdmap::TrafficSignPtr &trafficSign,
        std::vector<hdmap::Point> &trafficSignPoints);

    /* *
     * @brief     获取停止线
     *
     * @param     stopLineId        停止线ID
     * @param     stopLine          停止线
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK           获取成功
     * @retval    CARFIELD_MAP_FAIL         获取失败
     */
    int32_t GetStopLine(const hdmap::MapId &stopLineId, hdmap::StopLinePtr &stopLine);

    /* *
     * @brief    获取StopLine  geometry点
     *
     * @param     stopLine          stopLine_feature
     * @param     stopLinePoints    stopLine  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetStopLinePoints(const hdmap::StopLinePtr &stopLine, std::vector<hdmap::Point> &stopLinePoints);

    /* *
     * @brief     获取停车位
     *
     * @param     parkingSpotId    停车位ID
     * @param     parkingSpot      停车位
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetParkingSpot(const hdmap::MapId &parkingSpotId, hdmap::ParkingSpotPtr &parkingSpot);

    /* *
     * @brief    获取ParkingSpot  geometry points
     *
     * @param     parkingSpot             parkingSpot feature
     * @param     parkingSpotPoints       parkingSpot  geometry points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetParkingSpotPoints(const hdmap::ParkingSpotPtr &parkingSpot,
        std::vector<hdmap::Point> &parkingSpotPoints);

    /* *
     * @brief     获取停车场
     *
     * @param     parkingAreaId    停车场ID
     * @param     parkingArea      停车场
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK          获取成功
     * @retval    CARFIELD_MAP_FAIL        获取失败
     */
    int32_t GetParkingArea(const hdmap::MapId &parkingAreaId, hdmap::ParkingAreaPtr &parkingArea);

    /* *
     * @brief    获取Parking_area  geometry points
     *
     * @param     parkingArea             parkingArea feature
     * @param     parkingAreaPoints       parkingArea  geometry points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetParkingAreaPoints(const hdmap::ParkingAreaPtr &parkingArea,
        std::vector<hdmap::Point> &parkingAreaPoints);

    /* *
     * @brief     根据位置获取停车位ID
     * @note      如果指定位置的停车位有多个，返回所有停车位ID
     *
     * @param     location          位置
     * @param     parkingSpotIds    停车位ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK           获取成功
     * @retval    CARFIELD_MAP_FAIL         获取失败
     */
    int32_t GetLocatedParkingSpotId(const hdmap::Point &location, std::vector<hdmap::MapId> &parkingSpotIds);

    /* *
     * @brief     判断指定位置是否在路口
     *
     * @param     location     位置
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      在路口
     * @retval    CARFIELD_MAP_FAIL    不在路口
     */
    int32_t IsLocatedInIntersection(const hdmap::Point &location);

    /* *
     * @brief     根据指定位置获取路口ID
     * @note      如果指定位置的路口有多个，返回所有路口ID
     *
     * @param     location           位置
     * @param     intersectionIds    路口ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK            获取成功
     * @retval    CARFIELD_MAP_FAIL          获取失败
     */
    int32_t GetLocatedIntersectionId(const hdmap::Point &location, std::vector<hdmap::MapId> &intersectionIds);

    /* *
     * @brief     根据路口车道id获取路口id
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      查找成功
     * @retval    CARFIELD_MAP_FAIL    查找失败
     */
    int32_t GetIntersectionId(const hdmap::MapId &intersectionLaneId, hdmap::MapId &intersectionId);

    /* *
     * @brief    获取intersection  geometry points
     *
     * @param     intersection          intersection_feature
     * @param     intersectionPoints    intersection  geometry Points
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetIntersectionPoints(const hdmap::IntersectionPtr &intersection,
        std::vector<hdmap::Point> &intersectionPoints);

    /* *
     * @brief     判断指定位置是否在人行横道
     *
     * @param     location     位置
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      在
     * @retval    CARFIELD_MAP_FAIL    不在
     */
    int32_t IsLocatedInCrosswalk(const hdmap::Point &location);

    /* *
     * @brief     根据指定位置获取人行横道ID
     * @note      如果指定位置的人行横道有多个，返回所有人行横道ID
     *
     * @param     location        位置
     * @param     crosswalkIds    人行横道ID
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK         获取成功
     * @retval    CARFIELD_MAP_FAIL       获取失败
     */
    int32_t GetLocatedCrosswalkId(const hdmap::Point &location, std::vector<hdmap::MapId> &crosswalkIds);

    /* *
     * @brief    根据轮档ID获取轮档feature
     *
     * @param     wheelStopperId       道路ID
     * @param     wheelStopper         道路feature
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      获取成功
     * @retval    CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetWheelStopper(const hdmap::MapId &wheelStopperId, hdmap::WheelStopperPtr &wheelStopper);

    /* *
     * @brief    获取轮档 geometry点
     *
     * @param     wheelStopper          轮档
     * @param     wheelStopperPoints    轮档 geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetWheelStopperPoints(const hdmap::WheelStopperPtr &wheelStopper,
        std::vector<hdmap::Point> &wheelStopperPoints);

    /* *
     * @brief    根据停车位获取其轮档geometry点
     *
     * @param     parkingSpot           停车位
     * @param     wheelStopperPoints    轮档 geometry点
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK       获取成功
     * @retval    CARFIELD_MAP_FAIL     获取失败
     */
    int32_t GetParkingSpotWheelStopperPolygon(const hdmap::ParkingSpotPtr &parkingSpot,
        std::vector<hdmap::Point> &wheelStopperPolygon);

    // ##################################################################################
    // ************************************ 计算类接口 ************************************
    // ##################################################################################
    /* *
     * @brief    计算两点之间的距离
     *
     * @param     p1        点1
     * @param     p2        点2
     * @return    double    两点之间的距离
     */
    double CalcDistance(const hdmap::Point &p1, const hdmap::Point &p2);

    /* *
     * @brief    计算一系列点之间的距离之和
     *
     * @param    points          一系列点
     */
    double LineStringLength(const std::vector<hdmap::Point> &points);

    /* *
     * @brief    计算两点之间距离的平方
     *
     * @param     p1        点1
     * @param     p2        点2
     * @return    double    两点之间的距离的平方
     */
    double CalcDistanceSquare(const hdmap::Point &p1, const hdmap::Point &p2);

    /* *
     * @brief    计算两点方向
     *
     * @param     start     开始点
     * @param     end       结束点
     * @return    double    两点方向
     */
    double CalcPointsDirection(const hdmap::Point &start, const hdmap::Point &end);

    /* *
     * @brief    计算指定点到线段的投影系数
     *
     * @param    point           指定的点
     * @param    start_point     线段起点
     * @param    end_point       线段终点
     * @return   double          计算得的投影系数
     */
    double ProjectionFactor(const hdmap::Point &point, const hdmap::Point &start_point, const hdmap::Point &end_point);

    /* *
     * @brief    计算指定点到线段的投影点
     *
     * @param    point           指定的点
     * @param    start_point     线段起点
     * @param    end_point       线段终点
     * @return   hdmap::Point    计算得的投影点
     */
    void Project(const hdmap::Point &point, const hdmap::Point &start_point, const hdmap::Point &end_point,
        hdmap::Point &project_point);

    /* *
     * @brief    计算指定点到线段上的投影点(投影点在线段延长线上，则到线段最近的点要么为线段起点，要么为线段终点)
     *
     * @param    point           指定的点
     * @param    start_point     线段起点
     * @param    end_point       线段终点
     * @return   closest_point   计算得的最近点
     * @return   project_factor  到最近点的投影系数
     */
    void ClosestPoint(const hdmap::Point &point, const hdmap::Point &start_point, const hdmap::Point &end_point,
        hdmap::Point &closest_point, double &project_factor);

    /* *
     * @brief    计算指定点到线段的距离
     *
     * @param    point           指定的点
     * @param    start_point     线段起点
     * @param    end_point       线段终点
     * @return   double          最近距离
     */
    double DistanceToLineSegment(const hdmap::Point &point, const hdmap::Point &start_point,
        const hdmap::Point &end_point);

    double DistanceToLineSegment(const hdmap::Point &point, const hdmap::Point &start_point,
        const hdmap::Point &end_point, double &project_factor);

    /* *
     * @brief    计算指定点到直线的距离
     *
     * @param    point           指定的点
     * @param    l1,l2           直线两点
     * @return   double          距离
     */
    double DistanceToLine(const hdmap::Point &p0, const hdmap::Point &l1, const hdmap::Point &l2);

    /* *
     * @brief    计算点到折线(一系列相连线段)的最短距离
     *
     * @param     point     点
     * @param     points    折线
     * @return    double    点到折线的最短距离
     */
    double DistanceToLineString(const hdmap::Point &point, const std::vector<hdmap::Point> &points);

    double DistanceToLineString(const hdmap::Point &point, const std::vector<hdmap::Point> &points,
        int &nearest_point_index, double &project_factor);

    /* *
     * @brief    判断点是否在多边形内
     * @note     要求多边形是闭合的
     *
     * @param     point       点
     * @param     polygon     多边形
     * @return    bool
     * @retval    true        是
     * @retval    false       否
     */
    bool IsPointInPolygon(const hdmap::Point &point, const std::vector<hdmap::Point> &polygon);

    /* *
     * @brief    计算两点之间的沿路距离(分别找到距离两点最近的路点，计算两个路点之间的沿路距离)
     *
     * @param     points       道路点列
     * @param     location1    位置1
     * @param     location2    位置2
     * @param     distance     两点之间的沿路距离
     * @return    int
     * @retval    CARFIELD_MAP_OK      计算成功
     * @retval    CARFIELD_MAP_FAIL    计算失败
     */
    int32_t CalcDistanceBetweenTwoPointInLane(const std::vector<hdmap::Point> &points, const hdmap::Point &location1,
        const hdmap::Point &location2, double &distance);

    /* *
     * @brief     求点列中距离给定点最近的点
     *
     * @param     inPoint     点
     * @param     points      点列
     * @param     outPoint    找到的最近点
     * @retval    CARFIELD_MAP_OK      计算成功
     * @retval    CARFIELD_MAP_FAIL    计算失败
     */
    int32_t GetNearestPoint(const hdmap::Point &inPoint, const std::vector<hdmap::Point> &points,
        hdmap::Point &outPoint);

    /* *
     * @brief     求点列中距离给定点最近的点在点列中的序号
     *
     * @param     point      点
     * @param     points     点列
     * @return    int32_t    序号
     */
    int32_t GetNearestPointIndex(const hdmap::Point &point, const std::vector<hdmap::Point> &points);

    /* *
     * @brief     求点列中距离给定点最近的点在点列中的序号(参考上一次位置，提高查询效率)
     *
     * @param     lastLocation    上一次位置
     * @param     lastIndex       点列中距离上一次位置最近的点在点列中的序号
     * @param     curLocation     当前位置
     * @param     points          点列
     * @return    int32_t         点列中距离当前位置最近的点在点列中的序号,-1为异常值
     */
    int32_t GetNearestPointIndex(const hdmap::Point &lastLocation, const int &lastIndex,
        const hdmap::Point &curLocation, const std::vector<hdmap::Point> &points);

    /* *
     * @brief     根据输入点列和插值间隔进行插值得到输出点列
     *
     * @param     inPoints     输入点列
     * @param     outPoints    输出点列
     * @param     inteval      插值间隔
     * @return    int
     * @retval    CARFIELD_MAP_OK      执行成功
     * @retval    CARFIELD_MAP_FAIL    执行失败
     */
    int32_t LinearInterpolation(const std::vector<hdmap::Point> &inPoints, std::vector<hdmap::Point> &outPoints,
        double inteval);

    /* *
     * @brief     差值后降采样
     *
     * @param     inPoints     输入点列
     * @param     outPoints    输出点列
     * @param     windowLength 采样间隔
     * @return    bool
     * @retval    CARFIELD_MAP_OK      执行成功
     * @retval    CARFIELD_MAP_FAIL    执行失败
     */
    int32_t SlidingWindowFilter(const std::vector<hdmap::Point> &inPoints, std::vector<hdmap::Point> &outPoints,
        const double &windowLength);

    /* *
     * @brief     计算曲率
     *
     * @param     points       输入点列
     * @param     distance     检查距离间隔
     * @param     angle　　　　　好像没有用？？？
     * @return    double       曲率最大值
     */
    double CalculateCurvature(const std::vector<hdmap::Point> &points, double distance, double angle);

    /* *
     * @brief     计算三个点对应曲线的曲率
     *
     * @param     point1/point2/point3       输入三个点
     * @return    double       曲率
     */
    double Cal3PointCurvature(const hdmap::Point &point1, const hdmap::Point &point2, const hdmap::Point &point3);

    /* *
     * @brief     计算最大曲率
     *
     * @param     pointList       输入点列
     * @param     index        最大值位置
     * @return    double       曲率最大值
     */
    double GetMaxCurvature(const std::vector<hdmap::Point> &pointList);
    double GetMaxCurvature(const std::vector<hdmap::Point> &pointList, int &index);

    /* *
     * @brief     计算一系列lane中的最大曲率及最大位置
     *
     * @param     pointList       输入点列
     * @param     maxLaneIndex    最大曲率lane的序号
     * @param     maxPointIndex   最大曲率Point的序号
     * @param     length          最大曲率Point到lanesRoutes中的起始lane的起始点的道路长度
     * @return    double          曲率最大值
     */
    double GetLanesRouteMaxCurvature(const std::deque<hdmap::MapId> &lanesRoutes, int &maxLaneIndex, int &maxPointIndex,
        double &length);
    /* *
     * @brief     根据车道中心点和一侧边界点，镜像得到另一侧边界点
     *
     * @param     inPoints        一侧边界点
     * @param     centerPoints    车道中心点
     * @param     outPoints       另一侧边界点
     * @return    void
     */
    void MirrorLanePoints(const std::vector<hdmap::Point> &inPoints, const std::vector<hdmap::Point> &centerPoints,
        std::vector<hdmap::Point> &outPoints);

    /* *
     * @brief     计算某一个点到另外两个点连成的直线的的垂直距离
     *
     * @param     pt            要计算的点
     * @param     lineStart     直线起点
     * @param     lineEnd       直线终点
     * @return    double        返回的垂直距离
     */
    double PerpendicularDistance(const hdmap::Point &pt, const hdmap::Point &lineStart, const hdmap::Point &lineEnd);

    /* *
     * @brief
     * 对多边形集合点进行精简，得到更稀疏的点，并保持形状变化不大，根据最大垂直距离阀值优化，对一段多边形集合不超过最大阀值就直接取头尾两个点，
     * 大于阀值就作为边界一分为二，进行递归操作
     *
     * @param     pointList     多边形集合点
     * @param     epsilon       最大阀值
     * @param     out           优化后的多边形集合点
     * @return    bool          true 成功, false 失败
     */
    bool RamerDouglasPeucker(const std::vector<hdmap::Point> &pointList, double epsilon,
        std::vector<hdmap::Point> &out);

    /* *
     * @brief     计算两组点集角度差
     *
     * @param     p1     第一组坐标点1
     * @param     p2     第一组坐标点2
     * @param     p3     第二组坐标点1
     * @param     p4     第二组坐标点2
     * @return    double   返回的角度差，单位弧度
     */
    double CalculateAngleOfTwoVector(const hdmap::Point &p1, const hdmap::Point &p2, const hdmap::Point &p3,
        const hdmap::Point &p4);

    /* *
     * @brief     判断坐标点是否在集合点的内部，而非延长线上
     *
     * @param     point    坐标点
     * @param     points   集合点
     * @return    bool    true:在，false:不在
     */
    bool IsProjectedOnLineString(const hdmap::Point &point, const std::vector<hdmap::Point> &points);

    /* *
     * @brief 判断指定点是否在线段的左侧
     *
     * @param    p1    线段起点
     * @param    p2    线段终点
     * @param    p     指定的点
     * @return   bool  计算结果
     * @retval   true  点在线段的左侧
     * @retval   false 点在线段的右侧
     */
    bool IsOnLeftSideOfLineSegment(const hdmap::Point &p1, const hdmap::Point &p2, const hdmap::Point &p);

    /*
     * @brief 判断指定点是否在折线的左侧
     *
     * @param    point  指定的点
     * @param    points 折线包含的点集
     * @return   bool   计算结果
     * @retval   true   点在折线的左侧
     * @retval   false  点在折线的右侧
     */
    bool IsOnLeftSideOfLineString(const hdmap::Point &point, const std::vector<hdmap::Point> &points);

    /*
     * @brief 判断两条线段是否相交
     *
     * @param    p1  线段一点1坐标
     * @param    p2  线段一点2坐标
     * @param    l1  线段二点1坐标
     * @param    l2  线段二点2坐标
     * @return   bool   判断结果
     * @retval   true   相交
     * @retval   false  不相交
     */
    bool SegmentIntersectWithLine(const hdmap::Point &p1, const hdmap::Point &p2, const hdmap::Point &l1,
        const hdmap::Point &l2);

    /*
     * @brief 获取范数坐标点，即以线段p1点为原点，将线段p2点按顺时针旋转90度得到新的p2点
     *
     * @param    p1  线段点1坐标
     * @param    p2  线段点2坐标
     * @param    n   返回的范数坐标点
     * @return   int32_t
     * @retval   CARFIELD_MAP_OK      获取成功
     * @retval   CARFIELD_MAP_FAIL    获取失败
     */
    int32_t GetNormPoint(const hdmap::Point &p1, const hdmap::Point &p2, hdmap::Point &n);

    /*
     * @brief 获取两条直线交叉点，如果没有交叉则返回CARFIELD_MAP_FAIL
     *
     * @param    p1  直线一点1坐标
     * @param    p2  直线一点2坐标
     * @param    l1  直线二点1坐标
     * @param    l2  直线二点2坐标
     * @param    intersection  返回的交叉点坐标
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      直线交叉
     * @retval    CARFIELD_MAP_FAIL    直线不交叉
     */
    int32_t GetLineIntersection(const hdmap::Point &p1, const hdmap::Point &p2, const hdmap::Point &l1,
        const hdmap::Point &l2, hdmap::Point &intersection);

    /* *
     * @brief     判断坐标p2和p1,p3的弧度差是否大于阀值angle，也就是判断是否出现很大拐点
     *
     * @param     p1     坐标点1
     * @param     p2     坐标点2
     * @param     p3     坐标点3
     * @param     angle  弧度阀值
     * @return    bool   true or false
     */
    bool IsInflectionPoint(const hdmap::Point &p1, const hdmap::Point &p2, const hdmap::Point &p3, const double &angle);

    /* *
     * @brief     计算累计角度差
     *
     * @param     points   坐标点集
     * @return    double   累计角度差
     */
    double CalculateCumulateAngleOfPoints(const std::vector<hdmap::Point> &points);

    /* *
     * @brief     计算两组坐标的朝向
     *
     * @param     p1     第一组坐标点1
     * @param     p2     第一组坐标点2
     * @param     p3     第二组坐标点1
     * @param     p4     第二组坐标点2
     * @return    RelativeDirection   返回枚举方向值
     */
    RelativeDirection JudgeRelativeDirectionFromVec1ToVec2(const hdmap::Point &p1, const hdmap::Point &p2,
        const hdmap::Point &p3, const hdmap::Point &p4);
    /* *
     * @brief     生成二阶贝塞尔曲线集
     *
     * @param     p1         坐标点1
     * @param     p2         坐标点2
     * @param     p3         坐标点3
     * @param     pointDis   最小距离阀值
     * @param     wayPoints  输出贝塞尔曲线集
     * @return    void
     */
    void SecondOrderBezierCurve(const hdmap::Point &p0, const hdmap::Point &p1, const hdmap::Point &p2,
        const double &pointDis, std::vector<hdmap::Point> &wayPoints);

    /* *
     * @brief     生成三阶贝塞尔曲线集
     *
     * @param     p1         坐标点1
     * @param     p2         坐标点2
     * @param     p3         坐标点3
     * @param     pointDis   最小距离阀值
     * @param     wayPoints  输出贝塞尔曲线集
     * @return    void
     */
    void ThirdOrderBezierCurve(const hdmap::Point &p0, const hdmap::Point &p1, const hdmap::Point &p2,
        const hdmap::Point &p3, const double &pointDis, std::vector<hdmap::Point> &wayPoints);

    // todo
    void GeneratePointsByDir(const hdmap::Point &p0, const hdmap::Point &dir, const int &num, const double &pointDis,
        std::vector<hdmap::Point> &points);

    // todo
    int32_t PointsMatch(std::unordered_map<int, std::vector<hdmap::Point>> pointsMap1,
        std::unordered_map<int, std::vector<hdmap::Point>> pointsMap2, std::unordered_map<int, int> &MatchMap,
        double lateralDistance);

    /* *
     * @brief    查找指定元素在vector容器中的索引位置
     *
     * @param     data          指定元素
     * @param     dataVector    vector容器数据集
     * @param     index         要返回的索引值
     * @return    int32_t
     * @retval    CARFIELD_MAP_OK      已找到
     * @retval    CARFIELD_MAP_FAIL    未找到
     */
    int32_t FindVectorIndex(uint32_t data, std::vector<uint32_t> &dataVector, int64_t &index);

    /* *
     * @brief    根据指定比例获取两点连成的线段上的对应的点坐标
     *
     * @param     rate1/rate2   指定比例:rate1从pt1开始朝向pt2，rate2从pt2开始朝向pt1
     * @param     pt1           线段坐标点1
     * @param     pt2           线段坐标点2
     * @param     pt            计算出新的坐标
     * @return    void
     */
    hdmap::Point GetRate2Point(double rate2, const hdmap::Point &pt1, const hdmap::Point &pt2);
    hdmap::Point GetRate1Point(double rate1, const hdmap::Point &pt1, const hdmap::Point &pt2);

    // ##################################################################################
    // **************************** 地图数据坐标转换接口 **********************************
    // ##################################################################################
    /* *
     * @brief  根据utm带号, 更新lla和utm的坐标转换关系
     * @note  内部地图接口所有涉及坐标转换的函数都会受到影响
     */
    void UpdateCoordinateTransformation(int utmZone);

    /* *
     * @brief  进行lla<->utm的坐标转换
     */
    int32_t CoordinateTransform(const hdmap::Point &input, hdmap::Point &output, CoordinateTransformation tranform);
    int32_t CoordinateTransform(double inputx, double inputy, double &outputx, double &outputy,
        CoordinateTransformation tranform);
    int32_t CoordinateTransform(const std::vector<hdmap::Point> &inputs, std::vector<hdmap::Point> &outputs,
        CoordinateTransformation tranform);

private:
    std::atomic<CarfieldMapApiImpl*> impl_;
    CarfieldMapApiImpl* protectedApiCache_;
    std::mutex initLock_;
};
}

#endif // CARFIELD_MAP_API_H

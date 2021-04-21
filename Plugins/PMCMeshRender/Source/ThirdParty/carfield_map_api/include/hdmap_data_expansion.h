/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Map data expansion define.
 * Author: c00525315
 * Create: 2020-07-04
 */

#ifndef INCLUDE_HDMAP_HDMAP_DATA_EXPANSION_H
#define INCLUDE_HDMAP_HDMAP_DATA_EXPANSION_H

#include <cstdint>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

namespace hdmap {

    template<class T>
    class GeoPoint {
    public:
        const static constexpr double FLOAT_ZERO = 1e-6;

        GeoPoint() : x(T()), y(T()), z(T()) {}
        GeoPoint(const T xx, const T yy, const T zz = T()) : x(xx), y(yy), z(zz) {}

        ~GeoPoint() {}

        GeoPoint &operator += (const GeoPoint &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        bool operator==(const GeoPoint &rhs) const
        {
            if (&rhs == this) {
                return true;
            }
            return ((std::abs(x - rhs.x) < FLOAT_ZERO) && (std::abs(y - rhs.y) < FLOAT_ZERO) &&
                    (std::abs(z - rhs.z) < FLOAT_ZERO));
        }

        bool operator!=(const GeoPoint &rhs) const
        {
            return !(*this == rhs);
        }

        GeoPoint operator+(const GeoPoint &other) const
        {
            return GeoPoint(this->x + other.x, this->y + other.y, this->z + other.z);
        }

        GeoPoint operator-(const GeoPoint &other) const
        {
            return GeoPoint(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        T x;
        T y;
        T z;
    };

    class HdMapClient;
    using Point = GeoPoint<double>;

    using MapId = uint64_t;

    /**
     * hdmap struct
     */
    struct FeatureAttr;
    using MapFeaturePtr = std::shared_ptr<FeatureAttr>;

    /**
     * Error Code
     */
    enum ErrorCode : int32_t {
        MAP_FAIL = -1,
        MAP_OK = 0,
        // add error code here
        MAP_NO_INIT = 1,      // 未正确初始化
        MAP_NO_MAP = 2,       // 未找到合适的地图
        MAP_UNSUPPORTED = 3,  // 不支持的场景
        MAP_NO_RESULT = 4,    // 查询结果为空
        MAP_CALLEXT_FAIL = 5, // 调用外部接口失败
        MAP_ERRCODE_BUTT = 6,
    };

    /**
     * Feature Type
     */
    enum class FeatureType : uint8_t {
        FEATURE_TYPE_ROAD = 0,          // 道路
        FEATURE_TYPE_LANE,              // 车道
        FEATURE_TYPE_BOUNDARY,          // 边线
        FEATURE_TYPE_INTERSECTION,      // 路口
        FEATURE_TYPE_STOPLINE,          // 停止线
        FEATURE_TYPE_CROSSWALK,         // 人行道
        FEATURE_TYPE_ROADMARKING,       // 各种地面指示，如减速带,黄色网格线等（地面箭头等信息放到了车道里面）
        FEATURE_TYPE_TRAFFICLIGHT,      // 交通灯
        FEATURE_TYPE_TRAFFICSIGN,       // 各种交通指示，如停止标志，减速标志，行人标志，危险标志等
        FEATURE_TYPE_POLE,              // 修改：各种杆；（各种杆，龙门架，障碍物等）
        FEATURE_TYPE_PARKINGAREA,       // 新增：停车场
        FEATURE_TYPE_PARKINGSPOT,       // 新增：停车位
        FEATURE_TYPE_WHEELSTOPPER,      // 停车位轮挡
        FEATURE_TYPE_OBSTACLE,          // 新增：路口障碍物
        FEATURE_TYPE_TOLLAREA,          // 收费站
        FEATURE_TYPE_SDROAD,            // SD道路
        FEATURE_TYPE_BUTT,
    };

    /**
     * Feature的几何类型
     */
    enum class FeatureGeometryType : uint8_t {
        GEOMETRY_TYPE_POINT = 0,
        GEOMETRY_TYPE_POLYLINE,
        GEOMETRY_TYPE_POLYGON,
        GEOMETRY_TYPE_BUTT,
    };

    /**
     * Boundary物理隔离类型定义
     */
    enum class BoundaryIsolationType : uint8_t {
        BOUNDARY_ISOLATION_TYPE_INVALID = 0, 	    // 无效
        BOUNDARY_ISOLATION_TYPE_NOMARKING,          // 无物理隔离
        BOUNDARY_ISOLATION_TYPE_GUARDRAIL,          // 护栏
        BOUNDARY_ISOLATION_TYPE_CURB,               // 路牙
        BOUNDARY_ISOLATION_TYPE_WALL,               // 墙面
        BOUNDARY_ISOLATION_TYPE_BARRIER,            // 隔离墩
        BOUNDARY_ISOLATION_TYPE_CLIFF,              // 悬崖
        BOUNDARY_ISOLATION_TYPE_DITCH,              // 沟渠
        BOUNDARY_ISOLATION_BUTT,
    };

    /**
     * Boundary线型LineType定义
     */
    enum class LineType : uint8_t {
        LINE_TYPE_INVALID = 0,				        // 无效
        LINE_TYPE_VIRTUALLINE,                      // 虚拟标线
        LINE_TYPE_SINGLE_WHITE_DASHEDLINE,          // 单白虚线
        LINE_TYPE_SINGLE_WHITE_SOLIDLINE,           // 单白实线
        LINE_TYPE_SINGLE_YELLOW_DASHDLINE,          // 单黄虚线
        LINE_TYPE_SINGLE_YELLOW_SOLIDLINE,          // 单黄实现
        LINE_TYPE_DOUBLE_WHITE_DASHEDLINE,          // 双白虚线
        LINE_TYPE_DOUBLE_WHITE_SOLIDLINE,           // 双白实线
        LINE_TYPE_DOUBLE_YELLOW_SOLIDLINE,          // 双黄实线
        LINE_TYPE_DOUBLE_YELLOW_DASHEDLINE,         // 双黄虚线
        LINE_TYPE_WHITE_SOLIDLINE_DASHEDLINE,       // 左白实线右白虚线
        LINE_TYPE_WHITE_DASHEDLINE_SOLIDLINE,       // 左白虚线右白实线
        LINE_TYPE_YELLOW_SOLIDLINE_DASHEDLINE,      // 左黄实线右黄虚线
        LINE_TYPE_YELLOW_DASHEDLINE_SOLIDLINE,      // 左黄虚线右黄实线
        LINE_TYEP_YELLOW_WHITE_SOLIDLINE,           // 左黄实线右白实线
        LINE_TYPE_BUTT,
    };

    /**
     * RoadClass 道路等级
     */
    enum class RoadClass : uint8_t {
        ROAD_CLASS_UNKNOWN = 0,     // 0：默认值，没有调查道路等级
        ROAD_CLASS_FREEWAY,         // 1：高速路、城市快速路（高速等级）
        ROAD_CLASS_NATIONAL_ROAD,   // 2：一级公路部分控制出入、部分立体交叉的公路，主要连接重要政治、经济中心，通往重点工矿区，是国家的干线公路。
        ROAD_CLASS_PROVINCE_ROAD,   // 3：二级公路连接政治、经济中心或大工矿区等地的干线公路，或运输繁忙的城郊公路。
        ROAD_CLASS_CITY_ROAD,       // 4：三级公路、城市次干道 沟通县及县以上城镇的一般干线公路。
        ROAD_CLASS_COUNTRY_ROAD,    // 5：四级公路、沟通县、乡、村等的支线公路
        ROAD_CLASS_BUTT,
    };

    /**
     * RoadType 道路类型定义
     * 目前与数据规格不一致, 接口反馈的类型包含数据规格的类型
     * 因此, 当前实现中, 数据有的类型接口正常返回, 数据不支持的类型数据会以为分类道路返回
     */
    enum class RoadType : uint8_t {
        ROAD_TYPE_INVALID = 0,
        ROAD_TYPE_OTHER, 	                // 未分类道路
        ROAD_TYPE_RAMP,                     // 匝道
        ROAD_TYPE_BRIDGE,                   // 桥梁. 从次开始及以下与规格不一致
        ROAD_TYPE_TUNNEL,                   // 隧道
        ROAD_TYPE_ROUND_CIRCLE,             // 环岛
        ROAD_TYPE_FERRY,                    // 轮渡
        ROAD_TYPE_SUBWAY,                   // 地下通道
        ROAD_TYPE_TOLL,                     // 收费站
        ROAD_TYPE_BUTT,
    };

    /**
     * SdRoadType SD道路类型定义
     * 道路类型不互斥,可按位或
     */
    enum class SdRoadType : uint64_t {
        ROAD_TYPE_INVALID                           = 0,
        ROAD_TYPE_OTHER                             = 0x0000000000000001,    // 未分类道路
        ROAD_TYPE_FERRY                             = 0x0000000000000002,    // 轮渡
        ROAD_TYPE_TUNNEL                            = 0x0000000000000004,    // 隧道
        ROAD_TYPE_BRIDGE                            = 0x0000000000000008,    // 桥梁
        ROAD_TYPE_TOLL                              = 0x0000000000000010,    // 收费站
        ROAD_TYPE_FREEWAY                           = 0x0000000000000020,    // 高速
        ROAD_TYPE_CITY_FREEWAY                      = 0x0000000000000040,    // 城市高速
        ROAD_TYPE_OVERPASS                          = 0x0000000000000080,    // 高架
        ROAD_TYPE_INTERSECTION_ROAD                 = 0x0000000000000100,    // 路口内部虚拟道路
        ROAD_TYPE_NO_THROUGH                        = 0x0000000000000200,    // 禁止穿行道路
        ROAD_TYPE_URBAN_ROAD                        = 0x0000000000000400,    // 市内道路
        ROAD_TYPE_RAMP                              = 0x0000000000000800,    // 匝道
        ROAD_TYPE_IC                                = 0x0000000000001000,    // IC。连接高速（包括城市高速）和普通道路的连接道。
        ROAD_TYPE_JCT                               = 0x0000000000002000,    // JCT。连接2个高速（包括城市高速）的连接道。
        ROAD_TYPE_ROUND_CIRCLE                      = 0x0000000000004000,    // 环岛
        ROAD_TYPE_SERVICE_ROAD                      = 0x0000000000008000,    // 服务区道路
        ROAD_TYPE_PARKING_ROAD                      = 0x0000000000010000,    // 停车区道路
        ROAD_TYPE_SIDE_ROAD                         = 0x0000000000020000,    // 辅路
        ROAD_TYPE_WALK_WAY                          = 0x0000000000040000,    // 步行街
        ROAD_TYPE_DIVIDED_ROAD                      = 0x0000000000080000,    // 是否上下线分离道路（中间有物理隔离带）
        ROAD_TYPE_FORBID                            = 0x0000000000100000,    // 存在禁转的道路
        ROAD_TYPE_ALL_DAY_FORBID                    = 0x0000000000200000,    // 路口内全天禁转道路
        ROAD_TYPE_AREA_ROAD                         = 0x0000000000400000,    // 区域内道路(包括小区,园区)
        ROAD_TYPE_ALL_CLOSED_ROAD                   = 0x0000000000800000,    // 是否是全封闭道路
        ROAD_TYPE_BUS_ROAD                          = 0x0000000001000000,    // 是否为公交车专用道
        ROAD_TYPE_EARLY_LEFT_TURN                   = 0x0000000002000000,    // 是否为提前左转道路
        ROAD_TYPE_EARLY_RIGHT_TURN                  = 0x0000000004000000,    // 是否为提前右转道路
        ROAD_TYPE_UTURN_ROAD                        = 0x0000000008000000,    // 是否为调头口
        ROAD_TYPE_MAIN_SIDE_ROAD_GATE               = 0x0000000010000000,    // 是否是主辅路出入口
        ROAD_TYPE_GUIDE_ROAD_OF_PARK                = 0x0000000020000000,    // 是否为停车位引导路
        ROAD_TYPE_VIRTUAL_CONNECT_ROAD              = 0x0000000040000000,    // 是否为虚拟链接路
        ROAD_TYPE_TURN_LEFT_BORROW_ROAD             = 0x0000000080000000,    // 是否是借道左转道路
        ROAD_TYPE_PARKING_GATE                      = 0x0000000100000000,    // 是否是停车场出入口道路
        ROAD_TYPE_OTHER_NO_ATTRIBUTE_ROAD           = 0x0000000200000000,    // 是否是其他无属性道路(如POI链接路,即园区出入口道路)
        ROAD_TYPE_BUTT                              = 0xFFFFFFFFFFFFFFFF,
    };

    /**
     * 车道类型LaneType定义
     */
    enum class LaneType : uint8_t {
        LANE_TYPE_INVALID = 0,          // 无效值
        LANE_TYPE_NORMAL,               // 常规车道
        LANE_TYPE_TURNERIGHT,           // 右转专用道
        LANE_TYPE_UTURN,                // 掉头专用道
        LANE_TYPE_WAITING_AREA,         // 待转区车道
        LANE_TYPE_SHOULDER,             // 路肩车道
        LANE_TYPE_PARKING,              // 路边停靠车道(包括港湾式停靠站车道和路边式停靠站车道)
        LANE_TYPE_TIDAL,                // 潮汐车道(如方向变化)
        LANE_TYPE_VARIABLE,             // 可变车道(如左转,直行变化)
        LANE_TYPE_ETC,                  // etc 车道
        LANE_TYPE_FILTER_NON_SHOULDER,  // 过滤选项, 规格无此类型. 除了非路肩车道及无效值
        LANE_TYPE_FILTER_NON_VAR_TID,   // 过滤选项, 规格无此类型. 除了可变车道,潮汐车道及无效值
        LANE_TYPE_FILTER_ALL,           // 过滤选项, 作为所有LaneType的选项
        LANE_TYPE_BUTT,
    };

    /**
    * VehicleType 车辆类型
    */
    enum class VehicleType : uint8_t {
        VEHICLE_TYPE_INVALID = 0,        // vehicle type not valid
        VEHICLE_TYPE_MOTORVEHICLE,       // 全部机动车
        VEHICLE_TYPE_SMALLCAR,           // 小汽车
        VEHICLE_TYPE_BUS,                // bus 公交车
        VEHICLE_TYPE_NOMOTORVEHICLE,     // 非机动车
        VEHICLE_TYPE_MOPTORNOMOTOR,      // 机非混合车
        VEHICLE_TYPE_TRUCK,              // truck 货车
        VEHICLE_TYPE_MINIBUS,            // 小客车
        VEHICLE_TYPE_COACH,              // 大客车
        VEHICLE_TYPE_FILTER_MOTOVEHICLE, // 过滤选项, 选取所有机动车(除了非机动车)
        VEHICLE_TYPE_FILTER_ALL,         // 过滤选项, 选取所有
        VEHICLE_TYPE_BUTT,
    };

    /**
    * LaneTurn 转向信息, 对应规格文档中的车道是否允许直行,左转,右转,掉头
    */
    enum class LaneTurn : uint8_t {
        LANE_TURN_INVALID    = 0,       // 无转向信息()
        LANE_TURN_LEFT       = 0x01,    // 左转(CanTurnLeft）
        LANE_TURN_STRAIGHT   = 0x02,    // 直行(CanStraight)
        LANE_TURN_RIGHT      = 0x04,    // 右转(CanTurnRight)
        LANE_TURN_UTURN      = 0x08,    // 掉头(CanUTurn)
        LANE_TURN_BUTT       = 0xFF,
    };

    /**
    * 路面标志要素，地图要素的子结构体
    */
    enum class RoadMarkingType : uint8_t {
        ROAD_MARKING_INVALID = 0,
        ROAD_MARKING_TXT,                       // 文字
        ROAD_MARKING_ARROW_STRAIGHT,            // 直行箭头
        ROAD_MARKING_ARROW_LEFT,                // 左转箭头
        ROAD_MARKING_ARROW_RIGHT,               // 右转箭头
        ROAD_MARKING_ARROW_UTURN,               // 掉头箭头
        ROAD_MARKING_ARROW_STRAIGHT_LEFT,       // 直行左转箭头
        ROAD_MARKING_ARROW_STRAIGHT_RIGHT,      // 直行右转箭头
        ROAD_MARKING_ARROW_LEFT_RIGHT,          // 左右转箭头
        ROAD_MARKING_ARROW_LEFT_STRAIGHT_RIGHT, // 左直右箭头
        ROAD_MARKING_ARROW_LEFT_UTURN,          // 左转掉头箭头
        ROAD_MARKING_ARROW_STRAIGHT_UTURN,      // 直行掉头箭头
        ROAD_MARKING_ARROW_NOUTURN,             // 禁止掉头
        ROAD_MARKING_DECELERATION_SIGN,         // 减速标识
        ROAD_MARKING_SPEED_BUMP,                // 减速带
        ROAD_MARKING_BUTT,
    };


    /**
    * TrafficLightType 红绿灯类型
    */
    enum class TrafficLightType : uint8_t {
        TRAFFICLIGHT_TYPE_INVALID = 0,              // 未知
        TRAFFICLIGHT_TYPE_CIRCLE,                   // 圆灯
        TRAFFICLIGHT_TYPE_ARROW_LEFT,               // 左转箭头灯
        TRAFFICLIGHT_TYPE_ARROW_RIGHT,              // 右转箭头灯
        TRAFFICLIGHT_TYPE_ARROW_FORWARD,            // 直行灯
        TRAFFICLIGHT_TYPE_ARROW_UTURN,              // 调头灯
        TRAFFICLIGHT_TYPE_WALK,                     // 行人指示灯
        TRAFFICLIGHT_TYPE_COUNTDOWN,                // 倒计时
        TRAFFICLIGHT_TYPE_LANE_SIGNAL,              // 车道信号
        TRAFFICLIGHT_TYPE_ARROW_LEFT_AND_FORWARD,   // 左转直行箭头灯
        TRAFFICLIGHT_TYPE_ARROW_RIGHT_AND_FORWARD,  // 右转直行箭头灯
        TRAFFICLIGHT_TYPE_BICYCLE,                  // 非机动车指示灯
        TRAFFICLIGHT_TYPE_FLASH_WARNING,            // 闪光警告
        TRAFFICLIGHT_TYPE_BUTT,
    };

    /**
     * TrafficSignType 交通标志类型
     */
    enum class TrafficSignType : uint8_t {
        TRAFFICSIGN_TYPE_INVALID = 0,           // 无效值
        TRAFFICSIGN_TYPE_UNKONOW,               // 未知标志
        TRAFFICSIGN_TYPE_STOP,                  // 停止标志
        TRAFFICSIGN_TYPE_YIELD,                 // 让行标志
        TRAFFICSIGN_TYPE_CROSSBAR,              // 停车横杆
        TRAFFICSIGN_TYPE_WAITINGAREA,           // 直行等待或待转区指示牌
        TRAFFICSIGN_TYPE_BUTT,
    };

    /**
    * ParkingAreaType 停车场类型
    */
    enum class ParkingAreaType : uint8_t {
        PARKING_AREA_TYPE_INVALID = 0,      // 未知
        PARKING_AREA_TYPE_INDOOR,           // 室内停车场
        PARKING_AREA_TYPE_OUTDOOR,          // 室外停车场
        PARKING_AREA_TYPE_BUTT,
    };


    /**
     * StopLineType 停止线类型
     */
    enum class StopLineType : uint8_t {
        STOPLINE_TYPE_INVALID = 0,         // 无效值
        STOPLINE_TYPE_REALISTIC,           // 实体停止线
        STOPLINE_TYPE_VIRTUAL,             // 虚拟停止线
        STOPLINE_TYPE_BUTT,
    };

    enum class PoleType : uint8_t {
        POLE_TYPE_INVALID = 0, 	         // 无效
        POLE_TYPE_OTHER,                 // 其它干状物
        BOUNDARY_ISOLATION_BUTT,
    };

}

#endif // INCLUDE_HDMAP_HDMAP_DATA_EXPANSION_H

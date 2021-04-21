/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Map map related interface.
 * Author: w00480427
 * Create: 2020-04-28
 */

#ifndef INCLUDE_HDMAP_HDMAP_DATA_H_
#define INCLUDE_HDMAP_HDMAP_DATA_H_

#include <cstdint>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include "..\ThirdParty\carfield_map_api\include\hdmap_data_expansion.h"

#define INVALID_ID  0UL

namespace hdmap {

// Feature
    class Feature {
    public:
        Feature(const FeatureType &type, const MapFeaturePtr &feature)
            : type_(type), feature_(feature) {}

        Feature(const FeatureType &type, const MapFeaturePtr &feature, HdMapClient& client)
            : type_(type), feature_(feature), client_(&client) {}


        virtual ~Feature() = default;

        /**
         * 获取Feature的ID
         * @return
         */
        const MapId& GetID() const;

        /**
         * 获取要素类别
         * @return 查询结果
         */
        FeatureType GetFeatureType() const
        {
            return type_;
        }

        /**
         * 获取要素的几何信息
         * @return
         */
        const std::vector<Point>& GetGeometry();

        /**
         * 获取关联的Features
         * @param featureType
         * @param relatedFeatures
         * @return
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &featureType, std::vector<MapId> &relatedFeatures) const
        {
            return MAP_UNSUPPORTED;
        }



    public:



    protected:
        FeatureType type_;
        MapFeaturePtr feature_;

        HdMapClient *client_ = nullptr;

    };
    using FeaturePtr = std::shared_ptr<Feature>;

/**************************************************************************************************
 *                                             Road                                               *
 **************************************************************************************************/
    class Road : public Feature {
    public:
        explicit Road(const MapFeaturePtr& road) : Feature(FeatureType::FEATURE_TYPE_ROAD, road) {}
        explicit Road(const MapFeaturePtr& road, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_ROAD, road, client) {}
        virtual ~Road();

        /**
         * 获取RoadClass
         * @return
         */
        RoadClass GetRoadClass() const;

		/**
		 * 获取道路类别
		 * @return 查询结果
		 */
        RoadType GetRoadType() const;

        /**
         * 获取道路长度，单位m
         * @return 查询结果
         */
        float GetLength() const;

        /**
         * 获取关联左边界
         * @return 查询结果
         */
        const MapId& GetLeftBoundary() const;

        /**
         * 获取关联右边界
         * @return 查询结果
         */
        const MapId& GetRightBoundary() const;

		/**
		 * 获取是否为双向道路
		 * @return 查询结果
		 */
        bool GetIsBothway() const;

        /**
         * 获取接续关系下几条road
         * @return 查询结果
         */
        const std::vector<MapId>& GetNexts() const;

        /**
         * 获取接续关系上几条road
         * @return 查询结果
         */
        const std::vector<MapId>& GetPrevs() const;

        /**
         * 查询road关联的lanes，返回laneIds数据的排列横向是从左到右
         *
         * @param laneType      车道类型, 默认非路肩车道(除了路肩车道LANE_TYPE_SHOULDER和LANE_TYPE_INVALID)
         * @param vehicleType   车类型, 默认所有机动车道(除了非机动车VEHICLE_TYPE_NOMOTORVEHICLE和VEHICLE_TYPE_INVALID)
         * @param onlyInCache   是否只在缓存里查找
         * @return
         */
        std::vector<MapId> GetLanes(const LaneType laneType = LaneType::LANE_TYPE_FILTER_NON_SHOULDER,
                                    const VehicleType vehicleType = VehicleType::VEHICLE_TYPE_FILTER_MOTOVEHICLE,
                                    const bool onlyInCache = false);

        /**
         * 通过LaneType获取lanes
         *
         * @param laneType      车道类型, 默认非路肩车道(除了路肩车道LANE_TYPE_SHOULDER和LANE_TYPE_INVALID)
         * @param onlyInCache
         * @return
         */
        std::vector<MapId> GetLanesByLaneType(const LaneType laneType = LaneType::LANE_TYPE_FILTER_NON_SHOULDER,
                                              const bool onlyInCache = false);

        /**
         * 通过VehicleType获取lanes
         *
         * @param vehicleType   车类型, 默认所有机动车道(除了非机动车VEHICLE_TYPE_NOMOTORVEHICLE和VEHICLE_TYPE_INVALID)
         * @param onlyInCache
         * @return
         */
        std::vector<MapId> GetLanesByVehicleType(const VehicleType vehicleType =
                                                 VehicleType::VEHICLE_TYPE_FILTER_MOTOVEHICLE,
                                                 const bool onlyInCache = false);

        /**
         * 获取对向道路, 如果对象道路有多条, 按行驶方向排序
         * @return 查询结果
         */
        const std::vector<MapId>& GetOpposedRoads() const;

        /**
         * 获取关联路口前的对向道路
         * @return 查询结果
         */
        const std::vector<MapId>& GetIntersectionOppositeRoads() const;


        /**
         * 查询 Road 关联的Features
         *
         * FEATURE_TYPE_CROSSWALK   N
         * FEATURE_TYPE_LANE        N
         * FEATURE_TYPE_TOLLAREA    1
         * FEATURE_TYPE_PARKINGSPOT N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using RoadPtr = std::shared_ptr<Road>;

/**************************************************************************************************
 *                                          SpeedLimit                                            *
 **************************************************************************************************/
    struct SpeedLimit {
        uint32_t curvatureMaxSpeed  :8;      // 曲率限速, 只在ICA+才提供
        uint32_t minSpeed           :8;      // 最低速度, 公里/小时
        uint32_t maxSpeed           :8;      // 最高速度, 公里/小时
        uint32_t reserved    		:8;
    };


/**************************************************************************************************
 *                                          Restriction                                            *
 **************************************************************************************************/
    struct Restriction {
        uint32_t vehicleType      :8;   // 接口保留, 数据规格暂无: VehicleType.用于表示禁止的车辆类型.
        uint32_t timePeriodType   :8;   // 接口保留, 数据规格暂无: TimePeriodType。用于表示禁止的时间类型
        uint32_t laneTurn         :4;   // LaneTurn。可能存在位域合并。用于表示禁止的转向
        uint32_t permission       :1;   // 0-无效, 1-禁止, 2-允许. 对应规格中的限制时间类型
        uint32_t reserved         :11;
        uint32_t startTime;             // 起始时间。DAY_OF_WEEK 时作为星期 bit 位复用. 前面高８位，表示星期几．后面表示分钟数
        uint32_t endTime;               // 结束时间
    };


/**************************************************************************************************
 *                                             Lane                                               *
 **************************************************************************************************/
    class Lane : public Feature {
    public:
        explicit Lane(const MapFeaturePtr& lane_ptr)
            : Feature(FeatureType::FEATURE_TYPE_LANE, lane_ptr) {}
        explicit Lane(const MapFeaturePtr& lane, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_LANE, lane, client) {}
        virtual ~Lane();

        /**
         * 获取VehicleType
         * @return 查询结果
         */
        VehicleType GetVehicleType() const;

        /**
         * 获取 LaneType
         * @return 查询结果
         */
        LaneType GetLaneType() const;

        /**
         * 获取车道转向信息,返回值按位域进行合并。 如组合直行和左转等
         * 规格中存放的是canStraight, canTurnLeft, can TurnRight, canUTurn
         * @return 查询结果
         */
        uint8_t GetLaneTurn() const;

        /**
         * 获取当前车道的转向信息.
         * 与GetLaneTurn不一样, GetLaneTurn返回的值跟车道在路口内和路口外会有区别; 详见数据规格的说明
         * 注意: 返回值与LaneType返回值类似,即1代表左转,2代表直行,4代表右转,8代表掉头
         *
         * @return
         */
        uint8_t GetCurrentTurn() const;

        /**
         * 是否直行
         * @return 查询结果
         */
        bool IsLaneTurnStraight() const
        {
            return (GetLaneTurn() & static_cast<uint8_t>(LaneTurn::LANE_TURN_STRAIGHT)) ==
                    static_cast<uint8_t>(LaneTurn::LANE_TURN_STRAIGHT);
        }

        /**
         * 是否左转
         * @return 查询结果
         */
        bool IsLaneTurnLeft() const
        {
            return (GetLaneTurn() & static_cast<uint8_t>(LaneTurn::LANE_TURN_LEFT)) ==
                    static_cast<uint8_t>(LaneTurn::LANE_TURN_LEFT);
        }

        /**
         * 是否右转
         * @return 查询结果
         */
        bool IsLaneTurnRight() const
        {
            return (GetLaneTurn() & static_cast<uint8_t>(LaneTurn::LANE_TURN_RIGHT)) ==
                    static_cast<uint8_t>(LaneTurn::LANE_TURN_RIGHT);
        }

        /**
         * 是否掉头
         * @return 查询结果
         */
        bool IsLaneTurnUturn() const
        {
            return (GetLaneTurn() & static_cast<uint8_t>(LaneTurn::LANE_TURN_UTURN)) ==
                    static_cast<uint8_t>(LaneTurn::LANE_TURN_UTURN);
        }

        /**
         * 获取车道编号, 从左到右, 从 0 到 n
         * @return 查询结果
         */
        uint8_t GetSequence() const;

        /**
         * 是否为路口内车道。如待转区、虚拟车道
         * @return 查询结果
         */
        bool IsIntersectionLane() const;

        /**
         * 是否为路口进入车道
         * @return
         */
        bool IsIntersectionInLane() const;

        /**
         * 是否为路口驶出车道
         * @return 查询结果
         */
        bool IsIntersectionOutLane() const;

        /**
         * 获取关联的道路 ID
         * @return 查询结果
         */
        const MapId& GetRoad() const;

        /**
         * 获取左侧车道
         * @return 查询结果
         */
        const MapId& GetLeftLane() const;

        /**
         * 车道右侧车道
         * @return 查询结果
         */
        const MapId& GetRightLane() const;

        /**
         * 获取车道左侧边线
         * @return 查询结果
         */
        uint64_t GetLeftBoundary() const;

        /**
         * 获取车道右侧边线
         * @return 查询结果
         */
        uint64_t GetRightBoundary() const;

        /**
         * 获取接续关系下几条 Lane
         * @return 查询结果
         */
        const std::vector<MapId>& GetNexts() const;

        /**
         * 获取接续关系上几条 Lane
         * @return 查询结果
         */
        const std::vector<MapId>& GetPrevs() const;

        /**
         * 获取限速信息
         * @return 查询结果
         */
        const SpeedLimit& GetSpeedLimit() const;

        /**
         * 限行信息
         * @return
         */
        const std::vector<Restriction>& GetRestrictions() const;

        /**
         * 获取车道长度,单位:m
         * @return 查询结果
         */
        float GetLength() const;

        /**
         * 查询 Lane 关联的Features
         *
         * FEATURE_TYPE_INTERSECTION    1
         * FEATURE_TYPE_TRAFFICLIGHT    N
         * FEATURE_TYPE_STOPLINE        1
         * FEATURE_TYPE_OBSTACLE        N
         * FEATURE_TYPE_ROADMARKING     N
         * FEATURE_TYPE_TRAFFICSIGN     N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;

    private:
        int32_t GetRelatedFeaturesOthers(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using LanePtr = std::shared_ptr<Lane>;

/**************************************************************************************************
 *                                           Boundary                                             *
 **************************************************************************************************/
    class Boundary : public Feature {
    public:
        explicit Boundary(const MapFeaturePtr& boundary): Feature(FeatureType::FEATURE_TYPE_BOUNDARY, boundary) {}
        explicit Boundary(const MapFeaturePtr& boundary, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_BOUNDARY, boundary, client) {}

        virtual ~Boundary();

        /**
         * 获取Boundary物理隔离类型
         * @return  查询结果
         */
        BoundaryIsolationType GetIsolationType() const;

        /**
         * 获取Boundary线型
         * @return  查询结果
         */
        LineType GetLineType() const;

        /**
         * 获取Boundary长度,单位:m
         * @return  查询结果
         */
        float GetLength() const;
    };
    using BoundaryPtr = std::shared_ptr<Boundary>;

/**************************************************************************************************
 *                                         Intersection                                           *
 **************************************************************************************************/
    class Intersection : public Feature {
    public:
        explicit Intersection(const MapFeaturePtr& intersection)
            : Feature(FeatureType::FEATURE_TYPE_INTERSECTION, intersection) {}
        explicit Intersection(const MapFeaturePtr& intersection, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_INTERSECTION, intersection, client) {}

        virtual ~Intersection();

        /**
         * 获取驶入lanes
         * @return
         */
        const std::vector<MapId>& GetInLanes() const;

        /**
         * 获取驶出lanes
         * @return
         */
        const std::vector<MapId>& GetOutLanes() const;

        /**
         * 查询 intersection 关联的Features
         *
         * FEATURE_TYPE_LANE        N
         * FEATURE_TYPE_OBSTACLE    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using IntersectionPtr = std::shared_ptr<Intersection>;

/**************************************************************************************************
 *                                           Stopline                                             *
 **************************************************************************************************/
    class StopLine : public Feature {
    public:
        explicit StopLine(const MapFeaturePtr& stop_line): Feature(FeatureType::FEATURE_TYPE_STOPLINE, stop_line) {}
        explicit StopLine(const MapFeaturePtr& stop_line, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_STOPLINE, stop_line, client) {}
        virtual ~StopLine();

        /**
         * 获取StopLineType
         * @return  查询结果
         */
        StopLineType GetStopLineType() const;

        /**
         * 查询 stopline 关联的Features
         *
         * FEATURE_TYPE_LANE    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using StopLinePtr = std::shared_ptr<StopLine>;

/**************************************************************************************************
 *                                         TrafficLight                                           *
 **************************************************************************************************/
    class TrafficLight : public Feature {
    public:
        explicit TrafficLight(const MapFeaturePtr& traffic_light_ptr)
            : Feature(FeatureType::FEATURE_TYPE_TRAFFICLIGHT, traffic_light_ptr) {}
        explicit TrafficLight(const MapFeaturePtr& traffic_light_ptr, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_TRAFFICLIGHT, traffic_light_ptr, client) {}
        virtual ~TrafficLight();

        /**
         * 获取是否混合类型灯, 决定了获取红绿灯类型的方法具体使用哪个
         * @return  查询结果
         */
        bool GetIsHybrid() const;

        /**
         * 非混合灯, 获取trafficlight类型. 对应规格中的LightType
         * @return  查询结果
         */
        TrafficLightType GetTrafficLightType() const;

        /**
         * 混合灯, 获取所有灯泡的trafficlight类型
         * @return  查询结果
         */
        const std::vector<TrafficLightType>& GetLightBulbTypes() const;

        /**
         * 获取灯泡数量, 对应规格中的BulbNum
         * @return  查询结果
         */
        uint32_t GetLightNum() const;

        /**
         * 获取灯泡到路面的高度 单位m
         * @return  查询结果
         */
        float GetHeight() const;

        /**
         * 是否倒计时灯
         * @return  查询结果
         */
        bool HasCountDownLight() const;

        /**
         * 获取朝向
         * @return  查询结果
         */
        float GetHeading() const;

        /**
         * 查询 TrafficLight
         *
         * FEATURE_TYPE_LANE    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using TrafficLightPtr = std::shared_ptr<TrafficLight>;

/**************************************************************************************************
 *                                          Crosswalk                                             *
 **************************************************************************************************/
    class Crosswalk : public Feature {
    public:
        explicit Crosswalk(const MapFeaturePtr& cross_walk): Feature(FeatureType::FEATURE_TYPE_CROSSWALK, cross_walk) {}
        explicit Crosswalk(const MapFeaturePtr& cross_walk, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_CROSSWALK, cross_walk, client) {}
        virtual ~Crosswalk();

        /**
         * 查询 crosswalk 关联的Features
         *
         * FEATURE_TYPE_ROAD    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId>& features) const;
    };
    using CrosswalkPtr = std::shared_ptr<Crosswalk>;

/**************************************************************************************************
 *                                         RoadMarking                                            *
 **************************************************************************************************/
    class RoadMarking : public Feature {
    public:
        explicit RoadMarking(const MapFeaturePtr& road_marking)
            : Feature(FeatureType::FEATURE_TYPE_ROADMARKING, road_marking) {}
        explicit RoadMarking(const MapFeaturePtr& road_marking, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_ROADMARKING, road_marking, client) {}
        virtual ~RoadMarking();

        /**
         * 获取路面标志类别
         * @return  查询结果
         */
        RoadMarkingType GetRoadMarkingType() const;

        /**
         * 查询 roadmarking 关联的Features
         *
         * FEATURE_TYPE_LANE    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    };
    using RoadMarkingPtr = std::shared_ptr<RoadMarking>;

/**************************************************************************************************
 *                                         TrafficSign                                            *
 **************************************************************************************************/
    class TrafficSign : public Feature {
    public:
        explicit TrafficSign(const MapFeaturePtr& traffic_sign_ptr)
            : Feature(FeatureType::FEATURE_TYPE_TRAFFICSIGN, traffic_sign_ptr) {}
        explicit TrafficSign(const MapFeaturePtr& traffic_sign_ptr, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_TRAFFICSIGN, traffic_sign_ptr, client) {}
        virtual ~TrafficSign();

        /**
         * 获取交通信号牌的类别
         * @return
         */
        TrafficSignType GetTrafficSignType() const;

        /**
         * 获取交通指示牌下边沿到路面高度 单位:m
         * @return  查询结果
         */
        float GetHeight();

        /**
         * 查询 trafficsign 关联的features
         *
         * FEATURE_TYPE_LANE    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    };
    using TrafficSignPtr = std::shared_ptr<TrafficSign>;

/**************************************************************************************************
 *                                             Pole                                               *
 **************************************************************************************************/
    class Pole : public Feature {
    public:
        explicit Pole(const MapFeaturePtr& pole) : Feature(FeatureType::FEATURE_TYPE_POLE, pole) {}
        explicit Pole(const MapFeaturePtr& pole, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_POLE, pole, client) {}
        virtual ~Pole(){}

        /**
         * 获取杆状物高度
         * @return  查询结果
         */
        uint32_t GetHeight() const;

        /**
         * 获取Pole的类型
         * @return
         */
        PoleType GetPoleType() const;
    };
    using PolePtr = std::shared_ptr<Pole>;

/**************************************************************************************************
 *                                         ParkingArea                                            *
 **************************************************************************************************/
    class ParkingArea : public Feature {
    public:
        explicit ParkingArea(const MapFeaturePtr& parking_area_ptr)
            : Feature(FeatureType::FEATURE_TYPE_PARKINGAREA, parking_area_ptr) {}
        explicit ParkingArea(const MapFeaturePtr& parking_area_ptr, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_PARKINGAREA, parking_area_ptr, client) {}
        virtual ~ParkingArea() {}

        /**
         * 获取停车场类别, 对应规格中的Type
         * @return
         */
        ParkingAreaType GetParkingAreaType();

        /**
         * 获取驶入停车场的Road
         * @return
         */
        const std::vector <MapId>& GetRoadIn() const;

        /**
         * 获取驶出停车场的Road
         * @return
         */
        const std::vector <MapId>& GetRoadOut() const;

        /**
         * 查询 ParkingArea 关联的要素
         *
         * ParkingSpot      N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    };
    using ParkingAreaPtr = std::shared_ptr<ParkingArea>;

/**************************************************************************************************
 *                                          ParkingSpot                                           *
 **************************************************************************************************/
    class ParkingSpot : public Feature {
    public:
        explicit ParkingSpot(const MapFeaturePtr& parking_spot_ptr)
            : Feature(FeatureType::FEATURE_TYPE_PARKINGSPOT, parking_spot_ptr) {}
        explicit ParkingSpot(const MapFeaturePtr& parking_spot_ptr, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_PARKINGSPOT, parking_spot_ptr, client) {}
        virtual ~ParkingSpot() {}

        /**
         * 查询 ParkingSpot 关联的要素
         *
         * FEATURE_TYPE_PARKINGAREA     1
         * FEATURE_TYPE_WHEELSTOPPER    1
         * FEATURE_TYPE_ROAD            1
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    private:
        int32_t GetRelatedFeaturesOthers(const FeatureType &type, std::vector<MapId> &features) const;

    };
    using ParkingSpotPtr = std::shared_ptr<ParkingSpot>;
	
// wheel stopper
    class WheelStopper : public Feature {
    public:
        explicit WheelStopper(const MapFeaturePtr& wheelstopper_ptr)
            : Feature(FeatureType::FEATURE_TYPE_WHEELSTOPPER, wheelstopper_ptr) {}
        explicit WheelStopper(const MapFeaturePtr& wheelstopper_ptr, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_WHEELSTOPPER, wheelstopper_ptr, client) {}
        virtual ~WheelStopper() {}

        /**
         * 查询 WheelStopper 关联的要素
         *
         * FEATURE_TYPE_PARKINGSPOT     1
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    };
    using WheelStopperPtr = std::shared_ptr<WheelStopper>;

    struct MapRoutePoint {
        MapId roadId = 0U;
        double x = 0.0;
        double y = 0.0;
        double heading = -1.0;
    };

/**************************************************************************************************
 *                                         Obstacle                                               *
 **************************************************************************************************/
    class Obstacle : public Feature {
    public:
        explicit Obstacle(const MapFeaturePtr& obstacle) : Feature(FeatureType::FEATURE_TYPE_OBSTACLE, obstacle) {}
        explicit Obstacle(const MapFeaturePtr& obstacle, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_OBSTACLE, obstacle, client) {}
        virtual ~Obstacle() {}

        /**
         * 获取是否行人或自行车可跨越
         * @return  查询结果
         */
        bool GetTraversable() const;

        /**
         * 查询 Obstacle 关联的要素
         *
         * FEATURE_TYPE_LANE            N
         * FEATURE_TYPE_INTERSECTION    N
         *
         * @param type          Feature类型
         * @param features      查询结果集合
         * @return              查询结果标志
         */
        virtual int32_t GetRelatedFeatures(const FeatureType &type, std::vector<MapId> &features) const;
    };
    using ObstaclePtr = std::shared_ptr<Obstacle>;

/**************************************************************************************************
 *                                         TollArea                                               *
 **************************************************************************************************/
    class TollArea : public Feature {
    public:
        explicit TollArea(const MapFeaturePtr& tollArea) : Feature(FeatureType::FEATURE_TYPE_TOLLAREA, tollArea) {}
        explicit TollArea(const MapFeaturePtr& tollArea, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_TOLLAREA, tollArea, client) {}
        virtual ~TollArea() {}

        /**
         * 获取关联进入的road
         * @return  查询结果
         */
        const std::vector<MapId>& GetInRoadIds() const;

        /**
         * 获取关联进入的road
         * @return  查询结果
         */
        const std::vector<MapId>& GetOutRoadIds() const;
    };
    using TollAreaPtr = std::shared_ptr<TollArea>;

    struct NavigationState {
        uint64_t updateTimestamp = 0U;
        std::vector<MapId> curLaneIds;          // 当前车道Ids
        std::vector<MapId> curRoadIds;          // 当前道路Ids
        std::vector<double> laneWidths;         // 车道宽度
        double distanceToIntersection = 0.0;
        double distanceFromIntersection = 0.0;
        MapId toIntersectionId = 0U;
        MapId fromIntersectionId = 0U;
    };

/**************************************************************************************************
 *                                           SdRoad                                               *
 **************************************************************************************************/
    struct PointSlope {
        Point point;
        int16_t slope; // 该点颜通行方向的坡度
    };
    struct SlopeInfo {
        std::vector<PointSlope> slopes; // 包括本road的起点,中间形状点和终点
    };
    using SlopeInfoPtr = std::shared_ptr<SlopeInfo>;

    struct PointCurvature {
        Point point;
        int32_t curvature;
    };
    struct NodeCurvature {
        Point point;
        std::vector<std::pair<uint64_t, int32_t>> curvatures; // 对于起点, key是pre road id, 对于终点, key是next road id
    };
    struct CurvatureInfo {
        std::vector<PointCurvature> middles;
        NodeCurvature end; // 道路通行方向的终点, 曲率值为从本road经过终点沿不同的next road退出本road的曲率
    };
    using CurvatureInfoPtr = std::shared_ptr<CurvatureInfo>;
    struct IntersectionLane {
        MapId inRoadId;
        uint8_t laneNum;
        // laneType表示id road中每个车道的车道类型, 最多8个车道
        // laneType[0]~laneType[7]分别表示沿道路通行方向从左到右的8条车道,
        // 每个车道形状由一个字节共8位表示, 从最高位~最低位分别表示:直行, 右前方, 右转, 右后方, 掉头, 左后方, 左转, 左前方
        uint8_t laneType[8] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU}; // 8 is max lane num of sdroad
        // outRoadIds是给定in road的一个或多个退出路口的道路
        // uint8_t的最低位~最高位分别表示沿道路通行方向从左到右的8条车道的通行情况, 为1表示可通行, 为0表示不可通行
        std::vector<std::pair<MapId, uint8_t>> outRoadIds;
    };
    using IntersectionLanePtr = std::shared_ptr<IntersectionLane>;

    class SdRoad : public Feature {
    public:
        explicit SdRoad(const MapFeaturePtr& sdRoad) : Feature(FeatureType::FEATURE_TYPE_SDROAD, sdRoad) {}
        explicit SdRoad(const MapFeaturePtr& sdRoad, HdMapClient& client)
            : Feature(FeatureType::FEATURE_TYPE_SDROAD, sdRoad, client) {}
        virtual ~SdRoad() {}

        uint32_t GetLength() const; // meter
        RoadClass GetRoadClass() const; // 暂时以function class + 1代替
        uint64_t GetSdRoadType() const;
        uint16_t GetLaneNum() const;
        const std::vector<uint8_t>& GetLaneTurns() const;
        uint8_t GetSpeedLimit() const;
        const std::vector<MapId>& GetPrevs() const;
        const std::vector<MapId>& GetNexts() const;
        bool IsIntersectionLane() const;
        bool HasTrafficLight() const;
        const SlopeInfoPtr& GetSlope() const;
        const CurvatureInfoPtr& GetCurvature() const;
        const IntersectionLanePtr& GetIntersectionLane() const;

        bool IsOther() const;
        bool IsFerry() const;
        bool IsTunnel() const;
        bool IsBridge() const;
        bool IsToll() const;
        bool IsFreeway() const;
        bool IsCityFreeway() const;
        bool IsOverpass() const;
        bool IsIntersection() const;
        bool IsNoThrough() const;
        bool IsUrbanRoad() const;
        bool IsRamp() const;
        bool IsIc() const;
        bool IsJct() const;
        bool IsRoundCircle() const;
        bool IsServiceRoad() const;
        bool IsParkingRoad() const;
        bool IsSideRoad() const;
        bool IsWalkWay() const;
        bool IsDividedRoad() const;
        bool IsForbid() const;
        bool IsAllDayForbid() const;
        bool IsAreaRoad() const;
        bool IsAllClosedRoad() const;
        bool IsBusRoad() const;
        bool IsEarlyLeftTurn() const;
        bool IsEarlyRightTurn() const;
        bool IsUTurnRoad() const;
        bool IsMainSideRoadGate() const;
        bool IsGuideRoadOfPark() const;
        bool IsVirtualConnectRoad() const;
        bool IsTurnLeftBorrowRoad() const;
        bool IsParkingGate() const;
        bool IsOtherNoAttributeRoad() const;
    };
    using SdRoadPtr = std::shared_ptr<SdRoad>;


}  // namespace hdmap
#endif  // INCLUDE_HDMAP_HDMAP_DATA_H_

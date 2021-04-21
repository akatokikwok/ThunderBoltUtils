/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Hdmap related extension interface
 * Author: w00480427
 * Create: 2020-04-28
 */

#ifndef MAPPING_HDMAP_EXPANSION_H
#define MAPPING_HDMAP_EXPANSION_H

#include <vector>
#include "hdmap_client_api.h"

namespace hdmap {

    class HdMapExpansion {
    public:
        HdMapExpansion(HdMapClient* const client);

        ~HdMapExpansion();

        /**
         * @brief 根据指定道路ID，获取关联的障碍物
         * @param road_id          道路ID
         * @param l_obstacles      左边障碍物集合
         * @param r_obstacles      右边障碍物集合
         * @param center_obstacles 道路中间障碍物
         * @retval MAP_OK          查找成功
         * @retval MAP_FAIL        查找失败
         * @retval MAP_NO_RESULT   无要素
         */
        int32_t GetRoadObstacle(const MapId &road_id, std::vector<ObstaclePtr> &l_obstacles,
                                std::vector<ObstaclePtr> &r_obstacles, std::vector<ObstaclePtr> &center_obstacles);

        /**
         * @brief 根据指定路口ID，获取关联的路口障碍物
         * @param intersection_id 路口ID
         * @param obstacles       路口关联的障碍物集合
         * @retval MAP_OK         查找成功
         * @retval MAP_FAIL       查找失败
         * @retval MAP_NO_RESULT  无要素
         */
        int32_t GetIntersectionObstacle(const MapId &intersection_id, std::vector<ObstaclePtr> &obstacles);

        void SetOnlyInCache(const bool value)
        {
            only_in_cache = value;
        }

    private:
        void LoadRoadObstacle(const RoadPtr &road_ptr, std::vector<ObstaclePtr> &l_obstacles,
                              std::vector<ObstaclePtr> &r_obstacles, std::vector<ObstaclePtr> &c_obstacles);

        int CheckObstaclePosition(const RoadPtr &road_ptr, const ObstaclePtr &op);

        HdMapClient *hdmap_client = nullptr;

        bool only_in_cache = false;
    };
}

#endif //MAPPING_HDMAP_EXPANSION_H

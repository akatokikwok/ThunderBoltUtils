/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Map client related interface.
 * Author: w00480427
 * Create: 2020-04-28
 */

#ifndef INCLUDE_HDMAP_HDMAP_API_H_
#define INCLUDE_HDMAP_HDMAP_API_H_

#include <string>
#include <memory>
#include <vector>

#include "ads_app.h"
#include "hdmap_data.h"

namespace hdmap {

class HdmapClientImpl;
class HdMapClient {
public:
    HdMapClient();
    ~HdMapClient();

    /**
     * @brief 地图初始化
     *
     * @retval   MAP_OK         查找成功
     * @retval   MAP_FAIL       查找失败
     */
    int32_t Init(const std::string& nameSpace = "");

    int32_t Init(AdsPlatform::AdsApp* adsApp, const std::string& nameSpace = "");
    
    /**
     * @brief 根据坐标和半径生成范围,获取指定范围内的指定类别所有要素
     *
     * @param   x
     * @param   y
     * @param   distance
     * @param   featureType
     * @param   features
     * @return  int32_t
     * @retval  MAP_OK         查找成功
     * @retval  MAP_FAIL       查找失败
     * @retval  MAP_NO_RESULT  无要素
     */
    int32_t GetFeatures(const double x, const double y, const double distance, FeatureType featureType,
                        std::vector<FeaturePtr>& features, bool onlyInCache = false);

    /**
     * @brief 根据坐标和半径生成范围,获取指定范围的所有要素
     *
     * @param   x
     * @param   y
     * @param   distance
     * @param   featureType
     * @param   features
     * @param   roadId        指定road id, 针对高架多层场景
     * @return  int32_t
     * @retval  MAP_OK         查找成功
     * @retval  MAP_FAIL       查找失败
     */
    int32_t GetFeatures(const double x, const double y, const double distance, FeatureType featureType,
                        std::vector<FeaturePtr>& features, const uint64_t roadId, bool onlyInCache = false);

    int32_t GetFeatures(const double x, const double y,  const double z, const double distance,
                        FeatureType featureType, std::vector<FeaturePtr>& features, bool onlyInCache = false);

    /**
     * @brief   根据坐标获取要素
     *
     * @param   featureId     属性id
     * @param   featureType   属性类型
     * @param   feature
     * @param   onlyInCache  是否只从缓存中查询,默认值为false. true:如果缓存中没有数据，返回为空．　false:如果缓存中没有数据，会往map server中查询．
     * @return  int32_t
     * @retval  MAP_OK         查找成功
     * @retval  MAP_FAIL       查找失败
     */
    int32_t GetFeature(const MapId& featureId, FeatureType featureType, FeaturePtr& feature, bool onlyInCache = false);

    /**
     * @brief   根据坐标获取场景
     *
     * @param   x　　　　　　　　当前位置
     * @param   y　　　　　　　　当前位置
     * @param   type　　　　　　要素类别
     * @param   featureVec　　要素集合　
     * @param   heading　　　　默认参数为-１，即不考虑自车的朝向．如果指定参数，会根据要素的朝向筛选要素．　
     * @param   onlyInCache   否只从缓存中查询,默认值为false.
     *                        true:如果缓存中没有数据，返回为空．　false:如果缓存中没有数据，会往map server中查询．
     * @return  int32_t
     * @retval  MAP_OK         查找成功
     * @retval  MAP_FAIL       查找失败
     */
    int32_t GetLocatedFeatures(const double x, const double y, FeatureType type, std::vector<FeaturePtr>& featureVec,
                               const double heading = -1.0, bool onlyInCache = false);

    int32_t GetLocatedFeatures(const double x, const double y, uint64_t roadId, const FeatureType type,
                               std::vector<FeaturePtr>& featureVec, const double heading = -1.0,
                               const bool onlyInCache = false);

    int32_t GetLocatedFeatures(const double x, const double y, const double z, const FeatureType type,
                               std::vector<FeaturePtr>& featureVec, const double heading = -1.0,
                               const bool onlyInCache = false);

    int32_t CalcRoute(const MapRoutePoint& startRoutePoint, const MapRoutePoint& endRoutePoint,
                      std::vector<MapId>& pathIds, const bool clearCache = true);

    int32_t CalcTraversalPath(const MapRoutePoint& startRoutePoint, const std::vector<MapId>& travelRoads,
                              std::vector<MapId>& pathIds);
    /**
     * @brief   根据指定位置获取到路口的距离.如果距离大于500米,则返回500米,小于500米,就返回具体的长度.
     *          主要是针对未来高速场景,可能几百公里都没有路口的情况.
     *
     * @param   x  　　             当前位置
     * @param   y                  当前位置
     * @param   roadId            指定road id, 针对高架多层场景　　　　　　   　　　
     * @outparam  distance　　　　　 距离　
     * @outparam  intersection_id  路口Id
     * @return
     */
    int32_t GetDistanceToIntersection(const double x, const double y, uint64_t roadId, double& distance,
                                      MapId* intersectionId = nullptr, bool onlyInCache = false);

    int32_t GetDistanceToIntersection(const double x, const double y, const double z, double& distance,
                                      MapId* intersectionId = nullptr, bool onlyInCache = false);

    /**
     * @brief   根据当前位置获取从过来的路口的距离.
     *          如果距离大于500米,则返回500米;
     *          小于500米,就返回具体的长度.主要是针对未来高速场景,可能几百公里都没有路口的情况.
     *
     * @param   x
     * @param   y
     * @param   onlyInCache  只中缓存中查询,默认值为false.
     *          false:如果缓存中没有数据，返回为空．　true:如果缓存中没有数据，会往map server中查询．
     * @return
     */
    int32_t GetDistanceFromIntersection(const double x, const double y, uint64_t roadId, double& distance,
                                        MapId* intersectionId = nullptr, bool onlyInCache = false);

    int32_t GetDistanceFromIntersection(const double x, const double y, double z, double& distance,
                                        MapId* intersectionId = nullptr, bool onlyInCache = false);


   /**
     * @brief  根据当前坐标，查询要素的宽度。（只支持车道和道路）
     *
     * @param   x
     * @param   y
     * @param   onlyInCache  只中缓存中查询,默认值为false.
     *          false:如果缓存中没有数据，返回为空．　true:如果缓存中没有数据，会往map server中查询．
     * @return
     */
    int32_t GetWidth(const double x, const double y, const FeaturePtr& feature,
                     double& distance, bool onlyInCache = false);

    /**
     * @brief map 服务端 50HZ更新和下发navigation
     * @param navigationState
     * @return
     */
    int32_t GetNavigationState(NavigationState& navigationState);

    int32_t GetMapServiceVersion(std::string& mapServiceVersion);

    int32_t GetMapDataVersion(std::string& mapDataVersion);
    /**
     * 获取沿路的所有要素, 存入缓存
     * @param roadIds
     * @return
     */
    int32_t GetPathCacheByRoadIds(const std::vector<MapId>& roadIds);

    /**
     * 清空沿路要素的缓存
     * @return
     */
    int32_t ClearPathCache();

    /**
     * Hdmap Client Impl修改为单例模式.
     * 手动清空Hdmap Client Impl
     */
    void DestoryMapClient();

protected:
    HdMapClient(const HdMapClient&) = delete;
    HdMapClient& operator= (const HdMapClient&) = delete;

private:
    std::string software_version_;
    static std::shared_ptr<HdmapClientImpl> hdmap_impl_;
    static std::mutex lock_;
    std::shared_ptr<HdmapClientImpl> hdmap_impl_self_;
    std::shared_ptr<AdsPlatform::AdsApp> ads_app_;
};

}  // namespace hdmap

#endif  // INCLUDE_HDMAP_HDMAP_API_H_

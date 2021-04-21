/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: hdmap_api
 * Author: y00265767
 * Create: 2019-08-26
 */

#ifndef INCLUDE_HDMAP_HDMAP_API_H_
#define INCLUDE_HDMAP_HDMAP_API_H_

#include <string>
#include <memory>
#include <vector>

#include "hdmap/hdmap_data.h"
namespace map {class HdmapDataApi; class SdmapDataApi;}
namespace hdmap {
    class BaseAdapter;

    class HdMap {
    public:
        HdMap();

        ~HdMap();

        int32_t Init(const std::string& path = ".", const std::string& name = "");

		int32_t Init(BaseAdapter& adapter);

        std::string GetDataVersion() const;

        void SetScenario(const double &x, const double &y, const int &utm_zone);

        int32_t GetFeaturesByPosition(const hdmap::Point& point, const double &distance, const hdmap::FeatureType &type,
                                      std::vector <hdmap::FeaturePtr>& features) const;

        int32_t GetFeatureByID(const uint64_t &featureId, const hdmap::FeatureType &type,
                               hdmap::FeaturePtr& feature) const;

        int32_t GetFeaturesByLocated(const hdmap::Point &point, const hdmap::FeatureType &type,
                                     std::vector <hdmap::FeaturePtr> &results) const;

        int32_t GetFeaturesByLocated3D(const hdmap::Point &point, const hdmap::FeatureType &type,
                                       std::vector <hdmap::FeaturePtr> &results) const;

    protected:
        HdMap(const HdMap&) = delete;

        HdMap& operator=(const HdMap&) = delete;

    private:
        std::string software_version_;
	    mutable ::map::HdmapDataApi* hdmapDataApi = nullptr;
	    mutable ::map::SdmapDataApi* sdmapDataApi = nullptr;
		BaseAdapter* adapter_;
    };
}

#endif

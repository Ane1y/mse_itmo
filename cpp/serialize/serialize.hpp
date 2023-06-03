#pragma once

#include <vector>
#include <string>
#include "boost/serialization/vector.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace cls_06 {

    struct GeoPoint {

        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & lat;
            ar & lon;
        }

        friend bool operator==(const GeoPoint& lhs, const GeoPoint& rhs) {
            return lhs.lat == rhs.lat && lhs.lon == rhs.lon;
        }

        double lat = 0;
        double lon = 0;
    };

    struct GeoLine {
        std::vector<GeoPoint> points{};
        GeoLine() = default;
        GeoLine(const std::vector<GeoPoint>& points) : points(points) {}
        friend bool operator==(const GeoLine& lhs, const GeoLine& rhs) {
            if (lhs.points.size() != rhs.points.size()) {
                return false;
            }
            for (size_t i = 0; i < lhs.points.size(); ++i) {
                if (lhs.points[i] != rhs.points[i]) {
                    return false;
                }
            }
            return true;
        }

        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & points;
        }

    };

    struct Attribute {
        std::string name;
        std::string value;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & name;
            ar & value;
        }
        bool operator==(const Attribute& rhs) const {
            return name == rhs.name && value == rhs.value;
        }
        bool operator!=(const Attribute& rhs) const {
            return !(*this == rhs);
        }
    };

    class Object {
    public:
        Object() = default;
        explicit Object(std::vector<Attribute> attributes);
        const std::vector<Attribute> &GetAttributes() const;
        friend bool operator==(const Object& lhs, const Object& rhs) {
            if (lhs.attributes_.size() != rhs.attributes_.size()) {
                return false;
            }
            for (size_t i = 0; i < lhs.attributes_.size(); ++i) {
                if (lhs.attributes_[i] != rhs.attributes_[i]) {
                    return false;
                }
            }
            return true;
        }
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & attributes_;
        }
        std::vector<Attribute> attributes_;
    };

    class PointObject : public Object {
    public:
        PointObject() = default;
        PointObject(GeoPoint point, std::vector<Attribute> attributes);

    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & boost::serialization::base_object<Object>(*this);
            ar & point_;
        }

        friend bool operator==(const PointObject& lhs, const PointObject& rhs) {
            return lhs.point_ == rhs.point_ && static_cast<const Object&>(lhs) == rhs;
        }
        GeoPoint point_;
    };

    class LineObject : public Object {
    public:
        LineObject(GeoLine line, std::vector<Attribute> attributes);

    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & boost::serialization::base_object<Object>(*this);
            ar & line_;
        }
        friend bool operator==(const LineObject& lhs, const LineObject& rhs) {
            return lhs.line_ == rhs.line_ && static_cast<const Object&>(lhs) == rhs;
        }
        GeoLine line_;
    };

}
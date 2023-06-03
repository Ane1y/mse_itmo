#include <gtest/gtest.h>
#include "serialize.hpp"
#include <boost/archive/tmpdir.hpp>
#include <fstream>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

template <class T>
std::string save_to_file(const T& obj, const std::string& name) {
    std::string filename(boost::archive::tmpdir());
    filename += name;
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << obj;
    return filename;
}

template<class T>
T restore_schedule(const std::string& name) {
    T obj;
    std::ifstream ifs(name);
    boost::archive::text_iarchive ia(ifs);
    ia >> obj;
    return obj;
}

TEST(Ser, GeoPoint) {
    cls_06::GeoPoint geo_point{5, 6};
    std::string path_to_geo_point = save_to_file(geo_point, "/GeoPoint.txt");
    auto restored_point = restore_schedule<cls_06::GeoPoint>(path_to_geo_point);
    ASSERT_EQ(geo_point, restored_point);
}


TEST(Ser, GeoLine) {
    cls_06::GeoLine geo_line{std::vector{cls_06::GeoPoint{5, 6}, cls_06::GeoPoint{4, 6}}};
    std::string path = save_to_file(geo_line, "/GeoLine.txt");
    auto restored_line = restore_schedule<cls_06::GeoLine>(path);
    ASSERT_EQ(geo_line, restored_line);
}


TEST(Ser, Attribute) {
    cls_06::Attribute attr = {"name", "value"};
    std::string path = save_to_file(attr, "/Attribute.txt");
    auto restored_attr = restore_schedule<cls_06::Attribute>(path);
    ASSERT_EQ(attr, restored_attr);
}



TEST(Ser, Object) {
    cls_06::Object obj(std::vector<cls_06::Attribute>{{"name", "value"},
                                                      {"name1", "value1"}});
    std::string path = save_to_file(obj, "/Object.txt");
    auto restored_attr = restore_schedule<cls_06::Object>(path);
    ASSERT_EQ(obj, restored_attr);
}


TEST(Ser, PointObject) {
    cls_06::PointObject obj(cls_06::GeoPoint{6, 7}, std::vector<cls_06::Attribute>{{"name", "value"},
                                                      {"name1", "value1"}});
    std::string path = save_to_file(obj, "/Point_Object.txt");
    auto restored_attr = restore_schedule<cls_06::PointObject>(path);
    ASSERT_EQ(obj, restored_attr);
}


TEST(Ser, LineObject) {
    cls_06::GeoLine geo_line{std::vector{cls_06::GeoPoint{5, 6}, cls_06::GeoPoint{4, 6}}};
    cls_06::LineObject obj(geo_line, std::vector<cls_06::Attribute>{{"name", "value"},
                                                                                   {"name1", "value1"}});
    std::string path = save_to_file(obj, "/lineObject.txt");
    auto restored_attr = restore_schedule<cls_06::PointObject>(path);
    ASSERT_EQ(obj, restored_attr);
}
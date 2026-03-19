#include <gtest/gtest.h>
#include "convex_hull.hpp"

using namespace geometry;
using namespace geometry::convex_hull;

TEST(ConvexHullTest, SimpleSquare) {
    std::vector<Point2D> points = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 4u);
}

TEST(ConvexHullTest, WithInteriorPoints) {
    std::vector<Point2D> points = {{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}, {1, 1}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    // Interior points should not be in the hull
    EXPECT_EQ(result->size(), 4u);
}

TEST(ConvexHullTest, TooFewPointsReturnsError) {
    std::vector<Point2D> points = {{0, 0}, {1, 1}};
    auto result = GrahamScan(points);
    ASSERT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().empty());
}

TEST(ConvexHullTest, TriangleHull) {
    std::vector<Point2D> points = {{0, 0}, {5, 0}, {2.5, 5}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3u);
}

TEST(ConvexHullTest, CollinearPoints) {
    std::vector<Point2D> points = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}};
    auto result = GrahamScan(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 3u);
}

TEST(ConvexHullTest, CrossProductFunction) {
    Point2D p1{1, 0};
    Point2D mid{0, 0};
    Point2D p2{0, 1};
    double cp = CrossProduct(p1, mid, p2);
    EXPECT_GT(cp, 0.0);  // counter-clockwise
}

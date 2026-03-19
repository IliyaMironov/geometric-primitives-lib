#include <gtest/gtest.h>
#include "triangulation.hpp"

using namespace geometry;
using namespace geometry::triangulation;

TEST(TriangulationTest, SimpleTriangulation) {
    std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 1u);
}

TEST(TriangulationTest, FivePoints) {
    std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};
    auto result = DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 3u);
}

TEST(TriangulationTest, TooFewPointsError) {
    std::vector<Point2D> points = {{0, 0}, {1, 1}};
    auto result = DelaunayTriangulation(points);
    ASSERT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().empty());
}

TEST(TriangulationTest, DelaunayTriangleCircumcenter) {
    DelaunayTriangle t{{0, 0}, {4, 0}, {2, 4}};
    auto center = t.Circumcenter();
    double radius = t.Circumradius();
    // All vertices should be equidistant from circumcenter
    EXPECT_NEAR(center.DistanceTo(t.a), radius, 1e-6);
    EXPECT_NEAR(center.DistanceTo(t.b), radius, 1e-6);
    EXPECT_NEAR(center.DistanceTo(t.c), radius, 1e-6);
}

TEST(TriangulationTest, DelaunayTriangleContainsPoint) {
    DelaunayTriangle t{{0, 0}, {10, 0}, {5, 10}};
    // Point inside circumcircle
    EXPECT_TRUE(t.ContainsPoint({5, 3}));
}

TEST(TriangulationTest, SharesEdge) {
    DelaunayTriangle t1{{0, 0}, {4, 0}, {2, 3}};
    DelaunayTriangle t2{{0, 0}, {4, 0}, {2, -3}};
    EXPECT_TRUE(t1.SharesEdge(t2));
}

TEST(TriangulationTest, DoesNotShareEdge) {
    DelaunayTriangle t1{{0, 0}, {4, 0}, {2, 3}};
    DelaunayTriangle t2{{10, 10}, {14, 10}, {12, 13}};
    EXPECT_FALSE(t1.SharesEdge(t2));
}

TEST(TriangulationTest, EdgeNormalization) {
    Edge e1{{3, 4}, {1, 2}};
    Edge e2{{1, 2}, {3, 4}};
    EXPECT_TRUE(e1 == e2);
}

#include <gtest/gtest.h>
#include "queries.hpp"

using namespace geometry;
using namespace geometry::queries;

// === DistanceToPoint ===

TEST(QueriesTest, DistanceToPointLine) {
    Shape s = Line{{0, 0}, {4, 0}};
    double d = DistanceToPoint(s, {2, 3});
    EXPECT_NEAR(d, 3.0, 1e-6);
}

TEST(QueriesTest, DistanceToPointCircle) {
    Shape s = Circle{{0, 0}, 5.0};
    double d = DistanceToPoint(s, {10, 0});
    EXPECT_NEAR(d, 5.0, 1e-6);
}

TEST(QueriesTest, DistanceToPointInsideCircle) {
    Shape s = Circle{{0, 0}, 5.0};
    double d = DistanceToPoint(s, {1, 0});
    EXPECT_NEAR(d, 0.0, 1e-6);
}

// === GetBoundBox ===

TEST(QueriesTest, GetBoundBoxLine) {
    Shape s = Line{{1, 2}, {4, 6}};
    auto bb = GetBoundBox(s);
    EXPECT_DOUBLE_EQ(bb.min_x, 1.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 6.0);
}

TEST(QueriesTest, GetBoundBoxCircle) {
    Shape s = Circle{{5, 5}, 3};
    auto bb = GetBoundBox(s);
    EXPECT_DOUBLE_EQ(bb.min_x, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 8.0);
}

// === GetHeight ===

TEST(QueriesTest, GetHeightTriangle) {
    Shape s = Triangle{{0, 0}, {4, 0}, {2, 7}};
    EXPECT_DOUBLE_EQ(GetHeight(s), 7.0);
}

TEST(QueriesTest, GetHeightRectangle) {
    Shape s = Rectangle{{0, 10}, 5, 3};
    EXPECT_DOUBLE_EQ(GetHeight(s), 13.0);
}

// === BoundingBoxesOverlap ===

TEST(QueriesTest, BoundingBoxesOverlapTrue) {
    Shape s1 = Circle{{0, 0}, 5};
    Shape s2 = Circle{{3, 0}, 5};
    EXPECT_TRUE(BoundingBoxesOverlap(s1, s2));
}

TEST(QueriesTest, BoundingBoxesOverlapFalse) {
    Shape s1 = Circle{{0, 0}, 1};
    Shape s2 = Circle{{100, 100}, 1};
    EXPECT_FALSE(BoundingBoxesOverlap(s1, s2));
}

// === DistanceBetweenShapes ===

TEST(QueriesTest, DistanceBetweenCircles) {
    Shape s1 = Circle{{0, 0}, 1};
    Shape s2 = Circle{{5, 0}, 1};
    auto d = DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_NEAR(*d, 3.0, 1e-6);
}

TEST(QueriesTest, DistanceBetweenLines) {
    Shape s1 = Line{{0, 0}, {1, 0}};
    Shape s2 = Line{{0, 3}, {1, 3}};
    auto d = DistanceBetweenShapes(s1, s2);
    ASSERT_TRUE(d.has_value());
    EXPECT_NEAR(*d, 3.0, 1e-6);
}

TEST(QueriesTest, DistanceUnsupported) {
    Shape s1 = Triangle{{0, 0}, {1, 0}, {0, 1}};
    Shape s2 = Rectangle{{5, 5}, 1, 1};
    auto d = DistanceBetweenShapes(s1, s2);
    EXPECT_FALSE(d.has_value());
}

// === PointInShape ===

TEST(QueriesTest, PointInCircle) {
    Circle c{{0, 0}, 5};
    PointInShapeVisitor visitor{{1, 1}};
    EXPECT_TRUE(visitor(c));
}

TEST(QueriesTest, PointOutsideCircle) {
    Circle c{{0, 0}, 1};
    PointInShapeVisitor visitor{{10, 10}};
    EXPECT_FALSE(visitor(c));
}

TEST(QueriesTest, PointInRectangle) {
    Rectangle r{{0, 0}, 4, 4};
    PointInShapeVisitor visitor{{2, 2}};
    EXPECT_TRUE(visitor(r));
}

TEST(QueriesTest, PointOutsideRectangle) {
    Rectangle r{{0, 0}, 4, 4};
    PointInShapeVisitor visitor{{5, 5}};
    EXPECT_FALSE(visitor(r));
}

TEST(QueriesTest, PointInTriangle) {
    Triangle t{{0, 0}, {4, 0}, {2, 4}};
    PointInShapeVisitor visitor{{2, 1}};
    EXPECT_TRUE(visitor(t));
}

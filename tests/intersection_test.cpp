#include <gtest/gtest.h>
#include "intersections.hpp"

using namespace geometry;
using namespace geometry::intersections;

TEST(IntersectionTest, LineLineIntersect) {
    Line l1{{0, 0}, {4, 4}};
    Line l2{{0, 4}, {4, 0}};
    auto result = GetIntersectPoint(Shape{l1}, Shape{l2});
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 2.0, 1e-6);
    EXPECT_NEAR(result->y, 2.0, 1e-6);
}

TEST(IntersectionTest, LineLineParallel) {
    Line l1{{0, 0}, {4, 0}};
    Line l2{{0, 1}, {4, 1}};
    auto result = GetIntersectPoint(Shape{l1}, Shape{l2});
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineLineNoIntersect) {
    Line l1{{0, 0}, {1, 0}};
    Line l2{{2, 1}, {3, 2}};
    auto result = GetIntersectPoint(Shape{l1}, Shape{l2});
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, LineCircleIntersect) {
    Line l{{-5, 0}, {5, 0}};
    Circle c{{0, 0}, 1.0};
    auto result = GetIntersectPoint(Shape{l}, Shape{c});
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(std::abs(result->x), 1.0, 1e-6);
    EXPECT_NEAR(result->y, 0.0, 1e-6);
}

TEST(IntersectionTest, LineCircleNoIntersect) {
    Line l{{0, 5}, {4, 5}};
    Circle c{{0, 0}, 1.0};
    auto result = GetIntersectPoint(Shape{l}, Shape{c});
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, CircleCircleIntersect) {
    Circle c1{{0, 0}, 2.0};
    Circle c2{{3, 0}, 2.0};
    auto result = GetIntersectPoint(Shape{c1}, Shape{c2});
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result->x, 1.5, 1e-6);
}

TEST(IntersectionTest, CircleCircleNoIntersect) {
    Circle c1{{0, 0}, 1.0};
    Circle c2{{10, 0}, 1.0};
    auto result = GetIntersectPoint(Shape{c1}, Shape{c2});
    EXPECT_FALSE(result.has_value());
}

TEST(IntersectionTest, UnsupportedThrows) {
    Triangle t{{0, 0}, {1, 0}, {0, 1}};
    Rectangle r{{0, 0}, 1, 1};
    EXPECT_THROW(GetIntersectPoint(Shape{t}, Shape{r}), std::logic_error);
}

TEST(IntersectionTest, TriangleLineThrows) {
    Triangle t{{0, 0}, {1, 0}, {0, 1}};
    Line l{{0, 0}, {1, 1}};
    EXPECT_THROW(GetIntersectPoint(Shape{t}, Shape{l}), std::logic_error);
}

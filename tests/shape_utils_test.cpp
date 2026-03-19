#include <gtest/gtest.h>
#include "shape_utils.hpp"
#include "queries.hpp"

using namespace geometry;
using namespace geometry::utils;

// === ParseShapes ===

TEST(ShapeUtilsTest, ParseCircle) {
    auto shapes = ParseShapes("circle 0 0 1.5");
    ASSERT_EQ(shapes.size(), 1u);
    EXPECT_TRUE(std::holds_alternative<Circle>(shapes[0]));
    auto& c = std::get<Circle>(shapes[0]);
    EXPECT_DOUBLE_EQ(c.center_p.x, 0.0);
    EXPECT_DOUBLE_EQ(c.radius, 1.5);
}

TEST(ShapeUtilsTest, ParseLine) {
    auto shapes = ParseShapes("line 1 2 3 4");
    ASSERT_EQ(shapes.size(), 1u);
    EXPECT_TRUE(std::holds_alternative<Line>(shapes[0]));
}

TEST(ShapeUtilsTest, ParseTriangle) {
    auto shapes = ParseShapes("triangle 0 0 1 0 0.5 1");
    ASSERT_EQ(shapes.size(), 1u);
    EXPECT_TRUE(std::holds_alternative<Triangle>(shapes[0]));
}

TEST(ShapeUtilsTest, ParseRectangle) {
    auto shapes = ParseShapes("rectangle 0 0 5 3");
    ASSERT_EQ(shapes.size(), 1u);
    EXPECT_TRUE(std::holds_alternative<Rectangle>(shapes[0]));
    auto& r = std::get<Rectangle>(shapes[0]);
    EXPECT_DOUBLE_EQ(r.width, 5.0);
    EXPECT_DOUBLE_EQ(r.height, 3.0);
}

TEST(ShapeUtilsTest, ParsePolygon) {
    auto shapes = ParseShapes("polygon 0 0 2 5");
    ASSERT_EQ(shapes.size(), 1u);
    EXPECT_TRUE(std::holds_alternative<RegularPolygon>(shapes[0]));
    auto& p = std::get<RegularPolygon>(shapes[0]);
    EXPECT_EQ(p.sides, 5);
}

TEST(ShapeUtilsTest, ParseMultipleShapes) {
    auto shapes = ParseShapes("circle 0 0 1; line 1 2 3 4; triangle 0 0 1 0 0 1");
    EXPECT_EQ(shapes.size(), 3u);
}

TEST(ShapeUtilsTest, ParseInvalidShapeIgnored) {
    auto shapes = ParseShapes("badshape; circle 0 0 1");
    EXPECT_EQ(shapes.size(), 1u);
}

TEST(ShapeUtilsTest, ParseNegativeRadiusIgnored) {
    auto shapes = ParseShapes("circle 0 0 -1");
    EXPECT_EQ(shapes.size(), 0u);
}

TEST(ShapeUtilsTest, ParseRectangleNegativeDimIgnored) {
    auto shapes = ParseShapes("rectangle 0 0 -5 3");
    EXPECT_EQ(shapes.size(), 0u);
}

TEST(ShapeUtilsTest, ParsePolygonTooFewSidesIgnored) {
    auto shapes = ParseShapes("polygon 0 0 1 2");
    EXPECT_EQ(shapes.size(), 0u);
}

// === FindAllCollisions ===

TEST(ShapeUtilsTest, FindAllCollisionsOverlapping) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 5},
        Circle{{3, 0}, 5},
    };
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 1u);
}

TEST(ShapeUtilsTest, FindAllCollisionsNoOverlap) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 1},
        Circle{{100, 100}, 1},
    };
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0u);
}

TEST(ShapeUtilsTest, FindAllCollisionsMultiple) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 5},
        Circle{{3, 0}, 5},
        Circle{{6, 0}, 5},
    };
    auto collisions = FindAllCollisions(shapes);
    // All three circles overlap pairwise
    EXPECT_EQ(collisions.size(), 3u);
}

TEST(ShapeUtilsTest, FindAllCollisionsEmpty) {
    std::vector<Shape> shapes;
    auto collisions = FindAllCollisions(shapes);
    EXPECT_EQ(collisions.size(), 0u);
}

// === FindHighestShape ===

TEST(ShapeUtilsTest, FindHighestShape) {
    std::vector<Shape> shapes = {
        Circle{{0, 0}, 1},     // height = 1
        Circle{{0, 0}, 10},    // height = 10
        Circle{{0, 0}, 5},     // height = 5
    };
    auto idx = FindHighestShape(shapes);
    ASSERT_TRUE(idx.has_value());
    EXPECT_EQ(*idx, 1u);
}

TEST(ShapeUtilsTest, FindHighestShapeEmpty) {
    std::vector<Shape> shapes;
    auto idx = FindHighestShape(shapes);
    EXPECT_FALSE(idx.has_value());
}

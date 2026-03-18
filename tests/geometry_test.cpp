#include <gtest/gtest.h>
#include <format>
#include "geometry.hpp"

using namespace geometry;

// === Point2D ===

TEST(Point2DTest, DefaultConstructor) {
    constexpr Point2D p;
    EXPECT_DOUBLE_EQ(p.x, 0.0);
    EXPECT_DOUBLE_EQ(p.y, 0.0);
}

TEST(Point2DTest, ParameterizedConstructor) {
    constexpr Point2D p{3.0, 4.0};
    EXPECT_DOUBLE_EQ(p.x, 3.0);
    EXPECT_DOUBLE_EQ(p.y, 4.0);
}

TEST(Point2DTest, Addition) {
    constexpr Point2D a{1.0, 2.0};
    constexpr Point2D b{3.0, 4.0};
    constexpr auto c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 4.0);
    EXPECT_DOUBLE_EQ(c.y, 6.0);
}

TEST(Point2DTest, Subtraction) {
    constexpr Point2D a{5.0, 7.0};
    constexpr Point2D b{2.0, 3.0};
    constexpr auto c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 3.0);
    EXPECT_DOUBLE_EQ(c.y, 4.0);
}

TEST(Point2DTest, Multiply) {
    constexpr Point2D p{2.0, 3.0};
    constexpr auto r = p * 2.0;
    EXPECT_DOUBLE_EQ(r.x, 4.0);
    EXPECT_DOUBLE_EQ(r.y, 6.0);
}

TEST(Point2DTest, Divide) {
    constexpr Point2D p{6.0, 8.0};
    constexpr auto r = p / 2.0;
    EXPECT_DOUBLE_EQ(r.x, 3.0);
    EXPECT_DOUBLE_EQ(r.y, 4.0);
}

TEST(Point2DTest, Dot) {
    constexpr Point2D a{1.0, 2.0};
    constexpr Point2D b{3.0, 4.0};
    EXPECT_DOUBLE_EQ(a.Dot(b), 11.0);
}

TEST(Point2DTest, Cross) {
    constexpr Point2D a{1.0, 0.0};
    constexpr Point2D b{0.0, 1.0};
    EXPECT_DOUBLE_EQ(a.Cross(b), 1.0);
}

TEST(Point2DTest, Length) {
    constexpr Point2D p{3.0, 4.0};
    EXPECT_DOUBLE_EQ(p.Length(), 5.0);
}

TEST(Point2DTest, DistanceTo) {
    constexpr Point2D a{0.0, 0.0};
    constexpr Point2D b{3.0, 4.0};
    EXPECT_DOUBLE_EQ(a.DistanceTo(b), 5.0);
}

TEST(Point2DTest, Normalize) {
    constexpr Point2D p{3.0, 4.0};
    constexpr auto n = p.Normalize();
    EXPECT_NEAR(n.x, 0.6, 1e-10);
    EXPECT_NEAR(n.y, 0.8, 1e-10);
}

TEST(Point2DTest, Equality) {
    constexpr Point2D a{1.0, 2.0};
    constexpr Point2D b{1.0, 2.0};
    constexpr Point2D c{1.0, 3.0};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(Point2DTest, LessThan) {
    constexpr Point2D a{1.0, 2.0};
    constexpr Point2D b{3.0, 4.0};
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

// === Line ===

TEST(LineTest, Length) {
    constexpr Line l{{0, 0}, {3, 4}};
    EXPECT_DOUBLE_EQ(l.Length(), 5.0);
}

TEST(LineTest, BoundBox) {
    constexpr Line l{{1, 2}, {4, 6}};
    constexpr auto bb = l.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 1.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 6.0);
}

TEST(LineTest, Height) {
    constexpr Line l{{1, 2}, {4, 6}};
    EXPECT_DOUBLE_EQ(l.Height(), 6.0);
}

TEST(LineTest, Center) {
    constexpr Line l{{0, 0}, {4, 6}};
    constexpr auto c = l.Center();
    EXPECT_DOUBLE_EQ(c.x, 2.0);
    EXPECT_DOUBLE_EQ(c.y, 3.0);
}

TEST(LineTest, Vertices) {
    constexpr Line l{{1, 2}, {3, 4}};
    constexpr auto v = l.Vertices();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v[0].x, 1.0);
    EXPECT_DOUBLE_EQ(v[1].x, 3.0);
}

// === Triangle ===

TEST(TriangleTest, Area) {
    constexpr Triangle t{{0, 0}, {4, 0}, {0, 3}};
    EXPECT_DOUBLE_EQ(t.Area(), 6.0);
}

TEST(TriangleTest, BoundBox) {
    constexpr Triangle t{{0, 0}, {4, 0}, {2, 3}};
    constexpr auto bb = t.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 0.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 3.0);
}

TEST(TriangleTest, Height) {
    constexpr Triangle t{{0, 0}, {4, 0}, {2, 5}};
    EXPECT_DOUBLE_EQ(t.Height(), 5.0);
}

TEST(TriangleTest, Center) {
    constexpr Triangle t{{0, 0}, {3, 0}, {0, 3}};
    constexpr auto c = t.Center();
    EXPECT_DOUBLE_EQ(c.x, 1.0);
    EXPECT_DOUBLE_EQ(c.y, 1.0);
}

// === Rectangle ===

TEST(RectangleTest, BoundBox) {
    constexpr Rectangle r{{1, 2}, 3, 4};
    constexpr auto bb = r.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 1.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 6.0);
}

TEST(RectangleTest, Height) {
    constexpr Rectangle r{{0, 10}, 5, 3};
    EXPECT_DOUBLE_EQ(r.Height(), 13.0);
}

TEST(RectangleTest, TopRight) {
    constexpr Rectangle r{{1, 2}, 3, 4};
    constexpr auto tr = r.TopRight();
    EXPECT_DOUBLE_EQ(tr.x, 4.0);
    EXPECT_DOUBLE_EQ(tr.y, 6.0);
}

TEST(RectangleTest, Vertices) {
    constexpr Rectangle r{{0, 0}, 2, 3};
    constexpr auto v = r.Vertices();
    EXPECT_EQ(v.size(), 4u);
}

// === Circle ===

TEST(CircleTest, BoundBox) {
    constexpr Circle c{{5, 5}, 3};
    constexpr auto bb = c.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 2.0);
    EXPECT_DOUBLE_EQ(bb.min_y, 2.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 8.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 8.0);
}

TEST(CircleTest, Height) {
    constexpr Circle c{{5, 5}, 3};
    EXPECT_DOUBLE_EQ(c.Height(), 8.0);
}

TEST(CircleTest, Center) {
    constexpr Circle c{{5, 5}, 3};
    EXPECT_DOUBLE_EQ(c.Center().x, 5.0);
    EXPECT_DOUBLE_EQ(c.Center().y, 5.0);
}

// === RegularPolygon ===

TEST(RegularPolygonTest, BoundBox) {
    constexpr RegularPolygon p{{0, 0}, 5, 6};
    constexpr auto bb = p.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, -5.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 5.0);
}

TEST(RegularPolygonTest, Vertices) {
    RegularPolygon p{{0, 0}, 1, 4};
    auto v = p.Vertices();
    EXPECT_EQ(v.size(), 4u);
}

TEST(RegularPolygonTest, Height) {
    constexpr RegularPolygon p{{0, 0}, 5, 6};
    EXPECT_DOUBLE_EQ(p.Height(), 5.0);
}

// === Polygon ===

TEST(PolygonTest, BoundBox) {
    Polygon poly({{0, 0}, {4, 0}, {4, 3}, {0, 3}});
    auto bb = poly.BoundBox();
    EXPECT_DOUBLE_EQ(bb.min_x, 0.0);
    EXPECT_DOUBLE_EQ(bb.max_x, 4.0);
    EXPECT_DOUBLE_EQ(bb.max_y, 3.0);
}

TEST(PolygonTest, Height) {
    Polygon poly({{0, 0}, {4, 0}, {4, 5}, {0, 5}});
    EXPECT_DOUBLE_EQ(poly.Height(), 5.0);
}

TEST(PolygonTest, Center) {
    Polygon poly({{0, 0}, {4, 0}, {4, 4}, {0, 4}});
    auto c = poly.Center();
    EXPECT_DOUBLE_EQ(c.x, 2.0);
    EXPECT_DOUBLE_EQ(c.y, 2.0);
}

TEST(PolygonTest, Vertices) {
    Polygon poly({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    EXPECT_EQ(poly.Vertices().size(), 4u);
}

// === BoundingBox ===

TEST(BoundingBoxTest, Overlaps) {
    constexpr BoundingBox a{0, 0, 2, 2};
    constexpr BoundingBox b{1, 1, 3, 3};
    constexpr BoundingBox c{5, 5, 6, 6};
    EXPECT_TRUE(a.Overlaps(b));
    EXPECT_FALSE(a.Overlaps(c));
}

TEST(BoundingBoxTest, WidthHeight) {
    constexpr BoundingBox bb{1, 2, 4, 7};
    EXPECT_DOUBLE_EQ(bb.Width(), 3.0);
    EXPECT_DOUBLE_EQ(bb.Height(), 5.0);
}

// === Formatter ===

TEST(FormatterTest, Point2DFormat) {
    Point2D p{1.5, 2.5};
    auto s = std::format("{}", p);
    EXPECT_EQ(s, "(1.50, 2.50)");
}

TEST(FormatterTest, VectorPoint2DInline) {
    std::vector<Point2D> v = {{1, 2}, {3, 4}};
    auto s = std::format("{}", v);
    EXPECT_EQ(s, "(1.00, 2.00), (3.00, 4.00)");
}

TEST(FormatterTest, VectorPoint2DNewLine) {
    std::vector<Point2D> v = {{1, 2}};
    auto s = std::format("{:new_line}", v);
    EXPECT_EQ(s, "\t(1.00, 2.00)\n");
}

TEST(FormatterTest, LineFormat) {
    Line l{{1, 2}, {3, 4}};
    auto s = std::format("{}", l);
    EXPECT_EQ(s, "Line((1.00, 2.00), (3.00, 4.00))");
}

TEST(FormatterTest, CircleFormat) {
    Circle c{{0, 0}, 1.5};
    auto s = std::format("{}", c);
    EXPECT_EQ(s, "Circle(center=(0.00, 0.00), r=1.50)");
}

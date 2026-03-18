#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");

    auto supported = others | views::filter([](const Shape &s) {
        return std::holds_alternative<Line>(s) || std::holds_alternative<Circle>(s);
    });

    for (const auto &other : supported) {
        auto result = intersections::GetIntersectPoint(shape, other);
        result.and_then([&](const Point2D &p) -> std::optional<Point2D> {
            std::println("Intersection found at {} between shapes", p);
            return p;
        }).or_else([&]() -> std::optional<Point2D> {
            std::println("Shapes do not intersect");
            return std::nullopt;
        });
    }
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    auto selected = shapes | views::take(5);

    for (const auto &shape : selected) {
        double dist = queries::DistanceToPoint(shape, p);
        std::visit([&](const auto &s) {
            std::println("Distance from point {} to shape {} is {:.2f}", p, s, dist);
        }, shape);
    }
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Analysis ===");

    auto collisions = utils::FindAllCollisions(shapes);
    std::println("Found {} bounding box collisions:", collisions.size());
    for (const auto &[s1, s2] : collisions) {
        std::visit([](const auto &a, const auto &b) {
            std::println("  Collision: {} <-> {}", a, b);
        }, s1, s2);
    }

    auto highest = utils::FindHighestShape(shapes);
    if (highest.has_value()) {
        std::visit([&](const auto &s) {
            std::println("Highest shape [{}]: {}", *highest, s);
        }, shapes[*highest]);
    }

    if (shapes.size() >= 2) {
        auto dist = queries::DistanceBetweenShapes(shapes[0], shapes[1]);
        dist.and_then([](double d) -> std::optional<double> {
            std::println("Distance between first two shapes: {:.2f}", d);
            return d;
        }).or_else([]() -> std::optional<double> {
            std::println("Distance calculation not supported for this pair");
            return std::nullopt;
        });
    }
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    auto above_50 = shapes | views::filter([](const Shape &s) {
        return queries::GetHeight(s) > 50.0;
    }) | views::take(3);

    std::println("Shapes above height 50.0:");
    for (const auto &s : above_50) {
        std::visit([](const auto &shape) {
            std::println("  {}", shape);
        }, s);
    }

    if (!shapes.empty()) {
        auto [min_it, max_it] = rng::minmax_element(shapes, {}, [](const Shape &s) {
            return queries::GetHeight(s);
        });
        std::visit([](const auto &s) {
            std::println("Lowest shape: {}", s);
        }, *min_it);
        std::visit([](const auto &s) {
            std::println("Highest shape: {}", s);
        }, *max_it);
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту
    for (auto i : views::iota(0u, shapes.size())) {
        std::visit([&](const auto &s) {
            std::println("[{}] {} — height: {:.2f}", i, s, queries::GetHeight(shapes[i]));
        }, shapes[i]);
    }

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;

    for (const auto &shape : shapes) {
        std::visit(queries::Multilambda{
            [&](const Circle &c) {
                auto verts = c.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            },
            [&](const RegularPolygon &p) {
                auto verts = p.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            },
            [&](const Polygon &p) {
                auto verts = p.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            },
            [&](const auto &s) {
                auto verts = s.Vertices();
                points.insert(points.end(), verts.begin(), verts.end());
            }
        }, shape);
    }

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    auto hull_result = convex_hull::GrahamScan(points);
    if (hull_result.has_value()) {
        shapes.push_back(Polygon{hull_result.value()});
        std::println("Convex hull built with {} points", hull_result.value().size());
    } else {
        std::println("Convex hull error: {}", hull_result.error());
    }

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //
    geometry::visualization::Draw(shapes);

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //
        auto triangulation_result = triangulation::DelaunayTriangulation(points);
        if (triangulation_result.has_value()) {
            std::println("Triangulation produced {} triangles", triangulation_result.value().size());
            for (const auto &t : triangulation_result.value()) {
                std::println("  {}", t);
            }
            geometry::visualization::Draw(triangulation_result.value());
        } else {
            std::println("Triangulation error: {}", triangulation_result.error());
        }
    }
    return 0;
}
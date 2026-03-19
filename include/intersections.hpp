#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>
#include <stdexcept>

namespace geometry::intersections {

class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line &l1, const Line &l2) const {
        Point2D d1 = l1.end - l1.start;
        Point2D d2 = l2.end - l2.start;

        double cross = d1.Cross(d2);
        if (std::abs(cross) < 1e-10) {
            return std::nullopt;
        }

        Point2D diff = l2.start - l1.start;
        double t = diff.Cross(d2) / cross;
        double u = diff.Cross(d1) / cross;

        if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
            return l1.start + d1 * t;
        }
        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const {
        Point2D d = line.end - line.start;
        Point2D f = line.start - circle.center_p;

        double a = d.Dot(d);
        double b = 2.0 * f.Dot(d);
        double c = f.Dot(f) - circle.radius * circle.radius;

        double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0) {
            return std::nullopt;
        }

        double sqrt_disc = std::sqrt(discriminant);
        double t1 = (-b - sqrt_disc) / (2.0 * a);
        double t2 = (-b + sqrt_disc) / (2.0 * a);

        if (t1 >= 0.0 && t1 <= 1.0) {
            return line.start + d * t1;
        }
        if (t2 >= 0.0 && t2 <= 1.0) {
            return line.start + d * t2;
        }
        return std::nullopt;
    }

    std::optional<Point2D> operator()(const Circle &circle, const Line &line) const {
        return (*this)(line, circle);
    }

    std::optional<Point2D> operator()(const Circle &c1, const Circle &c2) const {
        double dist = c1.center_p.DistanceTo(c2.center_p);

        if (dist > c1.radius + c2.radius || dist < std::abs(c1.radius - c2.radius) || dist < 1e-10) {
            return std::nullopt;
        }

        double a = (c1.radius * c1.radius - c2.radius * c2.radius + dist * dist) / (2.0 * dist);
        Point2D dir = (c2.center_p - c1.center_p) / dist;
        return c1.center_p + dir * a;
    }

    template <typename T, typename U>
    std::optional<Point2D> operator()(const T &, const U &) const {
        throw std::logic_error("Intersection not supported for this combination of shapes");
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    return std::visit(IntersectionVisitor{}, shape1, shape2);
}

}  // namespace geometry::intersections
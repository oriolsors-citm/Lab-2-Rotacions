#pragma once
#include <cstdint>

struct Operations {
    std::uint64_t add = 0, subtract = 0, multiply = 0, divide = 0;
    std::uint64_t total() const { return add + subtract + multiply + divide; }
};

inline constexpr double Pi = 3.14159265358979323846;
inline constexpr double Tolerance = 1e-6;

struct Vec3 {
    double x = 0, y = 0, z = 0;
    static double Dot(const Vec3& a, const Vec3& b);
    static Vec3 Cross(const Vec3& a, const Vec3& b);
    double Norm() const;
    Vec3 Normalize() const;
};
bool isFinite(const Vec3& v);

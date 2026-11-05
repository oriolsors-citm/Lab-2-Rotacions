#pragma once
#include "Geometry.hpp"
#include <cstddef>

struct Matrix3x3 {

    double m[9] = {};
    static Matrix3x3 Identity();
    double& At(std::size_t row, std::size_t col);
    double At(std::size_t row, std::size_t col) const;
    Vec3 Multiply(const Vec3& v) const;
    Matrix3x3 Multiply(const Matrix3x3& matrix) const;
    double Det() const;
    Matrix3x3 Transposed() const;
    double Trace() const;

    bool IsRotation() const;

    static Matrix3x3 RotationAxisAngle(const Vec3& axis, double angle);

    void ToAxisAngle(Vec3& axis, double& angle) const;

    Vec3 Rotate(const Vec3& v, Operations& operations) const;

    static Matrix3x3 FromEulerZYX(double yaw, double pitch, double roll);

    void ToEulerZYX(double& yaw, double& pitch, double& roll) const;
};

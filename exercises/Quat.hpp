#pragma once
#include "Matrix3x3.hpp"

struct Quat {

    double s = 1, x = 0, y = 0, z = 0;
    Quat Normalized() const;

    Quat Multiply(const Quat& other, Operations& operations) const;

    Vec3 Rotate(const Vec3& v, Operations& operations) const;
    static Quat FromMatrix3x3(const Matrix3x3& matrix);
    Matrix3x3 ToMatrix3x3() const;
    static Quat FromAxisAngle(const Vec3& axis, double angle);

    void ToAxisAngle(Vec3& axis, double& angle) const;
};

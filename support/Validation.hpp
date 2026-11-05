#pragma once
#include "Quat.hpp"
#include <string>

enum class Task {
    IsRotation, AxisMatrix, AxisQuat, MatrixAxis, MatrixQuat, MatrixEuler,
    EulerMatrix, QuatProduct, QuatAxis, QuatMatrix, MatrixVector, QuatVector
};
struct Input {
    Vec3 axis{0,0,1}, vector{1,2,3};
    double angle = Pi/4, yaw = Pi/6, pitch = Pi/18, roll = -Pi/9;
    Matrix3x3 matrix = Matrix3x3::Identity();
    Quat first, second;
};
struct Report {
    Matrix3x3 matrix;
    Quat quat;
    Vec3 vector, axis;
    double angle = 0, yaw = 0, pitch = 0, roll = 0, milliseconds = 0, error = 0;
    bool rotation = false, correct = false, completed = false;
    bool countsApplicable = false, countsExpected = false;
    Operations operations;
    std::string detail;
};

Report runTask(Task task, const Input& input);
Input exampleInput(int example);

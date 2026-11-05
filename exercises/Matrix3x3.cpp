#include "Matrix3x3.hpp"
#include <stdexcept>

bool Matrix3x3::IsRotation() const {
	// TODO (exercici 1).
    throw std::logic_error("Pendent d'implementar: IsRotation");
}

Matrix3x3 Matrix3x3::RotationAxisAngle(const Vec3& , double ) {
    // TODO (exercici 1).
    throw std::logic_error("Pendent d'implementar: RotationAxisAngle");
}

Vec3 Matrix3x3::Rotate(const Vec3& , Operations& ) const {
    // TODO (exercici 1).
    throw std::logic_error("Pendent d'implementar: Rotate");
}

void Matrix3x3::ToAxisAngle(Vec3& , double& ) const {
    // TODO (exercici 1).
    throw std::logic_error("Pendent d'implementar: ToAxisAngle");
}

Matrix3x3 Matrix3x3::FromEulerZYX(double , double , double ) {
    // TODO (exercici 2).
    throw std::logic_error("Pendent d'implementar: FromEulerZYX");
}

void Matrix3x3::ToEulerZYX(double& , double& , double& ) const {
    // TODO (exercici 2).
    throw std::logic_error("Pendent d'implementar: ToEulerZYX");
}

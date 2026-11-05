#include "Quat.hpp"
#include <stdexcept>

Quat Quat::Multiply(const Quat& , Operations& ) const {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: Multiply");
}

Vec3 Quat::Rotate(const Vec3& , Operations& ) const {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: Rotate");
}

Quat Quat::FromAxisAngle(const Vec3& , double ) {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: FromAxisAngle");
}

Matrix3x3 Quat::ToMatrix3x3() const {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: ToMatrix3x3");
}

Quat Quat::FromMatrix3x3(const Matrix3x3& ) {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: FromMatrix3x3");
}

void Quat::ToAxisAngle(Vec3& , double& ) const {
    // TODO (exercici 3).
    throw std::logic_error("Pendent d'implementar: ToAxisAngle");
}

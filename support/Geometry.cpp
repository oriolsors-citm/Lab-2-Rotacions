#include "Geometry.hpp"
#include "Matrix3x3.hpp"
#include "Quat.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

bool isFinite(const Vec3& v) {
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}
double Vec3::Dot(const Vec3& a, const Vec3& b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
Vec3 Vec3::Cross(const Vec3& a, const Vec3& b) {
    return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
double Vec3::Norm() const { return std::hypot(x,y,z); }
Vec3 Vec3::Normalize() const {
    if (!isFinite(*this)) throw std::invalid_argument("Vector no finit");
    const double scale = std::max({std::abs(x),std::abs(y),std::abs(z)});
    if (scale == 0) throw std::invalid_argument("No es pot normalitzar el vector nul");
    const Vec3 v{x/scale,y/scale,z/scale};
    const double n = v.Norm();
    return {v.x/n,v.y/n,v.z/n};
}
Quat Quat::Normalized() const {
    if (!std::isfinite(s) || !isFinite({x,y,z}))
        throw std::invalid_argument("Quaternió no finit");
    const double scale = std::max({std::abs(s),std::abs(x),std::abs(y),std::abs(z)});
    if (scale == 0) throw std::invalid_argument("No es pot normalitzar el quaternió nul");
    Quat q{s/scale,x/scale,y/scale,z/scale};
    const double n = std::sqrt(q.s*q.s+q.x*q.x+q.y*q.y+q.z*q.z);
    return {q.s/n,q.x/n,q.y/n,q.z/n};
}
double& Matrix3x3::At(std::size_t row, std::size_t col) {
    if (row >= 3 || col >= 3) throw std::out_of_range("Índex de matriu fora de rang");
    return m[row*3+col];
}
double Matrix3x3::At(std::size_t row, std::size_t col) const {
    if (row >= 3 || col >= 3) throw std::out_of_range("Índex de matriu fora de rang");
    return m[row*3+col];
}
Matrix3x3 Matrix3x3::Identity() { return {{1,0,0,0,1,0,0,0,1}}; }
Vec3 Matrix3x3::Multiply(const Vec3& v) const {
    return {m[0]*v.x+m[1]*v.y+m[2]*v.z,
            m[3]*v.x+m[4]*v.y+m[5]*v.z,
            m[6]*v.x+m[7]*v.y+m[8]*v.z};
}
Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& b) const {
    Matrix3x3 c;
    for (std::size_t i=0;i<3;++i)
        for (std::size_t j=0;j<3;++j)
            for (std::size_t k=0;k<3;++k) c.At(i,j)+=At(i,k)*b.At(k,j);
    return c;
}
double Matrix3x3::Det() const {
    return m[0]*(m[4]*m[8]-m[5]*m[7])-m[1]*(m[3]*m[8]-m[5]*m[6])
        +m[2]*(m[3]*m[7]-m[4]*m[6]);
}
Matrix3x3 Matrix3x3::Transposed() const {
    return {{m[0],m[3],m[6],m[1],m[4],m[7],m[2],m[5],m[8]}};
}
double Matrix3x3::Trace() const { return m[0]+m[4]+m[8]; }

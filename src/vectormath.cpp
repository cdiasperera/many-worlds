#include "vectormath.h"

QPair<QVector3D, QVector3D> VectorMath::orthogonalVectors(QVector3D const &v) {
    QVector3D u = (std::abs(v.z()) > std::numeric_limits<float>::epsilon()) ? QVector3D(1, 0, 0) : QVector3D(0, 0, 1);
    QVector3D a = QVector3D::crossProduct(v, u).normalized();
    QVector3D b = QVector3D::crossProduct(v, a).normalized();
    return qMakePair(a, b);
}

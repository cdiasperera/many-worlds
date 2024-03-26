#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <QVector3D>


class VectorMath
{
public:
    static QPair<QVector3D, QVector3D> orthogonalVectors(QVector3D const &v);
};

#endif // VECTORMATH_H

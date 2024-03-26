#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

/**
 * @brief Represents a single vertex with coordinates and an rgb color.
 */
struct Vertex {
    QVector3D coords;
    QVector3D normal;
    QVector2D textureCoords;

    Vertex() = default;
    Vertex(QVector3D coords, QVector3D normal) : coords(coords), normal(normal) {}
};

#endif  // VERTEX_H

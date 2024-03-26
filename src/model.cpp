#include "model.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtCore/qlogging.h>
#include <tuple>

/**
 * @brief Model::Model Constructs a new model from a Wavefront .obj file.
 * @param filename The filename. Should be a .obj file
 */
Model::Model(const QString& filename) {
    qDebug() << ":: Loading model:" << filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        QString line;
        QStringList tokens;

        while (!in.atEnd()) {
            line = in.readLine();
            if (line.startsWith("#")) continue;  // skip comments

            tokens = line.split(" ", Qt::SkipEmptyParts);

            // Switch depending on first element
            if (tokens[0] == "v") {
                parseVertex(tokens);
            }

            if (tokens[0] == "vn") {
                parseNormal(tokens);
            }

            if (tokens[0] == "vt") {
                parseTexture(tokens);
            }

            if (tokens[0] == "f") {
                parseFace(tokens);
            }
        }

        file.close();

        // create an array version of the data
        unpackIndexes();

        // Allign all vertex indices with the right normal/texturecoord indices
        alignData();
    }
}

/**
 * @brief Model::parseVertex Parses the coordinates of a vertex from the
 * .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseVertex(QStringList tokens) {
    float x = tokens[1].toFloat();
    float y = tokens[2].toFloat();
    float z = tokens[3].toFloat();
    vertices_indexed.append(QVector3D(x, y, z));
}

/**
 * @brief Model::parseNormal Parses the normals of a vertex from the
 * .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseNormal(QStringList tokens) {
    hNorms = true;
    float x = tokens[1].toFloat();
    float y = tokens[2].toFloat();
    float z = tokens[3].toFloat();
    norm.append(QVector3D(x, y, z));
}

/**
 * @brief Model::parseTexture Parses a texture coordinate from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseTexture(QStringList tokens) {
    hTexs = true;
    float u = tokens[1].toFloat();
    float v = tokens[2].toFloat();
    tex.append(QVector2D(u, v));
}

/**
 * @brief Model::parseFace Parses a face from the .obj file.
 * @param tokens Tokens on the line in question.
 */
void Model::parseFace(QStringList tokens) {
    QStringList elements;

    for (int i = 1; i != tokens.size(); ++i) {
        elements = tokens[i].split("/");
        // -1 since .obj count from 1
        indices.append(elements[0].toInt() - 1);

        if (elements.size() > 1 && !elements[1].isEmpty()) {
            texcoord_indices.append(elements[1].toInt() - 1);
        }

        if (elements.size() > 2 && !elements[2].isEmpty()) {
            normal_indices.append(elements[2].toInt() - 1);
        }
    }
}

/**
 * @brief Model::alignData
 *
 * Make sure that the indices from the vertices align with those
 * of the normals and the texture coordinates, create extra vertices
 * if vertex has multiple normals or texturecoords
 */
void Model::alignData() {
    QVector<QVector3D> verts;
    verts.reserve(vertices_indexed.size());
    QVector<QVector3D> norms;
    norms.reserve(vertices_indexed.size());
    QVector<QVector2D> texcs;
    texcs.reserve(vertices_indexed.size());
    QVector<std::tuple<QVector3D, QVector3D, QVector2D>> vs;

    QVector<unsigned> ind;
    ind.reserve(indices.size());

    unsigned currentIndex = 0;

    for (int i = 0; i != indices.size(); ++i) {
        QVector3D v = vertices_indexed[indices[i]];

        QVector3D n(0, 0, 0);
        if (hNorms) {
            n = norm[normal_indices[i]];
        }

        QVector2D t(0, 0);
        if (hTexs) {
            t = tex[texcoord_indices[i]];
        }

        std::tuple<QVector3D, QVector3D, QVector2D> k(v, n, t);
        if (vs.contains(k)) {
            // Vertex already exists, use that index
            ind.append(vs.indexOf(k));
        } else {
            // Create a new vertex
            verts.append(v);
            norms.append(n);
            texcs.append(t);
            vs.append(k);
            ind.append(currentIndex);
            ++currentIndex;
        }
    }
    // Remove old data
    vertices_indexed.clear();
    normals_indexed.clear();
    textureCoords_indexed.clear();
    indices.clear();

    // Set the new data
    vertices_indexed = verts;
    normals_indexed = norms;
    textureCoords_indexed = texcs;
    indices = ind;
}

/**
 * @brief Model::unpackIndexes Unpack indices so that they are available for
 * glDrawArrays()
 *
 */
void Model::unpackIndexes() {
    vertices.clear();
    normals.clear();
    textureCoords.clear();
    for (int i = 0; i != indices.size(); ++i) {
        vertices.append(vertices_indexed[indices[i]]);

        if (hNorms) {
            normals.append(norm[normal_indices[i]]);
        }

        if (hTexs) {
            textureCoords.append(tex[texcoord_indices[i]]);
        }
    }
}

/**
 * @brief Model::unitize Unitizes the model by scaling so that it fits a box
 * with sides 1 and origin at 0,0,0.Useful for models with different scales. Not
 * Implemented yet!
 *
 */
void Model::unitize() { qDebug() << "Implement this yourself (optional)"; }

/**
 * @brief Model::getCoords Get all coordinates in the mesh. The coordinates are
 * ordered in such a way that they can be used in glDrawArrays. For a mesh
 * consisting of two triangles, this might look as follows: c1, c2, c3, c2,
 * c3, c4, etc.
 * @return The coordinates in the mesh.
 */
QVector<QVector3D> Model::getCoords() { return vertices; }

/**
 * @brief Model::getCoords Get all normals in the mesh. The normals are
 * ordered in such a way that they can be used in glDrawArrays. For a mesh
 * consisting of two triangles, this might look as follows: n1, n2, n3, n2, n3,
 * n4, etc.
 * @return The normals in the mesh.
 */
QVector<QVector3D> Model::getNormals() { return normals; }

/**
 * @brief Model::getCoords Get all texture coordinates in the mesh. The texture
 * coordinates are ordered in such a way that they can be used in glDrawArrays.
 * For a mesh consisting of two triangles, this might look as follows: tx1, tx2,
 * tx3, tx2, tx3, tx4, etc.
 * @return The texture coordinates in the mesh.
 */
QVector<QVector2D> Model::getTextureCoords() { return textureCoords; }

/**
 * @brief Model::getCoords Get all unique coordinates in the mesh. The
 * coordinates are ordered in such a way that they can be used in
 * glDrawElements. For a mesh consisting of two triangles, this might look as
 * follows: c1, c2, c3, c4. Intended to be used in combination with
 * getIndices().
 * @return The unique coordinates in the mesh.
 */
QVector<QVector3D> Model::getCoordsIndexed() { return vertices_indexed; }

/**
 * @brief Model::getCoords Get all unique normals in the mesh. The
 * normals are ordered in such a way that they can be used in
 * glDrawElements. For a mesh consisting of two triangles, this might look as
 * follows: n1, n2, n3, n4. Intended to be used in combination with
 * getIndices().
 * @return The unique normals in the mesh.
 */
QVector<QVector3D> Model::getNormalsIndexed() { return normals_indexed; }

/**
 * @brief Model::getCoords Get all unique texture coordinates in the mesh. The
 * texture coordinates are ordered in such a way that they can be used in
 * glDrawElements. For a mesh consisting of two triangles, this might look as
 * follows: tx1, tx2, tx3, tx4. Intended to be used in combination with
 * getIndices().
 * @return The unique texture coordinates in the mesh.
 */
QVector<QVector2D> Model::getTextureCoordsIndexed() {
    return textureCoords_indexed;
}

/**
 * @brief Model::getIndices Retrieves a list of indices in such a way that these
 * describe the topology of the mesh. In other words, these indices describe how
 * the given (unique) coordinates/normals/texture coordinates make up all the
 * different triangles in the mesh. Intended to be used with glDrawElements.
 * @return A list of indices.
 */
QVector<unsigned> Model::getIndices() { return indices; }

/**
 * @brief Model::getVNInterleaved Retrieves the coordinates and normals of a
 * mesh as separate float values so that all data can be handled by a single
 * buffer. The values are ordered in such a way that they can be used in
 * glDrawArrays. Each vertex consists of 6 float values (3 coordinate, 3
 * normal).
 * @return A list of float values containing the coordinates and normals.
 */
QVector<float> Model::getVNInterleaved() {
    QVector<float> buffer;

    for (int i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
    }

    return buffer;
}

/**
 * @brief Model::getVNTInterleaved Retrieves the coordinates, normals and
 * texture coordinates of a mesh as separate float values  so that all data can
 * be handled by a single buffer. The values are ordered in such a way that they
 * can be used in glDrawArrays. Each vertex consists of 8 float values (3
 * coordinate, 3 normal, 2 texture coordinate).
 * @return A list of float values containing the coordinates and normals.
 */
QVector<float> Model::getVNTInterleaved() {
    QVector<float> buffer;

    for (int i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        QVector2D uv = textureCoords.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
        buffer.append(uv.x());
        buffer.append(uv.y());
    }

    return buffer;
}

/**
 * @brief Model::getVNInterleavedIndexed Retrieves the coordinates and normals
 * of a mesh as separate float values  so that all data can be handled by a
 * single buffer. The values are ordered in such a way that they can be used in
 * glDrawElements. Each vertex consists of 6 float values (3 coordinate, 3
 * normal).
 * @return A list of float values containing the coordinates and normals.
 */
QVector<float> Model::getVNInterleavedIndexed() {
    QVector<float> buffer;

    for (int i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
    }

    return buffer;
}

/**
 * @brief Model::getVNTInterleavedIndexed Retrieves the coordinates, normals and
 * texture coordinates of a mesh as separate float values  so that all data can
 * be handled by a single buffer. The values are ordered in such a way that they
 * can be used in glDrawElements. Each vertex consists of 8 float values (3
 * coordinate, 3 normal, 2 texture coordinate).
 * @return A list of float values containing the coordinates and normals.
 */
QVector<float> Model::getVNTInterleavedIndexed() {
    QVector<float> buffer;

    for (int i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        QVector2D uv = textureCoords_indexed.at(i);
        buffer.append(vertex.x());
        buffer.append(vertex.y());
        buffer.append(vertex.z());
        buffer.append(normal.x());
        buffer.append(normal.y());
        buffer.append(normal.z());
        buffer.append(uv.x());
        buffer.append(uv.y());
    }

    return buffer;
}

/**
 * @brief Model::getNumTriangles Retrieves the number of triangles in this mesh.
 * @return The number of triangles in this mesh.
 */
int Model::getNumTriangles() { return vertices.size() / 3; }

QVector<QVector3D> Model::getRandomColors() {
    auto size = vertices.size();

    QVector<QVector3D> colors(size);

    for (int i = 0; i < size; ++i) {
        colors[i] = QVector3D{
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
        };
    }

    return colors;
}

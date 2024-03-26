#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QStringList>
#include <QVector2D>
#include <QVector3D>
#include <QVector>

/**
 * @brief A simple Model class. Represents a 3D triangle mesh and is able to
 * load this data from a Wavefront .obj file. IMPORTANT: Current only supports
 * TRIANGLE meshes!
 *
 * Support for other meshes can be implemented by students.
 *
 */
class Model {
public:
    Model(const QString& filename);

    // Used for glDrawArrays()
    QVector<QVector3D> getCoords();
    QVector<QVector3D> getNormals();
    QVector<QVector2D> getTextureCoords();
    QVector<QVector3D> getRandomColors();

    // Used for interleaving into one buffer for glDrawArrays()
    QVector<float> getVNInterleaved();
    QVector<float> getVNTInterleaved();

    // Used for glDrawElements()
    QVector<QVector3D> getCoordsIndexed();
    QVector<QVector3D> getNormalsIndexed();
    QVector<QVector2D> getTextureCoordsIndexed();
    QVector<unsigned> getIndices();

    // Used for interleaving into one buffer for glDrawElements()
    QVector<float> getVNInterleavedIndexed();
    QVector<float> getVNTInterleavedIndexed();

    bool hasNormals();
    bool hasTextureCoords();
    int getNumTriangles();

    void unitize();

private:
    // OBJ parsing
    void parseVertex(QStringList tokens);
    void parseNormal(QStringList tokens);
    void parseTexture(QStringList tokens);
    void parseFace(QStringList tokens);

    // Alignment of data
    void alignData();
    void unpackIndexes();

    // Intermediate storage of values
    QVector<QVector3D> vertices_indexed;
    QVector<QVector3D> normals_indexed;
    QVector<QVector2D> textureCoords_indexed;
    QVector<unsigned> indices;

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    // Utility storage
    QVector<unsigned> normal_indices;
    QVector<unsigned> texcoord_indices;
    QVector<QVector3D> norm;
    QVector<QVector2D> tex;

    bool hNorms = false;
    bool hTexs = false;
};

#endif  // MODEL_H

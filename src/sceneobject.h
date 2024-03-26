#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <GL/gl.h>

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>

/**
 * An object in the scene
 */
class SceneObject
{
public:
    GLuint vao;
    GLuint positionVBO;
    GLuint normalVBO;
    GLuint textureVBO;
    GLuint size;

    // Position in the scene
    QVector3D position;

    // The model to world transofrm
    QMatrix4x4 modelTransform;

    SceneObject() = default;
    SceneObject(QVector3D position);

};

#endif // SCENEOBJECT_H

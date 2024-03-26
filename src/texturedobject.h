#ifndef TEXTUREDOBJECT_H
#define TEXTUREDOBJECT_H

#include "sceneobject.h"


struct TexturedObject : public SceneObject
{
    GLuint texture;
    TexturedObject(QVector3D position);
};

#endif // TEXTUREDOBJECT_H

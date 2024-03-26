#ifndef PORTALOBJECT_H
#define PORTALOBJECT_H

#include <QOpenGLShaderProgram>

#include "qmatrix4x4.h"
#include "sceneobject.h"
#include "ShaderType.h"



struct PortalObject : public SceneObject
{
    enum class COLLISION_STATE {
        FRONT,
        BACK,
        NO_COLLISION
    };

    QMatrix4x4 effectTransform;
    COLLISION_STATE collisionState;

     // The shader to use inside this world
    ShaderType shaderType;

    PortalObject() = default;
    PortalObject(QVector3D position, QMatrix4x4 effect, ShaderType shaderType = ShaderType::PHONG);
};

QDebug operator<<(QDebug dbg, PortalObject::COLLISION_STATE state);

#endif // PORTALOBJECT_H

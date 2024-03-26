#include "portalobject.h"

PortalObject::PortalObject(QVector3D position, QMatrix4x4 effect, ShaderType shaderType)
    : SceneObject{position}, effectTransform{effect}, shaderType{shaderType} {}

QDebug operator<<(QDebug dbg, PortalObject::COLLISION_STATE state) {
    switch (state) {
    case PortalObject::COLLISION_STATE::FRONT: return dbg << "FRONT";
    case PortalObject::COLLISION_STATE::BACK: return dbg << "BACK";
    case PortalObject::COLLISION_STATE::NO_COLLISION: return dbg << "NO_COLLISION";
    default: return dbg;
    }
}

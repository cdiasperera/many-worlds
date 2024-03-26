#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "texturedobject.h"
#include "portalobject.h"

struct Scene
{
    std::vector<TexturedObject> texturedObjects;
    std::vector<PortalObject> portalObjects;
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "texturedobject.h"
#include "portalobject.h"

struct Scene
{
    std::vector<TexturedObject> texturedObjects;
    std::vector<PortalObject> portalObjects;

    static Scene createScene0();
    static Scene createScene1();
    static Scene createScene2();
    static Scene createScene3();
};

#endif // SCENE_H

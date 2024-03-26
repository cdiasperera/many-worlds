#include "scene.h"

Scene Scene::createScene0() {
    Scene scene;
    scene.texturedObjects.emplace_back(QVector3D{0,0,-10});

    // Portal 1 - Simple scaling
    QMatrix4x4 portal1Effect;
    portal1Effect.setToIdentity();
    portal1Effect.scale(2);
    scene.portalObjects.push_back({
        QVector3D{0,0,0},
        portal1Effect
    });

    return scene;

}

Scene Scene::createScene1() {
    Scene scene;
    scene.texturedObjects.emplace_back(QVector3D{0,0,-10});

    // Portal 2 - Complex affine
    QMatrix4x4 portal2Effect;
    portal2Effect.setToIdentity();
    portal2Effect.rotate(45,{1,1,0});
    portal2Effect.scale(3,2,1);
    scene.portalObjects.push_back({
        QVector3D{0,0,0},
        portal2Effect
    });

    return scene;

}

Scene Scene::createScene2() {
    Scene scene;
    scene.texturedObjects.emplace_back(QVector3D{0,0,-10});

    // Portal 3 - Normal Map
    QMatrix4x4 portal3Effect;
    portal3Effect.setToIdentity();
    scene.portalObjects.push_back({
                                 QVector3D{0,0,0},
                                 portal3Effect,
                                 ShaderType::NORMAL
    });

    return scene;

}

Scene Scene::createScene3() {
    Scene scene;
    scene.texturedObjects.emplace_back(QVector3D{0,0,-10});
    scene.texturedObjects.emplace_back(QVector3D{10,0,-10});
    scene.texturedObjects.emplace_back(QVector3D{-10,0,-10});

    // Portal 1 - Simple scaling
    QMatrix4x4 portal1Effect;
    portal1Effect.setToIdentity();
    portal1Effect.scale(3);
    scene.portalObjects.push_back({
        QVector3D{0,0,0},
        portal1Effect
    });

    // Portal 2 - Complex affine
    QMatrix4x4 portal2Effect;
    portal2Effect.setToIdentity();
    portal2Effect.scale(.5);
    scene.portalObjects.push_back({
        QVector3D{10,0,0},
        portal2Effect
    });

    // Portal 3 - Normal Map
    QMatrix4x4 portal3Effect;
    portal3Effect.setToIdentity();
    scene.portalObjects.push_back({
                                 QVector3D{-10,0,0},
                                 portal3Effect,
                                 ShaderType::NORMAL
    });

    return scene;
}


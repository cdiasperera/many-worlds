#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <KeyboardStatus.h>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector3D>

#include "camera.h"
#include "sceneobject.h"
#include "texturedobject.h"
#include "portalobject.h"
#include "scene.h"
#include "shadertype.h"

/**
 * @brief The MainView class is resonsible for the actual content of the main
 * window.
 */
class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    // The FPS to run at
    int const FPS =  60;

public:
    MainView(QWidget *parent = nullptr);
    ~MainView() override;

protected:
    void initializeGL() override;
    void resizeGL(int newWidth, int newHeight) override;
    void paintGL() override;

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;

    // Function for mouse input events
    void mousePressEvent(QMouseEvent *ev) override;

private slots:
    void onMessageLogged(QOpenGLDebugMessage Message);

private:
    void generateTexture(QString const &fileName, TexturedObject &tso);
    void createShaderProgram(
        QOpenGLShaderProgram &shader,
        QString const &verShaderFile,
        QString const &objectFragShaderFile
        );
    void loadIntoSceneObject(QString const &fileName, SceneObject &so);
    void loadIntoSceneObject(
        QString const &fileName, TexturedObject &tso, QString const &textureName);
    void loadMesh(const QString &filename);
    void cleanUpSceneObject(SceneObject &so);
    void destroyModelBuffers();
    void updateCameraPosition();
    void updateProjectionTransform();
    void updateModelTransforms(SceneObject &so);
    void loadPortal(const QString &filename);
    void paintPortal(PortalObject &po, bool renderBorder);
    void setPortalStencil(PortalObject &po, int stencilVal);
    void paintScene();
    void paintMeshWithPortalEffectAtStencil(TexturedObject &to, PortalObject &po);
    void updatePortalEffectTransforms(PortalObject &po);
    PortalObject::COLLISION_STATE getPortalCollision(PortalObject &po);

    QVector<quint8> imageToBytes(const QImage &image);

    QOpenGLDebugLogger debugLogger;
    QTimer timer;  // timer used for animation

    std::unordered_map<ShaderType, QOpenGLShaderProgram> shaders;

    // User Input
    KeyboardStatus keyboardStatus;
    Camera camera;
    QMatrix4x4 projectionTransform;

    // Scenes
    Scene scene;

    bool inPortal = false;

    // Rendering transformations
    QMatrix4x4 currentWorldEffectTransform;
    ShaderType currentShaderType = ShaderType::PHONG;

};

#endif  // MAINVIEW_H

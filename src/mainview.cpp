#include "mainview.h"
#include <QDateTime>
#include <algorithm>

#include "model.h"

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    setFocus();

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(1000 / FPS);
}

MainView::~MainView() {
  qDebug() << "MainView destructor";

  makeCurrent();

  destroyModelBuffers();
}

// --- OpenGL initialization

void MainView::initializeGL() {
  qDebug() << ":: Initializing OpenGL";
  initializeOpenGLFunctions();

  connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
          SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

  if (debugLogger.initialize()) {
    qDebug() << ":: Logging initialized";
    debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
  }

  QString glVersion{reinterpret_cast<const char *>(glGetString(GL_VERSION))};
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);

  // Enable backface culling
  glEnable(GL_CULL_FACE);

  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  // Set the color to be used by glClear. This is, effectively, the background
  // color.
  glClearColor(0.37f, 0.42f, 0.45f, 0.0f);

  createShaderProgram(shaders[ShaderType::PHONG], ":/shaders/vertshader.glsl", ":/shaders/fragshader.glsl");
  createShaderProgram(shaders[ShaderType::NORMAL], ":/shaders/normalvertshader.glsl", ":/shaders/normalfragshader.glsl");
  createShaderProgram(shaders[ShaderType::PORTAL], ":/shaders/vertshader.glsl", ":/shaders/portalfragshader.glsl");
  for (auto &to : currentScene.texturedObjects) {
      loadIntoSceneObject(":/models/cat.obj", to, ":/textures/cat_diff.png");
  }

  for (auto &po : currentScene.portalObjects) {
      loadIntoSceneObject(":/models/portal.obj", po);
  }

  // Initialize transformations
  updateProjectionTransform();

  for (auto &to : currentScene.texturedObjects) {
      updateModelTransforms(to);
  }

  for (auto &po : currentScene.portalObjects) {
      updateModelTransforms(po);
  }

  // Initialize current world transform
  currentWorldEffectTransform.setToIdentity();
}

void MainView::createShaderProgram(
    QOpenGLShaderProgram &shader,
    QString const &vertShaderFile,
    QString const &objectFragShaderFile
    ) {
  // Create shader program
  shader.addShaderFromSourceFile(QOpenGLShader::Vertex, vertShaderFile);
  shader.addShaderFromSourceFile(QOpenGLShader::Fragment, objectFragShaderFile);
  shader.link();
}

// --- OpenGL drawing

void MainView::paintScene() {
    QOpenGLShaderProgram &shaderProgram = shaders[currentShaderType];
    shaderProgram.bind();

    // Scene constants
    shaderProgram.setUniformValue("materialColor", QVector3D{1, 1, 1});
    shaderProgram.setUniformValue("ka", 0.4F);
    shaderProgram.setUniformValue("kd", 0.4F);
    shaderProgram.setUniformValue("ks", 0.4F);
    shaderProgram.setUniformValue("p", 8);
    shaderProgram.setUniformValue("lightCoordinates", QVector3D{100, 50, 0});
    shaderProgram.setUniformValue("lightColor", QVector3D{1, 1, 1});

    // Transformation Constants

    for (auto &to : currentScene.texturedObjects) {
        QMatrix4x4 meshWithNoEffectTransform = to.modelTransform * currentWorldEffectTransform;
        shaderProgram.setUniformValue("projectionTransform", projectionTransform);
        shaderProgram.setUniformValue("modelViewTransform", meshWithNoEffectTransform);
        shaderProgram.setUniformValue("normalMatrix", meshWithNoEffectTransform.normalMatrix());

        shaderProgram.setUniformValue("sampler", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, to.texture);

        glBindVertexArray(to.vao);
        glDrawArrays(GL_TRIANGLES, 0, to.size);
    }

    shaderProgram.release();
}


void MainView::paintGL() {
  updateCameraPosition();

  // Clear the screen before rendering
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Enable stencil testing
  glEnable(GL_STENCIL_TEST);
  // Set stencil buffer to all 0s
  glClear(GL_STENCIL_BUFFER_BIT);

  int stencilVal = 1;

  for (auto &po : currentScene.portalObjects) {
      setPortalStencil(po, stencilVal);
      for (auto &to : currentScene.texturedObjects) {
          glStencilFunc(GL_EQUAL, stencilVal, 0xFF);
          paintMeshWithPortalEffectAtStencil(to, po);
      }
      // Only draw where stencil buffer is 0
      glStencilFunc(GL_EQUAL, 0, 0xFF);
      paintPortal(po, true);
      ++stencilVal;
  }

  paintScene();

  glDisable(GL_STENCIL_TEST);
}

void MainView::resizeGL(int newWidth, int newHeight) {
  Q_UNUSED(newWidth)
  Q_UNUSED(newHeight)
  updateProjectionTransform();
}

void MainView::updateCameraPosition() {
    camera.update(keyboardStatus);

    for (auto &po : currentScene.portalObjects) {
        updatePortalEffectTransforms(po);
        updateModelTransforms(po);
    }


    for (auto &to : currentScene.texturedObjects) {
        updateModelTransforms(to);
    }

    updateProjectionTransform();
}

void MainView::updateProjectionTransform() {
  float aspectRatio =
      static_cast<float>(width()) / static_cast<float>(height());
  projectionTransform.setToIdentity();
  projectionTransform.perspective(60.0F, aspectRatio, 0.2F, 40.0F);

  projectionTransform = projectionTransform * camera.getProjectionTransform();
}

void MainView::destroyModelBuffers() {
    for (auto &to : currentScene.texturedObjects) {
        cleanUpSceneObject(to);
        glDeleteTextures(1, &to.texture);
    }

    for (auto &po : currentScene.portalObjects) {
        cleanUpSceneObject(po);
    }

}

void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}

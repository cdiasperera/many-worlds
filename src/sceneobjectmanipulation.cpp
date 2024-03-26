#include "mainview.h"

#include "model.h"

void MainView::loadIntoSceneObject(QString const &fileName, SceneObject &so) {
  Model model(fileName);
  QVector<QVector3D> meshCoords = model.getCoords();
  QVector<QVector3D> meshNormals = model.getNormals();
  QVector<QVector2D> meshTextureCoords = model.getTextureCoords();

  so.size = meshCoords.size();

  // Generate VAO
  glGenVertexArrays(1, &so.vao);
  glBindVertexArray(so.vao);

  // Generate VBO
  glGenBuffers(1, &so.positionVBO);
  glGenBuffers(1, &so.normalVBO);
  glGenBuffers(1, &so.textureVBO);

  // Copy the vertex coordinates into the position VBO
  glBindBuffer(GL_ARRAY_BUFFER, so.positionVBO);
  glBufferData(GL_ARRAY_BUFFER, meshCoords.size() * sizeof(QVector3D),
               meshCoords.data(), GL_STATIC_DRAW);

  // Set vertex coordinates to location 0
  // Note: glVertexAttribPointer implicitly reference the VBO currently bound to
  // GL_ARRAY_BUFFER
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D),
                        reinterpret_cast<GLvoid *>(0));
  glEnableVertexAttribArray(0);

  // Copy the normals into the normal VBO
  glBindBuffer(GL_ARRAY_BUFFER, so.normalVBO);
  glBufferData(GL_ARRAY_BUFFER, meshNormals.size() * sizeof(QVector3D),
               meshNormals.data(), GL_STATIC_DRAW);

  // Set normals values to location 1
  // Note: glVertexAttribPointer implicitly reference the VBO currently bound to
  // GL_ARRAY_BUFFER
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D),
                        reinterpret_cast<GLvoid *>(0));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, so.textureVBO);
  glBufferData(GL_ARRAY_BUFFER, meshTextureCoords.size() * sizeof(QVector2D),
               meshTextureCoords.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D),
                        reinterpret_cast<GLvoid *>(0));
  glEnableVertexAttribArray(2);

  // Generally good practice to unbind the buffers to prevent anything after
  // this from accidentally modifying it.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

void MainView::loadIntoSceneObject(
    QString const &fileName, TexturedObject &tso,
    QString const &textureFile
) {
    loadIntoSceneObject(fileName, static_cast<SceneObject &>(tso));
    generateTexture(textureFile, tso);
}

void MainView::generateTexture(QString const &fileName, TexturedObject &tso) {
  QImage textureImage{fileName};
  // Texture image data
  QVector<quint8> textureData = imageToBytes(textureImage);

  // Generate Texture
  glGenTextures(1, &tso.texture);
  glBindTexture(GL_TEXTURE_2D, tso.texture);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload texture data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureImage.width(),
               textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               textureData.data());
}

void MainView::setPortalStencil(PortalObject &po, int stencilVal) {
  // TODO: There is an error here, objects that are in front of the portal will also be transformed as if they were behind portal.

  // Set the stencil test to always pass
  glStencilFunc(GL_ALWAYS, stencilVal, 0xFF);

  // Increment stencil buffer value for every pixel that passes the stencil test
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // Disable writing to the depth and colour buffer (the actual screen)
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  paintPortal(po, false);

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void MainView::paintMeshWithPortalEffectAtStencil(TexturedObject &to, PortalObject &po) {
    // If we are in a portal world, make sure to set portal door to render the default world instead
    QOpenGLShaderProgram &shaderProgram = inPortal ? shaders[ShaderType::PHONG] : shaders[po.shaderType];
    QMatrix4x4 portalEffect = po.effectTransform;
    if (inPortal) {
        portalEffect.setToIdentity();
    }

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
    QMatrix4x4 meshWithEffectTransform = to.modelTransform * portalEffect;
    shaderProgram.setUniformValue("projectionTransform", projectionTransform);
    shaderProgram.setUniformValue("modelViewTransform", meshWithEffectTransform);
    shaderProgram.setUniformValue("normalMatrix", meshWithEffectTransform.normalMatrix());

    shaderProgram.setUniformValue("sampler", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, to.texture);

    glBindVertexArray(to.vao);
    glDrawArrays(GL_TRIANGLES, 0, to.size);

    shaderProgram.release();
}

void MainView::paintPortal(PortalObject &po, bool renderBorder) {
    QOpenGLShaderProgram &shaderProgram = shaders[ShaderType::PORTAL];
    shaderProgram.bind();
    shaderProgram.setUniformValue("modelViewTransform",
                  po.modelTransform);
    shaderProgram.setUniformValue("normalMatrix", po.modelTransform.normalMatrix());
    shaderProgram.setUniformValue("projectionTransform",
                  projectionTransform);
    shaderProgram.setUniformValue("borderWidth", 0.1F);
    shaderProgram.setUniformValue("renderBorder", renderBorder);

    // Assumes that no mesh will be infornt of the portal.
    if (renderBorder)
        glDisable(GL_CULL_FACE);

    glBindVertexArray(po.vao);
    glDrawArrays(GL_TRIANGLES, 0, po.size);

    if (renderBorder)
        glEnable(GL_CULL_FACE);

    shaderProgram.release();
}

void MainView::updateModelTransforms(SceneObject &so) {
  so.modelTransform.setToIdentity();
  so.modelTransform.translate(so.position);

  so.modelTransform = so.modelTransform * camera.getModelTransform();

  update();
}

void MainView::updatePortalEffectTransforms(PortalObject &po) {
    PortalObject::COLLISION_STATE newCollisionState = getPortalCollision(po);

    if (po.collisionState == newCollisionState) {
    return;
    }
    if (newCollisionState == PortalObject::COLLISION_STATE::NO_COLLISION
        || po.collisionState == PortalObject::COLLISION_STATE::NO_COLLISION
    ) {
        po.collisionState = newCollisionState;
        return;
    }

    if (inPortal) {
        inPortal = false;
        currentWorldEffectTransform.setToIdentity();
        currentShaderType = ShaderType::PHONG;
    } else {
        currentWorldEffectTransform = po.effectTransform;
        currentShaderType = po.shaderType;
        inPortal = true;
    }

    po.collisionState = newCollisionState;
}
PortalObject::COLLISION_STATE MainView::getPortalCollision(PortalObject &po) {
    auto portalPosition = po.modelTransform.map(QVector3D{0,0,0});
    double distance = portalPosition.length();
    if (distance < 1) {
        if (camera.getPosition().z() + 0.2*camera.viewVector().z() < 0) {
            return PortalObject::COLLISION_STATE::FRONT; // Camera is in front of the portal
        } else {
            return PortalObject::COLLISION_STATE::BACK; // Camera is in back of portal
        }
    }
    return PortalObject::COLLISION_STATE::NO_COLLISION; // No collision
}

void MainView::cleanUpSceneObject(SceneObject &so) {
    glDeleteBuffers(1, &so.positionVBO);
    glDeleteBuffers(1, &so.normalVBO);
    glDeleteBuffers(1, &so.textureVBO);
    glDeleteVertexArrays(1, &so.vao);
}

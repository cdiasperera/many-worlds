#include "camera.h"
#include "vectormath.h"
#include <QtCore/qtextstream.h>

Camera::Camera() {
    updateFunctions['W'] = &Camera::moveForward;
    updateFunctions['S'] = &Camera::moveBackward;
    updateFunctions['A'] = &Camera::moveLeft;
    updateFunctions['D'] = &Camera::moveRight;
    updateFunctions['Z'] = &Camera::hoverUp;
    updateFunctions['X'] = &Camera::hoverDown;
    updateFunctions['Q'] = &Camera::rotateLeft;
    updateFunctions['E'] = &Camera::rotateRight;
    updateFunctions['R'] = &Camera::rotateUp;
    updateFunctions['F'] = &Camera::rotateDown;
}

QMatrix4x4 Camera::getModelTransform() {
    QMatrix4x4 transform;

    transform.setToIdentity();
    transform.translate({x,y,z});

    return transform;
}

QMatrix4x4 Camera::getProjectionTransform() {
    QMatrix4x4 transform;

    transform.setToIdentity();
    transform.rotate(pan, 0, 1, 0);
    transform.rotate(tilt, 1, 0, 0);
    transform.rotate(roll, 0, 1, 0);

    return transform;
}

void Camera::update(KeyboardStatus keyboardStatus) {
    for (auto const &pair : updateFunctions) {
        int key = pair.first;
        if (keyboardStatus.isDown(key)) {
            UpdateCameraFunction update = updateFunctions[key];
            (*this.*update)();
        }
    }
}

void Camera::move(QVector3D const &direction) {
    QVector3D movement = direction * cameraMovementSpeed;

    x += movement.x();
    y += movement.y();
    z += movement.z();
}

QVector3D Camera::viewVector() {
    QQuaternion quaternion = QQuaternion::fromEulerAngles(-tilt, -pan, roll);
    QVector3D view = quaternion.rotatedVector(QVector3D(0, 0, 1));
    return view;
}

void Camera::moveForward() {
    move(viewVector());
}

void Camera::moveBackward() {
    move(-viewVector());
}

void Camera::moveRight() {
    if (-90 < pan && pan < 90) {
      move(
          VectorMath::orthogonalVectors(viewVector()).second
      );
    } else {
      move(
          -VectorMath::orthogonalVectors(viewVector()).second
      );
    } 
}

// TODO there is a bug here when you turn more than 270 degrees.
void Camera::moveLeft() {
    if (-90 < pan && pan < 90) {
      move(
          -VectorMath::orthogonalVectors(viewVector()).second
      );
    } else {
      move(
          VectorMath::orthogonalVectors(viewVector()).second
      );
    } 
}

void Camera::hoverUp() {
    move(
        -VectorMath::orthogonalVectors(viewVector()).first
    );
}

void Camera::hoverDown() {
    move(
        VectorMath::orthogonalVectors(viewVector()).first
    );
}

void Camera::rotateUp() {
    tilt -= cameraRotationalSpeed;
}

void Camera::rotateDown() {
    tilt += cameraRotationalSpeed;
}

void Camera::rotateLeft() {
    pan -= cameraRotationalSpeed;
}

void Camera::rotateRight() {
    pan += cameraRotationalSpeed;
}

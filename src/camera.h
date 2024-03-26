#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <unordered_map>

#include "keyboardstatus.h"

/*
 * A data class to hold the camera's "position" and where it's looking at.
 * Formally speaking, it's always looking forward from 0,0,0 but this class will hold
 * the values to define the model transform such that they are moved relative to the
 * camera's "0" position.
 */
class Camera
{
    /*
     * The xyz location of the camera.
     */
    float x = 0;
    float y = 0;
    float z = -15;

    // The speed the camera moves within the scene
    float cameraMovementSpeed = 0.1;

    /*
     * The pan / tilt / roll of the camera
     */
    float pan = 0;
    float tilt = 0;
    float roll = 0;

    // The speed the camera rotates around it's position
    float cameraRotationalSpeed = 0.5;

public:
    QVector3D getPosition() {
      return {x, y, z};
    }

    // Gets the unit vector in the direction of the camera perspective
    QVector3D viewVector();

    // Move / rotate the camera based on the keys currently pressed down
    void update(KeyboardStatus keyboardStatus);

    /*
     * Returns a transformation matrix (in homogeneous coordinates) such that if all objects were
     * transformed with this matrix, the camera would be at the origin facing forward.
     */
    QMatrix4x4 getModelTransform();

    /*
     * Returns a transformation matrix (in homogeneous coordinates) such that if the projection matrix
     * is multiplied with this matrix, the camera would be facing down the -z axis
     */
    QMatrix4x4 getProjectionTransform();

    Camera();

private:
    using UpdateCameraFunction = void (Camera::*)();
    std::unordered_map<int, UpdateCameraFunction> updateFunctions;

    /*
     * Move one tick (based on the camera movement speed)
     */

    // Performs the actual movement
    void move(QVector3D const &direction);


    // Setups direction of movement and calls Camera::move
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    void hoverUp();
    void hoverDown();

    /*
     * Rotate one tick (based on the camera rotational speed)
     */

    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();
};

#endif // CAMERA_H

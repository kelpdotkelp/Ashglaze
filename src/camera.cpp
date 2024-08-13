#include "camera.h"

Camera::Camera(num::Vec2 mousePos)
{
    up = num::Vec3(0.0, 1.0, 0.0);
    position = num::Vec3(0.0, 0.0, 0.0);
    direction = num::Vec3(0.0, 0.0, 1.0);

    Camera::mousePos = mousePos;
}

void Camera::updateCoordSystem()
{
    right = (num::crossProduct(up, direction)).normalize();
    //Since the up vector is not orthogonal to the direction vector
    trueUp = (num::crossProduct(direction, right)).normalize();
}

void Camera::setPos(num::Vec3 position)
{
    Camera::position = position;
    updateCoordSystem();
}

void Camera::setPos(float x, float y, float z) {position = num::Vec3(x, y, z);}

void Camera::setSpeed(float speed) {Camera::speed = speed;}

num::Mat4 Camera::getViewMatrix() {return num::lookAt(trueUp, direction, right, position);}

num::Vec3 Camera::getPos() {return position;}

num::Vec3 Camera::getDirection() {return direction;}

void Camera::disable(bool isDisabled)
{
    if (isDisabled)
    {

    }

    Camera::isDisabled = isDisabled;
}

void Camera::updateDirection(num::Vec2 mousePos)
{
    if (isDisabled)
        return;

    num::Vec2 offset = mousePos - Camera::mousePos;
    Camera::mousePos = mousePos;

    offset = offset * sensitivity;
    yaw += offset.x;
    pitch += offset.y;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    float rad = 3.14159f/180;

    direction.x = cos(rad*yaw) * cos(rad*pitch);
    direction.y = sin(rad*pitch);
    direction.z = sin(rad*yaw) * cos(rad*pitch);
    direction = direction.normalize();
    updateCoordSystem();
}

void Camera::moveForward()
{
    position = position - speed*direction;
    updateCoordSystem();
}

void Camera::moveBackward()
{
    position = position + speed*direction;
    updateCoordSystem();
}

void Camera::moveLeft()
{
    position = position - speed*right;
    updateCoordSystem();
}

void Camera::moveRight()
{
    position = position + speed*right;
    updateCoordSystem();
}

void Camera::moveUp()
{
    position.y += speed;
    updateCoordSystem();
}

void Camera::moveDown()
{
    position.y -= speed;
    updateCoordSystem();
}
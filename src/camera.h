#ifndef CAMERA_H
#define CAMERA_H

#include "num/num.h"

class Camera
{
    private:
        float speed;
        float sensitivity = 0.1f;

        float pitch = 0.0f;
        float yaw = -90.0f;

        num::Vec3 up;
        num::Vec3 position;

        num::Vec2 mousePos;

        //Camera coordinate system
        num::Vec3 direction;
        num::Vec3 right;
        num::Vec3 trueUp;

        bool isDisabled = false;

        void updateCoordSystem();
    
    public:
        Camera(num::Vec2 mousePos);

        void setPos(num::Vec3 position);
        void setPos(float x, float y, float z);
        void setSpeed(float speed);
        void updateDirection(num::Vec2 mousePos);
        void disable(bool isDisabled);

        num::Vec3 getPos();
        num::Vec3 getDirection();

        void moveForward();
        void moveBackward();
        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();

        num::Mat4 getViewMatrix();
};

#endif
#pragma once

#include "i_camera.h"

class Camera3D : public ICamera {
public:
    Camera3D(GameWindow& window, Vec3f pos = { 0.f, 0.f, 0.f }, Vec3f up = { 0.f, 1.f, 0.f });

public: // ICamera interface
    void getViews(Mat4f& view, Mat4f& projection) const;

private:
    void updateCameraVectors();

private:
    Vec3f position;
    Vec3f front;
    Vec3f up;
    Vec3f right;
    Vec3f worldUp;

    // Euler Angles
    float yaw = 0.f;
    float pitch = 0.f;

    // Camera options
    float movementSpeed = 0.f;
    float mouseSensitivity = 0.f;
    float zoom = 0.f;
};

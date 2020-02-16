#include "camera_3d.h"

#include "common/game_window.h"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera3D::Camera3D(GameWindow& window, Vec3f pos, Vec3f up)
    : ICamera(window)
    , front(0.f, 0.f, -1.f)
    , position(pos)
    , up(up)
    , worldUp(up)
    , yaw(YAW)
    , pitch(PITCH)
    , movementSpeed(SPEED)
    , mouseSensitivity(SENSITIVITY)
    , zoom(ZOOM)
{
    updateCameraVectors();
}

void Camera3D::getViews(Mat4f& view, Mat4f& projection) const
{
    const Vec2f wsize = window.getSize();

    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(glm::radians(zoom), wsize.x / wsize.y, 0.1f, 100.0f);
}

void Camera3D::updateCameraVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

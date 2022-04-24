#include "Camera.h"

// Default
Camera::Camera () {
    this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->fov = 35.0f;
}

// Custom
Camera::Camera (glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov) {
    this->cameraPos = pos;
    this->cameraFront = front;
    this->cameraUp = up;
    this->fov = fov;
}

std::function<void (float)> Camera::set_lastX_bind = nullptr;
std::function<float ()> Camera::get_lastX_bind = nullptr;
std::function<void (float)> Camera::set_lastY_bind = nullptr;
std::function<float ()> Camera::get_lastY_bind = nullptr;
std::function<void (float)> Camera::set_yaw_bind = nullptr;
std::function<float ()> Camera::get_yaw_bind = nullptr;
std::function<void (float)> Camera::set_pitch_bind = nullptr;
std::function<float ()> Camera::get_pitch_bind = nullptr;
std::function<void (float)> Camera::set_roll_bind = nullptr;
std::function<float ()> Camera::get_roll_bind = nullptr;
std::function<void (float)> Camera::set_fov_bind = nullptr;
std::function<float ()> Camera::get_fov_bind = nullptr;
std::function<void (glm::vec3)> Camera::set_cameraFront_bind = nullptr;
std::function<void (bool)> Camera::set_cameraUpdated_bind = nullptr;

void Camera::bind_instance (std::shared_ptr<Camera> instance) {
    using namespace std::placeholders;

    set_lastX_bind = std::bind (&Camera::set_lastX, instance, _1);
    get_lastX_bind = std::bind (&Camera::get_lastX, instance);
    set_lastY_bind = std::bind (&Camera::set_lastY, instance, _1);
    get_lastY_bind = std::bind (&Camera::get_lastY, instance);
    set_yaw_bind = std::bind (&Camera::set_yaw, instance, _1);
    get_yaw_bind = std::bind (&Camera::get_yaw, instance);
    set_pitch_bind = std::bind (&Camera::set_pitch, instance, _1);
    get_pitch_bind = std::bind (&Camera::get_pitch, instance);
    set_roll_bind = std::bind (&Camera::set_roll, instance, _1);
    get_roll_bind = std::bind (&Camera::get_roll, instance);
    set_fov_bind = std::bind (&Camera::set_fov, instance, _1);
    get_fov_bind = std::bind (&Camera::get_fov, instance);
    set_cameraFront_bind = std::bind (&Camera::set_cameraFront, instance, _1);
    set_cameraUpdated_bind = std::bind (&Camera::set_cameraUpdated, instance, _1);
}

void Camera::mouse_callback (GLFWwindow* window, double xpos, double ypos) {
    if (get_lastX_bind () == MAXFLOAT || get_lastY_bind () == MAXFLOAT) {
        set_lastX_bind (xpos);
        set_lastY_bind (ypos);
    }

    float xoffset = xpos - get_lastX_bind ();
    float yoffset = get_lastY_bind () - ypos; 
    set_lastX_bind (xpos);
    set_lastY_bind (ypos);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    set_yaw_bind (get_yaw_bind () + xoffset);
    set_pitch_bind (get_pitch_bind () + yoffset);

    if (get_pitch_bind () > 89.0f) set_pitch_bind (89.0f);
    if (get_pitch_bind () < -89.0f) set_pitch_bind (-89.0f);

    glm::vec3 new_front;
    new_front.x = cos (glm::radians (get_yaw_bind ())) * cos (glm::radians (get_pitch_bind ()));
    new_front.y = sin (glm::radians (get_pitch_bind ()));
    new_front.z = sin (glm::radians(get_yaw_bind ())) * cos (glm::radians (get_pitch_bind ()));
    set_cameraFront_bind (glm::normalize (new_front));

    set_cameraUpdated_bind (true);
}

void Camera::scroll_callback (GLFWwindow *window, double xoffset, double yoffset) {
    float bound_fov = get_fov_bind ();

    if (bound_fov >= 1.0f && bound_fov <= 45.0f) set_fov_bind (get_fov_bind () - yoffset);
    else if (bound_fov <= 1.0f) set_fov_bind (1.0f);
    else if (bound_fov >= 45.0f) set_fov_bind (45.0f);

    set_cameraUpdated_bind (true);
}

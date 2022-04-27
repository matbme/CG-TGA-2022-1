#include "Camera.h"
#include "Mesh.h"

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

// Set during setup_camera()
GLuint* Camera::scene_height;
GLuint* Camera::scene_width;
glm::mat4* Camera::proj_mat;
glm::mat4* Camera::view_mat;
std::vector<std::unique_ptr<Model>>* Camera::objects;
bool Camera::initialized = false;

// Internal use
GLfloat Camera::last_ray_test_run = 0.0;
std::unique_ptr<Model> *Camera::closest_intersect = nullptr;
bool Camera::right_click_hold = false;
bool Camera::middle_click_hold = false;

// Instance binding access
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
std::function<void (glm::vec3)> Camera::set_cameraPos_bind = nullptr;
std::function<glm::vec3 ()> Camera::get_cameraPos_bind = nullptr;
std::function<void (glm::vec3)> Camera::set_cameraFront_bind = nullptr;
std::function<glm::vec3 ()> Camera::get_cameraFront_bind = nullptr;
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
    set_cameraPos_bind = std::bind (&Camera::set_cameraPos, instance, _1);
    get_cameraPos_bind = std::bind (&Camera::get_cameraPos, instance);
    set_cameraFront_bind = std::bind (&Camera::set_cameraFront, instance, _1);
    get_cameraFront_bind = std::bind (&Camera::get_cameraFront, instance);
    set_cameraUpdated_bind = std::bind (&Camera::set_cameraUpdated, instance, _1);
}

void Camera::mouse_callback (GLFWwindow* window, double xpos, double ypos) {
    if (get_lastX_bind () == MAXFLOAT || get_lastY_bind () == MAXFLOAT) {
        set_lastX_bind (0);
        set_lastY_bind (0);
    }

    float xoffset = xpos;
    float yoffset = -ypos; 

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Ray picking based on Anton's implementation
    if (Camera::initialized && (glfwGetTime() - Camera::last_ray_test_run) >= 0.2) {
        Camera::closest_intersect = nullptr;
        glm::vec3 ray = Camera::_get_ray_from_mouse(*Camera::scene_width/2.0, *Camera::scene_height/2.0);

        float closest_intersection = 0.0f;
        for (auto&& obj : *objects) {
            obj->remove_highlight(); // Clear previous highlights

            for (auto mesh : obj->meshes) {
                float t_dist = 0.0f;
                if (Camera::_ray_sphere(get_cameraPos_bind(),
                                        ray+get_cameraFront_bind(),
                                        mesh.get_center_pos(), 
                                        3.0f, 
                                        t_dist)) 
                {
                    // if more than one sphere is in path of ray, only use the closest one
                    if (Camera::closest_intersect == nullptr || t_dist < closest_intersection ) {
                        Camera::closest_intersect = &obj;
                        closest_intersection = t_dist;
                    }
                    break;
                }
            }
        }

        Camera::last_ray_test_run = glfwGetTime ();
        if (Camera::closest_intersect != nullptr)
            Camera::closest_intersect->get()->highlight ();
    }

    if (Camera::right_click_hold) {
        if (Camera::closest_intersect != nullptr) {
            Camera::closest_intersect->get()->rotate (glm::radians (xoffset*3), glm::vec3 (0.0f, 0.0f, 1.0f));
            Camera::closest_intersect->get()->rotate (glm::radians (yoffset*3), glm::vec3 (1.0f, 0.0f, 0.0f));
        }
    } 
    else if (Camera::middle_click_hold) {
        if (Camera::closest_intersect != nullptr)
            Camera::closest_intersect->get()->rotate (glm::radians (xoffset*3), glm::vec3 (0.0f, 1.0f, 0.0f));
    } else {
        set_yaw_bind (get_yaw_bind () + xoffset);
        set_pitch_bind (get_pitch_bind () + yoffset);

        if (get_yaw_bind () > 179.0f) set_yaw_bind (-179.0f);
        if (get_yaw_bind () < -179.0f) set_yaw_bind (179.0f);

        if (get_pitch_bind () > 89.0f) set_pitch_bind (89.0f);
        if (get_pitch_bind () < -89.0f) set_pitch_bind (-89.0f);

        glm::vec3 new_front;
        new_front.x = cos (glm::radians (get_yaw_bind ())) * cos (glm::radians (get_pitch_bind ()));
        new_front.y = sin (glm::radians (get_pitch_bind ()));
        new_front.z = sin (glm::radians(get_yaw_bind ())) * cos (glm::radians (get_pitch_bind ()));
        set_cameraFront_bind (glm::normalize (new_front));

        set_cameraUpdated_bind (true);
    }

    glfwSetCursorPos (window, 0, 0);
}

void Camera::scroll_callback (GLFWwindow *window, double xoffset, double yoffset) {
    if (closest_intersect != nullptr) {
        glm::vec3 scale_factors;
        if (yoffset > 0) scale_factors = glm::vec3 (1.1f, 1.1f, 1.1f);
        else if (yoffset < 0) scale_factors = glm::vec3 (0.9f, 0.9f, 0.9f);

        closest_intersect->get()->rescale (scale_factors);
    }
}

void Camera::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        action == GLFW_PRESS ? 
            Camera::right_click_hold = true :
            Camera::right_click_hold = false;

    else if (button == GLFW_MOUSE_BUTTON_3)
        action == GLFW_PRESS ?
            Camera::middle_click_hold = true :
            Camera::middle_click_hold = false;
}

inline glm::vec3 Camera::_get_ray_from_mouse (float mouse_x, float mouse_y) {
    // screen space (viewport coordinates)
    float x = ( 2.0f * mouse_x ) / *Camera::scene_width - 1.0f;
    float y = 1.0f - ( 2.0f * mouse_y ) / *Camera::scene_width;
    float z = 1.0f;

    // normalised device space
    glm::vec3 ray_nds = glm::vec3 (x, y, z);

    // clip space
    glm::vec4 ray_clip = glm::vec4 (ray_nds.x, ray_nds.y, -1.0, 1.0);

    // eye space
    glm::vec4 ray_eye = glm::inverse (*proj_mat) * ray_clip;
    ray_eye = glm::vec4 (ray_eye.x, ray_eye.y, -1.0, 0.0);

    // world space
    glm::vec3 ray_wor = glm::vec3 (glm::inverse (*view_mat) * ray_eye);

    // don't forget to normalise the vector at some point
    ray_wor = glm::normalize (ray_wor);
    return ray_wor;
}

inline bool Camera::_ray_sphere (glm::vec3 ray_origin,
                          glm::vec3 ray_direction, 
                          glm::vec3 sphere_centre, 
                          float sphere_radius, 
                          float& intersection_distance)
{
    // work out components of quadratic
    glm::vec3 dist_to_sphere = ray_origin - sphere_centre;
    float b = glm::dot(ray_direction, dist_to_sphere);
    float c = glm::dot(dist_to_sphere, dist_to_sphere) - sphere_radius * sphere_radius;
    float b_squared_minus_c = b * b - c;

    // check for "imaginary" answer. == ray completely misses sphere
    if (b_squared_minus_c < 0.0f) return false;

    // check for ray hitting twice (in and out of the sphere)
    if (b_squared_minus_c > 0.0f) {
        // get the 2 intersection distances along ray
        float t_a = -b + std::sqrt(b_squared_minus_c);
        float t_b = -b - std::sqrt(b_squared_minus_c);
        intersection_distance = t_b;
        // if behind viewer, throw one or both away
        if (t_a < 0.0) {
            if (t_b < 0.0) return false;
        } else if (t_b < 0.0) {
            intersection_distance = t_a;
        }

        return true;
    }

    // check for ray hitting once (skimming the surface)
    if (0.0f == b_squared_minus_c) {
        // if behind viewer, throw away
        float t = -b + sqrt(b_squared_minus_c);
        if (t < 0.0f) return false;
        intersection_distance = t;
        return true;
    }

    return false;
}

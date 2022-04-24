#ifndef CAMERA_H
#define CAMERA_H

#include <functional>
#include <memory>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    Camera ();
    Camera (glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov);

    ~Camera () { }

    inline bool changed () { return this->cameraUpdated; }

    // Funcoes de callback
    static void mouse_callback (GLFWwindow *window, double xpos, double ypos);
    static void scroll_callback (GLFWwindow* window, double xoffset, double yoffset);

    // Getters e setters para variaveis usadas no callback
    inline void set_lastX (float val) { this->lastX = val; }
    inline float get_lastX () { return this->lastX; }
    inline void set_lastY (float val) { this->lastY = val; }
    inline float get_lastY () { return this->lastY; }
    inline void set_yaw (float val) { this->yaw = val; }
    inline float get_yaw () { return this->yaw; }
    inline void set_pitch (float val) { this->pitch = val; }
    inline float get_pitch () { return this->pitch; }
    inline void set_roll (float val) { this->roll = val; }
    inline float get_roll () { return this->roll; }
    inline void set_fov (float val) { this->fov = val; }
    inline float get_fov () { return this->fov; }
    inline void set_cameraFront (glm::vec3 val) { this->cameraFront = val; }
    inline void set_cameraUpdated (bool val) { this->cameraUpdated = val; }

    // Bind de instancia para as funcoes de callback
    static void bind_instance (std::shared_ptr<Camera> instance);

    // Se os valores foram alterados
    bool cameraUpdated = false;
    // Vetores de camera
    glm::vec3 cameraPos, cameraFront, cameraUp;
    // Campo de visao da camera
    float fov;

    // Velocidade de movimento da camera
    static constexpr float cameraSpeed = 0.05f;

private:
    // Valores para transformacao
    float lastX = MAXFLOAT, lastY = MAXFLOAT;
    float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;

    // Para qual instancia de camera os valores do callback sao salvos
    static std::function<void (float)> set_lastX_bind;
    static std::function<float ()> get_lastX_bind;

    static std::function<void (float)> set_lastY_bind;
    static std::function<float ()> get_lastY_bind;

    static std::function<void (float)> set_yaw_bind;
    static std::function<float ()> get_yaw_bind;

    static std::function<void (float)> set_pitch_bind;
    static std::function<float ()> get_pitch_bind;

    static std::function<void (float)> set_roll_bind;
    static std::function<float ()> get_roll_bind;

    static std::function<void (float)> set_fov_bind;
    static std::function<float ()> get_fov_bind;

    static std::function<void (glm::vec3)> set_cameraFront_bind;
    static std::function<void (bool)> set_cameraUpdated_bind;
};

#endif

#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Shader;

class Camera {
private:
    glm::vec3 m_Position;
    glm::vec3 m_Orientation {0.0f, 0.0f, -1.0f};
    glm::vec3 m_Up {0.0f, 1.0f, 0.0f};

    int m_Width;
    int m_Height;


    float m_Speed = 3.f;
    float m_Sensitivity = 40.0f;
    bool firstEntry {true};

public:
    Camera(int width, int height, glm::vec3 position);
    ~Camera() = default;

    glm::mat4 camMatrix(float fov, float zNear, float zFar, Shader &shader);

    void HandleInputs(GLFWwindow *window, float deltaSeconds);
};

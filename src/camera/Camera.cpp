#include "Camera.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

Camera::Camera(int width, int height, glm::vec3 position)
: m_Width(width), m_Height(height), m_Position(position)
{
}

glm::mat4 Camera::camMatrix(float fov, float zNear, float zFar, Shader &shader) {
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    projection = glm::perspective(fov, (float) m_Width/m_Height, zNear, zFar);

    return projection * view;
}

void Camera::HandleInputs(GLFWwindow *window, float deltaSeconds) {

    if (firstEntry) {
        glfwSetCursorPos(window, m_Width / 2.0, m_Height / 2.0);
        firstEntry = false;
    }

    // mouse control
    // 每一帧结束重置鼠标位置到屏幕中心，每一帧开始时计算鼠标在x y方向上移动了屏幕的多少比例

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    double xPos;
    double yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    float x = m_Sensitivity * ((xPos - m_Width / 2.0) / m_Width);
    float y = m_Sensitivity * ((yPos - m_Height / 2.0) / m_Height);

    m_Orientation = glm::rotate(m_Orientation, glm::radians(-y), glm::normalize(glm::cross(m_Orientation, m_Up)));
    m_Orientation = glm::rotate(m_Orientation, glm::radians(-x), m_Up);

    glfwSetCursorPos(window, m_Width / 2.0, m_Height / 2.0);

    // key stroke
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_Position += deltaSeconds * m_Speed * m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_Position += deltaSeconds * m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_Position += deltaSeconds * m_Speed * -m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_Position += deltaSeconds* m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_Position += deltaSeconds * m_Speed * m_Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        m_Position += deltaSeconds * m_Speed * -m_Up;
    }

}


    // std::cout << "x Pos " << x << ", yPos " << y << std::endl;


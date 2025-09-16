#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <thread>

#include "Macros.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "GLEW INIT ERROR!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] {
        -0.5f, -0.5f, 0.f , 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, // 0
        0.5f , -0.5f, 0.f , 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, // 1
        0.5f , 0.5f , 0.f , 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, // 2
        -0.5f, -0.5f, 0.5f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f // 3
    };

    unsigned int indicies[] {
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
    };

    // blend mode
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb {positions, 10 * 4 * sizeof(float)};
    IndexBuffer ib {indicies, 4 * 3};

    VertexBufferLayout layout;
    // vertices
    layout.Push<float>(4);
    // tex coord
    layout.Push<float>(2);
    // vertices color
    layout.Push<float>(4);

    va.AddBuffer(vb, layout);
    Shader shader { "res/shaders/Basic.shader" };
    shader.Bind();

    Texture texture { "res/textures/test1.png" };
    texture.Bind();
    
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    va.UnBind();
    shader.UnBind();
    vb.Unbind();
    ib.Unbind();

    float angle = 0.f;
    float time = 0.f;

    double lastFrameTime = glfwGetTime();
    float targetFPS = 10.f;
    float targetSecPerFrame = 1.f / targetFPS;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double ThisFrameTime = glfwGetTime();
        double deltaSeconds = ThisFrameTime - lastFrameTime;
        lastFrameTime = ThisFrameTime;

        /* Render here */
        // GLCall(glClear(GL_COLOR_BUFFER_BIT));
        renderer.Clear();

        shader.Bind();

        std::cout << 1.f / deltaSeconds << std::endl;
        glm::mat4 model = 
            glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.f, 1.1f)) *
            // glm::scale(glm::mat4(1.f), glm::vec3(1.5f, 1.5f, 1.5f)) * 
            glm::rotate(glm::mat4(1.f), glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
        // glm::mat4 projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
        glm::mat4 projection = glm::perspective(45.f, 16.f/9.f, -1.f, 1.f);
        glm::mat4 mvp = projection * view * model;
        angle += 10.f * deltaSeconds;

        shader.SetUniformMat4f("u_Mvp", model);
        // std::cout << deltaSeconds << std::endl;
        if (angle >= 360.f) {
            angle = 0.f;
        }
        // shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.f);

        renderer.Draw(va, ib, shader);
        // va.Bind();
        // ib.Bind();

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
}

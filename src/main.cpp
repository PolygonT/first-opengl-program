#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "GltfModel.h"
#include "Macros.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void drawMesh(const std::map<int, GLuint>& vbos,
              tinygltf::Model &model, tinygltf::Mesh &mesh) {
  for (size_t i = 0; i < mesh.primitives.size(); ++i) {
    tinygltf::Primitive primitive = mesh.primitives[i];
    tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

    glDrawElements(primitive.mode, indexAccessor.count,
                   indexAccessor.componentType,
                   BUFFER_OFFSET(indexAccessor.byteOffset));
  }
}

// recursively draw node and children nodes of model
void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
                    tinygltf::Model &model, tinygltf::Node &node) {
  if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
    drawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
  }
  for (size_t i = 0; i < node.children.size(); i++) {
    drawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
  }
}
void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
               tinygltf::Model &model) {
  glBindVertexArray(vaoAndEbos.first);

  const tinygltf::Scene &scene = model.scenes[model.defaultScene];
  for (size_t i = 0; i < scene.nodes.size(); ++i) {
    drawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
  }

  glBindVertexArray(0);
}


int main(void)
{
    GLFWwindow* window;

    int width {1280}, height {960};


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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
        //     COORDS          /         TexCoords      /       Colors
        -0.5f, 0.0f, -0.5f,        0.0f, 0.0f,         1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f,  0.5f,        5.0f, 0.0f,         0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, 0.0f,  0.5f,        0.0f, 0.0f,         0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, 0.0f, -0.5f,        5.0f, 0.0f,         0.3f, 0.8f, 0.2f, 1.0f,
         0.0f, 0.8f,  0.0f,        2.5f, 5.0f,         1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indicies[] {
        0, 1, 2,
        0, 2, 3,
        0, 4, 1,
        1, 4, 2,
        2, 4, 3,
        3, 4, 0
    };

    // blend mode
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb {positions, 9 * 5 * sizeof(float)};
    IndexBuffer ib {indicies, 6 * 3};

    VertexBufferLayout layout;
    // vertices
    layout.Push<float>(3);
    // tex coord
    layout.Push<float>(2);
    // vertices color
    layout.Push<float>(4);

    va.AddBuffer(vb, layout);
    Shader shader { "res/shaders/Basic.shader" };
    shader.Bind();

    Shader simpleShader { "res/shaders/Simple.shader" };
    simpleShader.Bind();

    // Texture texture { "res/textures/brick.png" };
    // texture.Bind();
    // 
    // shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
    Camera camera {width, height, {0.0f, 0.2f, 2.0f}};

    va.UnBind();
    shader.UnBind();
    vb.Unbind();
    ib.Unbind();

    float angle = 0.f;
    float time = 0.f;

    double lastFrameTime = glfwGetTime();
    float targetFPS = 10.f;
    float targetSecPerFrame = 1.f / targetFPS;


    GltfModel gltfModel {"res/model/practice.gltf"};
    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos = gltfModel.BindModel();

    // gltfModel.UnBind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double ThisFrameTime = glfwGetTime();
        double deltaSeconds = ThisFrameTime - lastFrameTime;
        lastFrameTime = ThisFrameTime;

        /* Render here */
        // GLCall(glClear(GL_COLOR_BUFFER_BIT));
        renderer.Clear();

        simpleShader.Bind();
        // std::cout << 1.f / deltaSeconds << std::endl;

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // glm::mat4 model(1.f);
        
        camera.HandleInputs(window, deltaSeconds);
        glm::mat4 camMatrix = camera.camMatrix(glm::radians(45.f), 0.1f, 100.f, shader);
        glm::mat4 mvp = camMatrix * model;
        
        angle += 10.f * deltaSeconds;

        shader.SetUniformMat4f("u_Mvp", mvp);
        if (angle >= 360.f) {
            angle = 0.f;
        }

        // renderer.Draw(va, ib, shader);
        // renderer.Draw(gltfModel, simpleShader);
        drawModel(vaoAndEbos, gltfModel.m_Model);


        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
}


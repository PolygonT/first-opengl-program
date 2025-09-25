#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "GltfModel.h"
#include "Macros.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void drawMesh(const ModelView& modelView,
              tinygltf::Model &model, tinygltf::Mesh &mesh) {
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

        Texture* ptex {nullptr};
        auto it = modelView.texMap->find(primitive.material);
        if (it != modelView.texMap->end()) {
            Texture& tex = it->second;
            tex.Bind();
            ptex = &tex;
        }

        modelView.vbMap->at(indexAccessor.bufferView).Bind();
        glDrawElements(primitive.mode, indexAccessor.count,
                       indexAccessor.componentType,
                       BUFFER_OFFSET(indexAccessor.byteOffset));
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

        if (ptex) {
            ptex->UnBind();
        }
    }
}

// recursively draw node and children nodes of model
void drawModelNodes(const ModelView& modelView,
                    tinygltf::Model &model, tinygltf::Node &node) {
  if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
    drawMesh(modelView, model, model.meshes[node.mesh]);
  }
  for (size_t i = 0; i < node.children.size(); i++) {
    drawModelNodes(modelView, model, model.nodes[node.children[i]]);
  }
}

void drawModel(const ModelView& modelView,
               tinygltf::Model &model) {

    auto vas = modelView.vaMap;
    const tinygltf::Scene &scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        vas->at(i).Bind();
        drawModelNodes(modelView, model, model.nodes[scene.nodes[i]]);
        vas->at(i).UnBind();
    }

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

    // blend mode
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Shader simpleShader { "res/shaders/Simple.shader" };
    simpleShader.Bind();


    GltfModel gltfModel {"res/model/marathon.glb"};
    ModelView modelView = gltfModel.BindModel();

    // Texture tex {"res/textures/brick.png"};
    // tex.Bind();

    simpleShader.SetUniform1i("u_Texture", 0);

    Renderer renderer;
    Camera camera {width, height, {0.0f, 0.2f, 2.0f}};

    float angle = 0.f;
    float time = 0.f;

    double lastFrameTime = glfwGetTime();
    float targetFPS = 10.f;
    float targetSecPerFrame = 1.f / targetFPS;

    simpleShader.UnBind();


    // gltfModel.UnBind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double ThisFrameTime = glfwGetTime();
        double deltaSeconds = ThisFrameTime - lastFrameTime;
        lastFrameTime = ThisFrameTime;

        /* Render here */
        renderer.Clear();

        simpleShader.Bind();
        // std::cout << 1.f / deltaSeconds << std::endl;

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        
        camera.HandleInputs(window, deltaSeconds);
        glm::mat4 camMatrix = camera.camMatrix(glm::radians(45.f), 0.1f, 100.f, simpleShader);
        glm::mat4 mvp = camMatrix * model;
        
        angle += 10.f * deltaSeconds;

        simpleShader.SetUniformMat4f("u_Mvp", mvp);
        if (angle >= 360.f) {
            angle = 0.f;
        }

        // renderer.Draw(va, ib, shader);
        // renderer.Draw(gltfModel, simpleShader);
        drawModel(modelView, gltfModel.m_Model);


        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glfwTerminate();
    return 0;
}


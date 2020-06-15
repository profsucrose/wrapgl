#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "lib/VertexBuffer.h"
#include "lib/IndexBuffer.h"
#include "lib/VertexArray.h"
#include "lib/Shader.h"
#include "lib/Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* initWindow() {
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Time", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
       std::cout << "Failed to init GLEW" << std::endl;
       return nullptr;
    }
    
    glfwSwapInterval(1);
    return window;
    
}

int main() {
    GLFWwindow* window = initWindow();
    if (!window)
        return -1;
    
    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    GLCall( glEnable(GL_BLEND) );
    GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    
    // vertex array object
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    
    va.addBuffer(vb, layout);
    
    IndexBuffer ib(indices, 6);
    
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3, -0.2, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2, -0.2, 0));
    
    glm::mat4 mvp = proj * view * model;
    
    Shader shader("res/shaders/vertex_shader.vs", "res/shaders/frag_shader.fs");
    shader.bind();
    shader.setUniformMat4f("u_MVP", mvp);

    Texture texture("res/textures/gold-dollar.png");
    texture.bind();
    
    Renderer renderer;
    
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();
        
        renderer.draw(va, ib, shader);
      
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

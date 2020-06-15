#include <string>
#include "Renderer.h"

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define WORKING_PATH "/Users/moffice/Documents/Cpp/opengl/GLFW OpenGL/GLFW OpenGL/"

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath) : m_RendererID(0) {
    std::string vertexShaderSource = parseShaderFile(vertexShaderFilepath);
    std::string fragmentShaderSource = parseShaderFile(fragmentShaderFilepath);
    
    m_RendererID = createShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader() {
    GLCall( glUseProgram(0) );
}

std::string Shader::parseShaderFile(const std::string &filepath) {
    std::ifstream file(WORKING_PATH + filepath);
    std::stringstream ss;
    ss << file.rdbuf();
    
    return ss.str();
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall( glAttachShader(program, vs) );
    GLCall( glAttachShader(program, fs) );

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "Failed to link program: " << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    GLCall( glDeleteShader(vs) );
    GLCall( glDeleteShader(fs) );

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source) {
    GLCall( unsigned int id = glCreateShader(type) );
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );

    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    std::cout
        << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
        << " shader compile status: "
        << result
        << std::endl;
    if (result == GL_FALSE) {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cout
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall( glDeleteShader(id) );
        return 0;
    }

    return id;
}

int Shader::getUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::bind() const {
    GLCall( glUseProgram(m_RendererID) );
}

void Shader::unbind() const {
    GLCall( glUseProgram(0) );
}

void Shader::setUniform1i(const std::string& name, int v0) {
    GLCall( glUniform1i(getUniformLocation(name), v0) );
}

void Shader::setUniform1f(const std::string& name, float v0) {
    GLCall( glUniform1f(getUniformLocation(name), v0) );
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall( glUniform4f(getUniformLocation(name), v0, v1, v2, v3) );
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& mat) {
    GLCall( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]) );
}

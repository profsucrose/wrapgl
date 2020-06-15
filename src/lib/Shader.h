#pragma once

#include <string>
#include "Renderer.h"

#include "../vendor/glm/glm.hpp"

#include <unordered_map>

class Shader {
private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
    std::string parseShaderFile(const std::string& filepath);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(unsigned int type, const std::string& source);
    int getUniformLocation(const std::string& name);
public:
    Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
    ~Shader();
    
    void bind() const;
    void unbind() const;
    
    // set uniforms
    void setUniform1i(const std::string& name, int v0);
    void setUniform1f(const std::string& name, float v0);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, const glm::mat4& mat);
};

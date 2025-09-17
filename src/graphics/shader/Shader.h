#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {

private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    // Set uniforms
    void SetUniform1i(const std::string &name, int value);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2,
                      float v3);
    void SetUniformMat4f(const std::string& name, glm::mat4& mat4);

  private:
    int GetUniformLocation(const std::string &name);

    unsigned int CompileShader();
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragementShader);
};

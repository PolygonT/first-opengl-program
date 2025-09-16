#include "Shader.h"

#include "Macros.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &filepath) {
    ShaderProgramSource shaderSource = ParseShader(filepath);
    m_RendererID = createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const {
    GLCall(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream fs {filepath};

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];
    std::string line;
    ShaderType ReadingType {ShaderType::NONE};
    while (std::getline(fs, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                ReadingType = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                ReadingType = ShaderType::FRAGMENT;
        } else {
            ss[(int) ReadingType] << line << std::endl;
        }
    }

    return { ss[0].str(), ss[1].str() };
} 


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragementShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragementShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}



void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2,
                          float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string &name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string &name, glm::mat4& mat4) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4)))
}


int Shader::GetUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;
    }
    // ASSERT(location != -1);

    m_UniformLocationCache[name] = location;
    return location;
}


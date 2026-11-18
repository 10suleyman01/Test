//
// Created by sbash on 18.11.2026.
//

#include "ShaderProgram.h"

#include <iostream>


namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader) {
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
            std::cerr << "ERROR::VERTEX_SHADER: Compile-time error" << std::endl;
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
            std::cerr << "ERROR::VERTEX_SHADER: Compile-time error" << std::endl;
            glDeleteShader(vertexShaderID); // при ошибке фрагментного шейдера удалить вершинный шейдер
            return;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar info[1024];
            glGetShaderInfoLog(programID, 1024, nullptr, info);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << info << std::endl;
        } else {
            isProgramCompiled = true;
        }
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
        glDeleteProgram(programID);
        programID = shaderProgram.programID;
        isProgramCompiled = shaderProgram.isProgramCompiled;

        shaderProgram.programID = 0;
        shaderProgram.isProgramCompiled = false;
        return *this;

    }
    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
        programID = shaderProgram.programID;
        isProgramCompiled = shaderProgram.isProgramCompiled;

        shaderProgram.programID = 0;
        shaderProgram.isProgramCompiled = false;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(programID);
    }

    bool ShaderProgram::createShader(const std::string &source, const GLenum shaderType, GLuint &shaderID) {
        shaderID = glCreateShader(shaderType);
        const char *code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, info);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << info << std::endl;
            return false;
        }
        return true;
    }

    void ShaderProgram::use() const {
        glUseProgram(programID);
    }
}

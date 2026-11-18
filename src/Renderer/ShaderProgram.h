#pragma once

#ifndef BATTLECITY_SHADERPROGRAM_H
#define BATTLECITY_SHADERPROGRAM_H

#include <glad/glad.h>
#include <string>

namespace Renderer {
    class ShaderProgram {
    private:
        bool isProgramCompiled = false;
        GLuint programID = 0;

        static bool createShader(const std::string &source, GLenum shaderType, GLuint &shaderID);

    public:
        ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

        ShaderProgram() = delete;
        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

        ~ShaderProgram();

        bool isCompiled() const { return isProgramCompiled; }

        void use() const;
    };
}

#endif

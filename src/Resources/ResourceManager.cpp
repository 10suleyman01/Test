//
// Created by sbash on 19.11.2026.
//

#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>


ResourceManager::ResourceManager(const std::string &executable_path) {
    const size_t found = executable_path.find_last_of("/\\");
    this->executable_path = executable_path.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShader(const std::string &shader_name,
                                                                     const std::string &vertex_shader_path,
                                                                     const std::string &fragment_shader_path) {

    std::string vertexFileSource = getFileSource(vertex_shader_path);
    if (vertexFileSource.empty()) {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }
    std::string fragmentFileSource = getFileSource(fragment_shader_path);
    if (fragmentFileSource.empty()) {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram>& newShader = shader_programs_map.emplace(shader_name, std::make_shared<Renderer::ShaderProgram>(vertexFileSource, fragmentFileSource)).first->second;
    if (newShader->isCompiled()) {
        return newShader;
    }

    std::cerr << "Can't load shader program:\n" << "Vertex: \n" << vertex_shader_path << "Fragment: " << fragment_shader_path << std::endl;

    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShader(const std::string &shader_name) {
    const auto it = shader_programs_map.find(shader_name);

    if (it == shader_programs_map.end()) {
        std::cerr << "Shader not found: " << shader_name << '\n';
        return nullptr;
    }

    return it->second;
}

std::string ResourceManager::getFileSource(const std::string &relative_file_path) const {
    std::ifstream file;
    file.open(this->executable_path + "/" + relative_file_path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << relative_file_path << std::endl;
        return std::string{};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

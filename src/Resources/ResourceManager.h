#pragma once
//
// Created by sbash on 19.11.2026.
//

#ifndef BATTLECITY_RESOURCEMANAGER_H
#define BATTLECITY_RESOURCEMANAGER_H
#include <map>
#include <memory>
#include <string>

#include "../Renderer/ShaderProgram.h"


class ResourceManager {
private:
    typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram> > ShaderProgramsMap;
    ShaderProgramsMap shader_programs_map;
    std::string executable_path;

    [[nodiscard]] std::string getFileSource(const std::string &relative_file_path) const;

public:
    explicit ResourceManager(const std::string &executable_path);

    ~ResourceManager() = default;

    ResourceManager(const ResourceManager &) = delete;

    ResourceManager &operator=(const ResourceManager &) = delete;

    ResourceManager &operator=(ResourceManager &&) = delete;

    ResourceManager(ResourceManager &&) = delete;

    std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string &shader_name,
                                                        const std::string &vertex_shader_path,
                                                        const std::string &fragment_shader_path);

    std::shared_ptr<Renderer::ShaderProgram> getShader(const std::string &shader_name);
};


#endif //BATTLECITY_RESOURCEMANAGER_H

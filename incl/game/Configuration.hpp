//
// Created by Necromant on 29.04.2021.
//

#pragma once

#include <string>
#include <robot2D/Core/Vector2.h>

struct ResourceConfiguration {
    mutable std::string audioDir;
    mutable std::string fontDir;
    mutable std::string levelDir;
    mutable std::string textureDir;
    mutable std::string shaderDir;
};

struct GameConfiguration {
    mutable float speed;
    mutable float ball_radius;
    mutable unsigned int max_lives;
    mutable unsigned int emitter_new_sz;
    mutable robot2D::vec2f ball_velocity;
    mutable robot2D::vec2f paddle_size;
};

class Configuration {
public:
    Configuration();
    ~Configuration() = default;


    bool loadResources(const std::string& path) const;
    bool loadGameSettings(const std::string& path) const;

    GameConfiguration& getGameConfiguration();
    ResourceConfiguration& getResourceConfiguration();
private:
    GameConfiguration m_gameConfiguration;
    ResourceConfiguration m_resourceConfiguration;
};

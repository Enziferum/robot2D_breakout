/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D - Zlib license.
This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.
2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#pragma once

#include <string>
#include <robot2D/Core/Vector2.hpp>

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


    bool loadResources(const std::string &path) const;

    bool loadGameSettings(const std::string &path) const;

    GameConfiguration &getGameConfiguration();

    ResourceConfiguration &getResourceConfiguration();

private:
    GameConfiguration m_gameConfiguration;
    ResourceConfiguration m_resourceConfiguration;
};

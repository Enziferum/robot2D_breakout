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

#include <unordered_map>
#include <vector>

#include <inih/INIReader.h>
#include <robot2D/Util/Logger.hpp>

#include "game/Configuration.hpp"

const robot2D::vec2f badVector = robot2D::vec2f(-0.0002, -0.0002);
const std::string sep = ",";

enum class ResourceID {
    Audio,
    Font,
    Level,
    Texture,
    Shader
};


std::vector<std::string> split(const std::string &text, const std::string &sep) {
    std::vector<std::string> res;
    std::size_t pos_start = 0U, pos_end, index = sep.length();

    while ((pos_end = text.find(sep, pos_start)) != std::string::npos) {
        auto token = text.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + index;
        res.emplace_back(token);
    }

    res.emplace_back(text.substr(pos_start));

    return res;
}

robot2D::vec2f combineFromString(const std::string &text) {
    auto vec = split(text, sep);
    robot2D::vec2f res = badVector;
    if (vec.empty())
        return res;

    res.x = std::stof(vec[0]);
    res.y = std::stof(vec[1]);

    return res;
}


Configuration::Configuration() {}

bool Configuration::loadResources(const std::string &path) const {
    const std::string resourceSection = "Resources";

    std::unordered_map<ResourceID, std::string> resourceIDs = {
            {ResourceID::Audio,   "audioDir"},
            {ResourceID::Font,    "fontDir"},
            {ResourceID::Level,   "levelDir"},
            {ResourceID::Texture, "textureDir"},
            {ResourceID::Shader,  "shaderDir"},
    };


    INIReader reader(path);
    if (reader.ParseError() < 0) {
        LOG_ERROR("Can't read path % ", path.c_str());
        return false;
    }

    if (!reader.HasSection(resourceSection)) {
        LOG_ERROR_E("No resources section")
        return false;
    }

    m_resourceConfiguration.audioDir = reader.GetString(resourceSection, resourceIDs[ResourceID::Audio], "");
    m_resourceConfiguration.fontDir = reader.GetString(resourceSection, resourceIDs[ResourceID::Font], "");
    m_resourceConfiguration.levelDir = reader.GetString(resourceSection, resourceIDs[ResourceID::Level], "");
    m_resourceConfiguration.textureDir = reader.GetString(resourceSection, resourceIDs[ResourceID::Texture], "");
    m_resourceConfiguration.shaderDir = reader.GetString(resourceSection, resourceIDs[ResourceID::Shader], "");

    return true;
}

bool Configuration::loadGameSettings(const std::string &path) const {
    INIReader reader(path);
    if (reader.ParseError() < 0) {
        LOG_ERROR("Can't read path % ", path.c_str());
        return false;
    }

    const std::string gameSection = "Game";

    if (!reader.HasSection(gameSection)) {
        LOG_ERROR_E("No game section")
        return false;
    }

    m_gameConfiguration.speed = reader.GetReal(gameSection, "speed", 0);
    m_gameConfiguration.ball_radius = reader.GetReal(gameSection, "ball_radius", 0);
    m_gameConfiguration.max_lives = reader.GetInteger(gameSection, "max_lives", 0);
    m_gameConfiguration.emitter_new_sz = reader.GetInteger(gameSection, "emitter_new_sz", 0);

    auto velocity_str = reader.GetString(gameSection, "ball_velocity", "");
    auto paddle_str = reader.GetString(gameSection, "paddle_size", "");
    m_gameConfiguration.ball_velocity = combineFromString(velocity_str);
    m_gameConfiguration.paddle_size = combineFromString(paddle_str);

    return true;
}

GameConfiguration &Configuration::getGameConfiguration() {
    return m_gameConfiguration;
}

ResourceConfiguration &Configuration::getResourceConfiguration() {
    return m_resourceConfiguration;
}

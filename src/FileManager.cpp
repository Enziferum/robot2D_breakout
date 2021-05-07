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

#include <game/FileManager.hpp>
#include "filesystem/FileSystem.hpp"

FileManager::FileManager() : m_resourceConfiguration(nullptr) {
    robot2D::FileSystem fileSystem;
    m_basepath = fileSystem.getCurrentDir();
}

std::string FileManager::combinePath(const ResourceType &resourceType, const std::string &localFileName) {
    if (!m_resourceConfiguration)
        return "";

    robot2D::FileSystem fileSystem;
    std::string fullPath = m_basepath;
#ifdef __APPLE__
    fullPath += '/';
#endif
    if (resourceType == ResourceType::Audio)
        fullPath += m_resourceConfiguration->audioDir;

    if (resourceType == ResourceType::Font)
        fullPath += m_resourceConfiguration->fontDir;

    if (resourceType == ResourceType::Level)
        fullPath += m_resourceConfiguration->levelDir;

    if (resourceType == ResourceType::Texture)
        fullPath += m_resourceConfiguration->textureDir;

    if (resourceType == ResourceType::Shader)
        fullPath += m_resourceConfiguration->shaderDir;

    if (!fileSystem.isDir(fullPath))
        return "";

    fullPath += "/" + localFileName;
    return fullPath;
}

void FileManager::setConfiguration(const ResourceConfiguration &configuration) {
    m_resourceConfiguration = &configuration;
}

std::vector<std::string> FileManager::levelsPath() {
    std::vector<std::string> res;
    if (!m_resourceConfiguration)
        return std::vector<std::string>();

    robot2D::FileSystem fs;
#ifdef __APPLE__
    m_basepath += '/';
#endif
    auto levelPath = m_basepath + m_resourceConfiguration->levelDir + "/";
    auto levels = fs.listFiles(levelPath);
#ifdef __APPLE__
    return levels;
#endif
    for (auto &level: levels)
        res.emplace_back(levelPath + level);
    return res;
}
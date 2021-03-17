/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
hakka_game - Zlib license.
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

#include <vector>
#include <list>
#include <map>

#include <SFML/Audio.hpp>

enum class AudioType{
    none,
    music,
    sound
};

class Audio{
public:
    static Audio* getInstanse();
    Audio(const Audio&)=delete;
    Audio(const Audio&&)=delete;
    Audio& operator=(const Audio&)=delete;
    Audio& operator=(const Audio&&)=delete;
    ~Audio() = default;

    bool loadFile(const char* filename, const char* id, AudioType type);

    void play(const char* id, bool looped = false);
    void stop(const char* id);

    void pause(const char* id, bool status);

    void setVolume(const char* id, const float& volume);
    const float& getVolume(const char* id) const;

    void update_sounds();
private:
    Audio() = default;

    AudioType getType(const char* id);
private:
    sf::Music m_music;
    std::list<sf::Sound> m_sounds;
    std::map<std::string, float> m_volumes;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, AudioType> m_audiotypes;
};



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
#include <vector>
#include <list>
#include <map>

#include <SFML/Audio.hpp>

enum class AudioType {
    music,
    sound
};

enum class AudioFileID {
    breakout,
    bleep,
    bleep_1,
    solid,
    power_up,
};

class AudioPlayer{
public:
    AudioPlayer();
    ~AudioPlayer() = default;

    bool loadFile(const std::string& filename, AudioFileID id, AudioType type);

    void play(AudioFileID, bool looped = false);
    void stop(AudioFileID);

    void pause(AudioFileID, bool status);

    void setVolume(AudioFileID, const float& volume);
    const float& getVolume(AudioFileID) const;

    void update_sounds();

private:
    AudioType getType(AudioFileID id);
private:
    sf::Music m_music;
    std::list<sf::Sound> m_sounds;
    std::map<AudioFileID, float> m_volumes;
    std::map<AudioFileID, sf::SoundBuffer> m_soundBuffers;
    std::map<AudioFileID, AudioType> m_audiotypes;
};

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

#include <cmath>
#include <stdexcept>
#include <SFML/Audio/Listener.hpp>

#include "game/Audio.h"

namespace
{
    // Sound coordinate system, point of view of a player in front of the screen:
    // X = left; Y = up; Z = back (out of the screen)
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D
                                        + ListenerZ*ListenerZ);
}

Audio* Audio::getInstanse() {
    static Audio m_instance;
    return &m_instance;
}


bool Audio::loadFile(const char* filename, const char* id, AudioType type) {
    if(type == AudioType::music) {
        if (!m_music.openFromFile(filename))
            throw std::runtime_error("Music " + std::string(filename) + " could not be loaded.");
    }

    if(type == AudioType::sound) {
        sf::SoundBuffer soundBuffer;
        if(!soundBuffer.loadFromFile(filename))
            return false;
        m_soundBuffers.insert(std::pair<std::string, sf::SoundBuffer>
                (id, soundBuffer));
    }

    m_audiotypes.insert(std::pair<std::string, AudioType>(id, type));
    return true;
}

void Audio::play(const char* id, bool looped) {
    if(getType(id) == AudioType::sound) {
        m_sounds.push_back(sf::Sound());
        sf::Sound &sound = m_sounds.back();

        sound.setBuffer(m_soundBuffers[id]);
        //sf::Vector3f position = sf::Listener::getPosition();
        //sound.setPosition(position.x, -position.y, 0.f);
        //sound.setAttenuation(Attenuation);
        //sound.setMinDistance(MinDistance3D);
        sound.setLoop(looped);
        sound.play();
    }
    if(getType(id) == AudioType::music) {
        m_music.setLoop(looped);
        m_music.play();
    }
}

void Audio::stop(const char* id) {
    if(getType(id) == AudioType::sound) {
    }
    if(getType(id) == AudioType::music) {
        m_music.stop();
    }
}


void Audio::pause(const char* id, bool status) {
    m_music.stop();
}

void Audio::setVolume(const char* id, const float& volume) {
    m_volumes[id] = volume;
    if(getType(id) == AudioType::music)
        m_music.setVolume(volume);

}

const float& Audio::getVolume(const char* id) const {
    return m_volumes.at(id);
}


void Audio::update_sounds() {
    m_sounds.remove_if([] (const sf::Sound& s)
                      {
                          return s.getStatus() == sf::Sound::Stopped;
                      });
}

AudioType Audio::getType(const char* id) {
    return m_audiotypes.at(id);
}

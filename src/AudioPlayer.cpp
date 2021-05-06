//
// Created by Necromant on 29.04.2021.
//

#include <cmath>
#include <stdexcept>

#include <SFML/Audio/Listener.hpp>
#include "game/AudioPlayer.hpp"

namespace {
    // Sound coordinate system, point of view of a player in front of the screen:
    // X = left; Y = up; Z = back (out of the screen)
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D
                                          + ListenerZ * ListenerZ);
}

AudioPlayer::AudioPlayer() {

}


bool AudioPlayer::loadFile(const std::string &filename, AudioFileID id, AudioType type) {
    if (type == AudioType::music) {
        if (!m_music.openFromFile(filename))
            throw std::runtime_error("Music " + std::string(filename) + " could not be loaded.");
    }

    if (type == AudioType::sound) {
        sf::SoundBuffer soundBuffer;
        if (!soundBuffer.loadFromFile(filename))
            return false;
        m_soundBuffers.insert(std::pair<AudioFileID, sf::SoundBuffer>
                                      (id, soundBuffer));
    }

    m_audiotypes.insert(std::pair<AudioFileID, AudioType>(id, type));
    return true;
}

void AudioPlayer::play(AudioFileID id, bool looped) {
    if (getType(id) == AudioType::sound) {
        m_sounds.emplace_back(sf::Sound());
        sf::Sound &sound = m_sounds.back();

        sound.setBuffer(m_soundBuffers[id]);
        sound.setLoop(looped);
        sound.play();
    }
    if (getType(id) == AudioType::music) {
        m_music.setLoop(looped);
        m_music.play();
    }
}

void AudioPlayer::stop(AudioFileID id) {
    if (getType(id) == AudioType::sound) {
    }
    if (getType(id) == AudioType::music)
        m_music.stop();
}


void AudioPlayer::pause(AudioFileID id, bool status) {
    m_music.stop();
}

void AudioPlayer::setVolume(AudioFileID id, const float &volume) {
    m_volumes[id] = volume;
    if (getType(id) == AudioType::music)
        m_music.setVolume(volume);
}

const float &AudioPlayer::getVolume(AudioFileID id) const {
    return m_volumes.at(id);
}


void AudioPlayer::update_sounds() {
    m_sounds.remove_if([](const sf::Sound &s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}

AudioType AudioPlayer::getType(AudioFileID id) {
    return m_audiotypes.at(id);
}
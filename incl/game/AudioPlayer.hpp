//
// Created by Necromant on 29.04.2021.
//

#pragma once
#include <vector>
#include <list>
#include <map>

#include <SFML/Audio.hpp>

enum class AudioType {
    none,
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

    bool loadFile(const char* filename, AudioFileID id, AudioType type);

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

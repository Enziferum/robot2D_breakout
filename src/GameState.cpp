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

#include <algorithm>
#include <ctime>

#include <robot2D/Core/Keyboard.hpp>
#include <robot2D/Util/Logger.hpp>

#include "game/GameState.hpp"
#include "game/States.hpp"
#include "game/FileManager.hpp"
#include "game/MessageIDs.hpp"

struct vec3 {
    float r;
    float g;
    float b;

    vec3(float _r, float _g, float _b) :
            r(_r), g(_g), b(_b) {}

    ~vec3() = default;
};

const auto emitter_offset = robot2D::vec2f(6.25f, 6.25f);
const vec3 wallbreakerColor = vec3(1.f, 0.5f, 0.5f);
const vec3 stickyColor = vec3(1.f, 0.5f, 1.f);


FileManager fm;

GameState::GameState(robot2D::IStateMachine &machine, AppContext<ContextID> &context, MessageBus &bus) :
        State(machine),
        m_context(context),
        m_audioPlayer(nullptr),
        m_gameConfiguration(nullptr),
        m_state(mState::Play),
        m_livesSprites(),
        m_bounceTimer(1.5f),
        m_bus(bus),
        m_world(m_bus, m_textures) {

    setup();
}


void GameState::setup_configuration() {
    m_audioPlayer = (AudioPlayer *) (m_context.getBuffer(ContextID::Audio));
    if (m_audioPlayer == nullptr) {
        LOG_ERROR_E("m_audioPlayer == nullptr, after getting")
        return;
    }

    auto configuration = (Configuration *) (m_context.getBuffer(ContextID::Configuration));
    m_gameConfiguration = &(configuration->getGameConfiguration());
    if (m_gameConfiguration == nullptr) {
        LOG_ERROR_E("m_gameConfiguration == nullptr, after getting")
        return;
    }

}

void GameState::setup_resources() {
    auto configuration = (Configuration *) (m_context.getBuffer(ContextID::Configuration));
    const auto resourceConfiguration = &(configuration->getResourceConfiguration());

    if (resourceConfiguration == nullptr) {
        LOG_ERROR_E("resourceConfiguration == nullptr, after getting")
        return;
    }

    fm.setConfiguration(*resourceConfiguration);

    m_textures.loadFromFile(ResourceIDs::Background,
                            fm.combinePath(ResourceType::Texture, "cityskyline.png"));

    m_textures.loadFromFile(ResourceIDs::Face,
                            fm.combinePath(ResourceType::Texture, "awesomeface.png"),
                            true);

    m_textures.loadFromFile(ResourceIDs::Block,
                            fm.combinePath(ResourceType::Texture, "block.png"));

    m_textures.loadFromFile(ResourceIDs::Solid,
                            fm.combinePath(ResourceType::Texture, "block_solid.png"));

    m_textures.loadFromFile(ResourceIDs::Paddle,
                            fm.combinePath(ResourceType::Texture, "paddle.png"), true);

    m_textures.loadFromFile(ResourceIDs::Particle,
                            fm.combinePath(ResourceType::Texture, "particle.png"), true);

    m_textures.loadFromFile(ResourceIDs::Chaos,
                            fm.combinePath(ResourceType::Texture, "powerup_chaos.png"), true);

    m_textures.loadFromFile(ResourceIDs::Confuse,
                            fm.combinePath(ResourceType::Texture, "powerup_confuse.png"), true);

    m_textures.loadFromFile(ResourceIDs::Size,
                            fm.combinePath(ResourceType::Texture, "powerup_increase.png"), true);

    m_textures.loadFromFile(ResourceIDs::Wallbreaker,
                            fm.combinePath(ResourceType::Texture, "powerup_passthrough.png"),
                            true);

    m_textures.loadFromFile(ResourceIDs::Speed,
                            fm.combinePath(ResourceType::Texture, "powerup_speed.png"),
                            true);

    m_textures.loadFromFile(ResourceIDs::Sticky,
                            fm.combinePath(ResourceType::Texture, "powerup_sticky.png"),
                            true);

    m_fonts.loadFromFile(ResourceIDs::Font, fm.combinePath(ResourceType::Font,
                                                           "game_font.ttf"));

    m_audioPlayer->loadFile(fm.combinePath(ResourceType::Audio,
                                           "bleep_1.wav"),
                            AudioFileID::bleep_1, AudioType::sound);
    m_audioPlayer->loadFile(fm.combinePath(ResourceType::Audio,
                                           "bleep.wav"),
                            AudioFileID::bleep, AudioType::sound);
    m_audioPlayer->loadFile(fm.combinePath(ResourceType::Audio,
                                           "solid.wav"),
                            AudioFileID::solid, AudioType::sound);
    m_audioPlayer->loadFile(fm.combinePath(ResourceType::Audio,
                                           "powerup.wav"),
                            AudioFileID::power_up, AudioType::sound);

    // resource loading //

    auto paths = fm.levelsPath();
    if (paths.empty())
        return;

    if (!m_world.setupLevels(paths, m_textures)) {
        return;
    }

    m_gameUI.setup(m_textures, *configuration);
}

void GameState::setup() {
    srand(time(nullptr));

    setup_configuration();
    setup_resources();

    m_bounceTimer.onTick([this](float dt) {
        (void) dt;
        m_bounceTimer.reset();
        m_state = mState::Play;
        auto msg = m_bus.post<LevelEvent>(messageIDs::LevelChangeEnd);
        msg->update_level = true;
    });

    if (!m_world.setup(m_gameConfiguration, m_audioPlayer)) {
        return;
    }
}

void GameState::handleEvents(const robot2D::Event &event) {
    if (event.type == robot2D::Event::Resized) {
        onResize(robot2D::vec2f(event.size.widht, event.size.heigth));
    }

    if (m_state == mState::Pause) {
        if (event.type == robot2D::Event::KeyPressed) {
            if (event.key.code == robot2D::Key::ESCAPE)
                m_state = mState::Play;
        }
        return;
    }

    if (m_state == mState::LevelChange) {

    }

    if (m_state == mState::Play) {
        if (event.type == robot2D::Event::KeyPressed) {
            if (event.key.code == robot2D::Key::ESCAPE) {
                //m_state = mState::Pause;
                m_machine.popState();
                m_machine.pushState(States::Pause);
            }
            if (event.key.code == robot2D::Key::Y)
                changeLevel();
        }

        if (event.type == robot2D::Event::MousePressed) {
            if (event.mouse.btn == robot2D::Event::MouseButtonEvent::left) {

            }
        }

        m_world.handleEvents(event);
    }
}

void GameState::onResize(const robot2D::vec2f &size) {
    //m_windowSize = robot2D::vec2u(size.x, size.y);
    //m_background.setScale(robot2D::vec2f(size.x, size.y));
    //m_levels[currlevel].onResize(size);
}

void GameState::forwardMessage(const Message &msg) {
    if (msg.id == messageIDs::LevelChange) {
        changeLevel();
    }

    if (msg.id == messageIDs::LivesEnd) {
        m_machine.pushState(States::Intro);
        return;
    }

    m_world.forwardMessage(msg);
    m_gameUI.handleMessage(msg);
}

void GameState::update(float dt) {
    while (!m_bus.empty()) {
        const auto &m = m_bus.poll();
        forwardMessage(m);
    }

    if (m_state == mState::Pause)
        return;

    if (m_state == mState::LevelChange) {
        m_bounceTimer.update(dt);
        return;
    }

    m_world.update(dt);
}

void GameState::changeLevel() {
    m_state = mState::LevelChange;
    m_won.setText("Congratulations! You won!");
}

void GameState::render() {
    //m_postProcessing.preRender();

    m_window.draw(m_world);
    m_window.draw(m_gameUI);

//    if(m_state == mState::LevelChange)
//        m_window.draw(m_won);

    //m_postProcessing.afterRender();
    //m_window.draw(m_postProcessing);
}





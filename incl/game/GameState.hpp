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

#include "robot2D/Extra/State.hpp"
#include "robot2D/Extra/IStateMachine.hpp"

#include "robot2D/Graphics/Sprite.hpp"
#include "robot2D/Util/ResourceHandler.hpp"
#include "robot2D/Graphics/Text.hpp"

#include "AppContext.hpp"
#include "ContextIDs.hpp"
#include "Configuration.hpp"
#include "IDs.hpp"

#include "AudioPlayer.hpp"
#include "InputManager.hpp"
#include "ParticleEmitter.hpp"
#include "ParallaxEffect.hpp"
#include "PostProcessing.hpp"

#include "Level.hpp"
#include "Timer.hpp"
#include "PowerupSystem.hpp"
#include "World.hpp"

#include "MessageBus.hpp"
#include "GameUI.hpp"

class GameState : public robot2D::State {
public:
    GameState(robot2D::IStateMachine &machine, AppContext<ContextID> &, MessageBus &bus);

    ~GameState() override = default;

    void handleEvents(const robot2D::Event &event) override;

    void update(float dt) override;

    void render() override;

private:
    void setup();

    void setup_configuration();

    void setup_resources();

    void forwardMessage(const Message &msg);

private:
    void onResize(const robot2D::vec2f &size);

    void changeLevel();

private:
    enum class mState {
        Play,
        Pause,
        LevelChange
    };

    mState m_state;

    MessageBus &m_bus;
    AppContext<ContextID> &m_context;
    GameConfiguration *m_gameConfiguration;
    GameUI m_gameUI;
    World m_world;
    AudioPlayer *m_audioPlayer;
    Timer m_bounceTimer;

    robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> m_textures;
    robot2D::ResourceHandler<robot2D::Font, ResourceIDs> m_fonts;

    robot2D::Text m_text;
    robot2D::Text m_won;
    std::vector<robot2D::Sprite> m_livesSprites;
};
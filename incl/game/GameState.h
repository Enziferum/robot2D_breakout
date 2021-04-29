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

#include "robot2D/Extra/State.h"
#include "robot2D/Extra/IStateMachine.h"

#include "robot2D/Graphics/Sprite.h"
#include "robot2D/Util/ResourceHandler.h"
#include "robot2D/Graphics/Text.h"


#include "InputManager.h"
#include "Level.h"
#include "ParticleEmitter.h"
#include "PostProcessing.h"
#include "ParallaxEffect.h"
#include "PowerUp.h"
#include "Timer.h"
#include "IDs.h"


#include "AppContext.hpp"
#include "ContextIDs.hpp"
#include "AudioPlayer.hpp"
#include "Configuration.hpp"


class GameState: public robot2D::State {
public:
    GameState(robot2D::IStateMachine& machine, AppContext<ContextID>& );
    ~GameState() override = default;

    void handleEvents(const robot2D::Event& event) override;
    void update(float dt) override;
    void render() override;
private:
    void setup();
    void load_resources();
    void onResize(const robot2D::vec2f& size);


    void process_input(float dt);
    void update_powerups(float dt);
    void process_collisions(float dt);

    void reset_game();
    void spawn_power_up(GameObject& box);
    void activate_power(PowerUp& power);

    void changeLevel();
private:
    AppContext<ContextID>& m_context;
    AudioPlayer* m_audioPlayer;
    GameConfiguration* m_gameConfiguration;


    enum class mState{
        Play,
        Pause,
        LevelChange
    };
    mState m_state;
    robot2D::vec2u m_windowSize;

    robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> m_textures;
    robot2D::ResourceHandler<robot2D::Font, ResourceIDs> m_fonts;
    robot2D::Sprite m_background;

    //effects

    PostProcessing m_postProcessing;
    ParticleEmitter m_particleEmitter;
    ParallaxEffect m_parallax;

    //effects

    GameObject m_paddle;
    BallObject m_ball;
    std::vector<PowerUp> m_power_ups;

    std::vector<Level> m_levels;
    unsigned int currlevel = 0;

    //gui stuff
    robot2D::Text m_text;
    std::vector<robot2D::Sprite> m_livesSprites;

    robot2D::Text m_won;
    int m_lives;
    //gui stuff

    InputManager inputManager;
    //part of input_manager
    bool m_keys[1024];
    bool m_keysProcessed[1024];

    // bounce animation //
    Timer m_bounceTimer;
};
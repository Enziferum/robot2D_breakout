/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
hakka - Zlib license.
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

#include "hakka/Sprite.h"
#include "hakka/ResourceHandler.h"
#include "hakka/Text.h"

#include "State.h"
#include "Level.h"
#include "ParticleEmitter.h"
#include "PostProcessing.h"
#include "PowerUp.h"

class GameState: public State{
public:
    using Ptr = std::shared_ptr<GameState>;
public:
    GameState(IStateMachine& machine);
    ~GameState() override;

    void handleEvents(const hakka::Event& event) override;
    void update(float dt) override;
    void render() override;

private:
    void process_input(float dt);
    void update_powerups(float dt);
    void setup();
    void process_collisions(float dt);

    void reset_game();
    void spawn_power_up(GameObject& box);
    void activate_power(PowerUp& power);
private:
    hakka::ResourceHandler<hakka::Texture> m_textures;
    hakka::ResourceHandler<hakka::Font> m_fonts;
    hakka::Sprite m_background;

    //effects
    PostProcessing m_postProcessing;
    ParticleEmitter m_particleEmitter;

    float shakeTime = 0.0f;

    GameObject m_paddle;
    BallObject m_ball;
    std::vector<PowerUp> m_power_ups;


    std::vector<Level> m_levels;
    unsigned int currlevel = 0;

    //part of input_manager
    bool m_keys[1024];
    bool m_keysProcessed[1024];

    //gui stuff
    hakka::Text m_text;
    int last_lives = 3;
    int m_lives = 3;

    bool m_pause;
};
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

#include <robot2D/Core/Keyboard.h>
#include <robot2D/Util/Logger.h>

#include "game/GameState.hpp"
#include "game/Collisions.hpp"

#include "game/States.hpp"

#include "game/FileManager.hpp"

GameState::GameState(robot2D::IStateMachine& machine, AppContext<ContextID>& context):
    State(machine),
    m_context(context),
    m_audioPlayer(nullptr),
    m_gameConfiguration(nullptr),
    m_keys(),
    m_keysProcessed() ,
    m_lives(),
    m_livesSprites(),
    m_bounceTimer(1.5f),
    m_state(mState::Play)
    {

    setup();
}

//todo rewrite spawn function
bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, PowerUpType type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.activated)
            if (powerUp.m_type == type)
                return true;
    }
    return false;
}

FileManager fm;

void GameState::load_resources() {
    auto configuration = (Configuration*)(m_context.getBuffer(ContextID::Configuration));
    const auto resourceConfiguration = &(configuration->getResourceConfiguration());

    if(resourceConfiguration == nullptr) {
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
                            fm.combinePath(ResourceType::Texture, "powerup_chaos.png"),true);

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

    m_audioPlayer -> loadFile(fm.combinePath(ResourceType::Audio,
                                             "bleep_1.wav"),
                              AudioFileID::bleep_1, AudioType::sound);
    m_audioPlayer -> loadFile(fm.combinePath(ResourceType::Audio,
                                             "bleep.wav"),
                              AudioFileID::bleep, AudioType::sound);
    m_audioPlayer -> loadFile(fm.combinePath(ResourceType::Audio,
                                             "solid.wav"),
                              AudioFileID::solid, AudioType::sound);
    m_audioPlayer -> loadFile(fm.combinePath(ResourceType::Audio,
                                             "powerup.wav"),
                              AudioFileID::power_up, AudioType::sound);

    // resource loading //

    auto paths = fm.levelsPath();
    if(paths.empty())
        return;


    for(int it = 1; it < paths.size(); ++it){
        Level level;
        level.loadLevel(paths[it], m_textures, robot2D::vec2f(m_windowSize.x,
                                                                 m_windowSize.y / 2),
                  robot2D::vec2f(0.f, 50.f));
        m_levels.push_back(level);
    }
}

void GameState::setup() {
    srand(time(0));

    auto size = m_window.get_size();
    m_windowSize = size;

    m_audioPlayer = (AudioPlayer*)(m_context.getBuffer(ContextID::Audio));
    if(m_audioPlayer == nullptr){
        LOG_ERROR_E("m_audioPlayer == nullptr, after getting")
        return;
    }

    auto configuration = (Configuration*)(m_context.getBuffer(ContextID::Configuration));
    m_gameConfiguration = &(configuration->getGameConfiguration());
    if(m_gameConfiguration == nullptr) {
        LOG_ERROR_E("m_gameConfiguration == nullptr, after getting")
        return;
    }

    m_lives = m_gameConfiguration -> max_lives;

    load_resources();


    m_text.setText("Lives:");
    m_text.setPos(robot2D::vec2f(650, 10));
    m_text.setFont(m_fonts.get(ResourceIDs::Font));
    m_won.setFont(m_fonts.get(ResourceIDs::Font));
    m_won.setPos(robot2D::vec2f(200, size.y / 2.f));


    m_postProcessing.set_size(m_windowSize);
    m_particleEmitter.setTexture(const_cast<robot2D::Texture &>(m_textures.get(ResourceIDs::Particle)));

    m_background.setTexture(m_textures.get(ResourceIDs::Background));
    m_background.setPosition(robot2D::vec2f(0.f, 0.f));
    m_background.setScale(robot2D::vec2f(size.x, size.y));

    m_paddle.m_sprite.setTexture(m_textures.get(ResourceIDs::Paddle));
    m_paddle.setPos(robot2D::vec2f(size.x / 2.f - m_gameConfiguration -> paddle_size.x / 2,
                    size.y - m_gameConfiguration -> paddle_size.y));
    m_paddle.setSize(m_gameConfiguration -> paddle_size);


    robot2D::vec2f ballPos = robot2D::vec2f(m_gameConfiguration -> paddle_size.x / 2.0f -
                                                    m_gameConfiguration ->ball_radius + m_paddle.m_pos.x,
                                                    - m_gameConfiguration -> ball_radius * 2.0f + m_paddle.m_pos.y);

    m_ball.m_sprite.setTexture(m_textures.get(ResourceIDs::Face));
    m_ball.velocity = m_gameConfiguration ->ball_velocity;
    m_ball.border = size.x;
    m_ball.radius = m_gameConfiguration ->ball_radius;
    m_ball.setPos(ballPos);
    m_ball.setSize(robot2D::vec2f(m_gameConfiguration ->ball_radius * 2.f,
                                  m_gameConfiguration ->ball_radius * 2.f));

    auto live_sz = robot2D::vec2f(robot2D::vec2f(30.f, 30.f));
    auto live_start_pos = robot2D::vec2f(m_text.getPos().x + 50, 5);

    for(int it = 0; it < m_gameConfiguration -> max_lives; ++it){
        robot2D::Sprite sprite;
        sprite.setTexture(m_textures.get(ResourceIDs::Face));
        sprite.setScale(live_sz);
        sprite.setPosition(robot2D::vec2f(live_start_pos.x + live_sz.x * it,
                                          live_start_pos.y));
        m_livesSprites.push_back(sprite);
    }

    m_bounceTimer.onTick([this](float dt){
        if(currlevel < m_levels.size())
            ++currlevel;
        m_bounceTimer.reset();
        m_state = mState::Play;
    });

}

void GameState::handleEvents(const robot2D::Event& event) {
    if (event.type == robot2D::Event::Resized) {
        onResize(robot2D::vec2f(event.size.widht, event.size.heigth));
    }

    if(m_state == mState::Pause) {
        if(event.type == robot2D::Event::KeyPressed) {
            if (event.key.code == robot2D::Key::ESCAPE)
                m_state = mState::Play;
        }
        return;
    }

    if(m_state == mState::LevelChange) {

    }

    if(m_state == mState::Play) {
        if (event.type == robot2D::Event::KeyPressed) {
            if (event.key.code == robot2D::Key::ESCAPE)
                m_state = mState::Pause;
            if (event.key.code == robot2D::Key::Y)
                changeLevel();
        }

        if (event.type == robot2D::Event::KeyPressed) {
            m_keys[event.key.code] = true;
        }

        if (event.type == robot2D::Event::KeyReleased) {
            m_keys[event.key.code] = false;
            m_keysProcessed[event.key.code] = false;
        }

        if (event.type == robot2D::Event::MousePressed) {
            if (event.mouse.btn == robot2D::Event::MouseButtonEvent::left) {
                robot2D::vec2f mouse_pos(event.mouse.x, event.mouse.y);
            }
        }
    }
}

// update stage //

void GameState::update(float dt) {
    if(m_state == mState::Pause)
        return;

    if(m_state == mState::LevelChange){
        m_bounceTimer.update(dt);
        return;
    }

    process_input(dt);
    process_collisions(dt);
    m_ball.move(dt);

    //need reset level
    if(m_ball.m_pos.y >= m_windowSize.y)
        reset_game();

    m_levels[currlevel].update(dt);
    m_parallax.update(dt);
    m_particleEmitter.update(dt, m_gameConfiguration ->emitter_new_sz, m_ball, robot2D::vec2f(6.25f,
                                                         6.25f));
    update_powerups(dt);

    if(m_levels[currlevel].destroyed())
        changeLevel();

    m_postProcessing.update(dt);
    m_audioPlayer -> update_sounds();
}

void GameState::process_input(float dt) {
    // input update //
    float velocity = dt * m_gameConfiguration ->speed;

    if(m_keys[robot2D::A]) {
        if (m_paddle.m_pos.x >= 0.f) {
            m_paddle.m_pos.x -= velocity;
            m_paddle.m_sprite.setPosition(m_paddle.m_pos);
            if (m_ball.stuck) {
                m_ball.m_pos.x -= velocity;
                m_ball.m_sprite.setPosition(m_ball.m_pos);
            }
        }
    }

    if(m_keys[robot2D::D]) {
        if (m_paddle.m_pos.x <= m_windowSize.x
                                - m_paddle.m_size.x) {
            m_paddle.m_pos.x += velocity;
            m_paddle.m_sprite.setPosition(m_paddle.m_pos);

            if (m_ball.stuck) {
                m_ball.m_pos.x += velocity;
                m_ball.m_sprite.setPosition(m_ball.m_pos);
            }
        }
    }

    if(m_keys[robot2D::SPACE]) {
        m_ball.stuck = false;
    }

}

void GameState::update_powerups(float dt) {
    for (auto& it: m_power_ups){

        auto vel_copy = it.velocity;
        it.m_pos += vel_copy * dt;
        it.setPos(it.m_pos);

        it.duration -= dt;

        if(it.duration <= 0.0f){
            it.activated = false;
            switch (it.m_type) {
                case PowerUpType::chaos:
                    if(IsOtherPowerUpActive(m_power_ups, PowerUpType::chaos))
                        break;
                    m_postProcessing.setValue("chaos", false);
                    break;
                case PowerUpType::confuse:
                    if(IsOtherPowerUpActive(m_power_ups, PowerUpType::confuse))
                        break;
                    m_postProcessing.setValue("confuse", false);
                    break;
                case PowerUpType::wallbreaker:
                    if(IsOtherPowerUpActive(m_power_ups, PowerUpType::wallbreaker))
                        break;
                    m_ball.color = robot2D::Color::White;
                    m_ball.wallbreaker = false;
                    break;
                case PowerUpType::sticky:
                    if(IsOtherPowerUpActive(m_power_ups, PowerUpType::sticky))
                        break;
                    m_ball.sticky = false;
                    m_ball.color = robot2D::Color::White;
                    break;
                default:
                    break;
            }
        }
    }

    m_power_ups.erase(std::remove_if(m_power_ups.begin(),
                                     m_power_ups.end(), [](const PowerUp& powerUp){
                return powerUp.m_destroyed && !powerUp.activated;
            }),m_power_ups.end());
}

void GameState::process_collisions(float dt) {
    for(auto& box: m_levels[currlevel].getTiles()){
        if(box.m_destroyed)
            continue;

        auto collision = checkCollision(m_ball, box);
        //collide
        if(std::get<0>(collision)) {

            if (!box.m_solid) {
                spawn_power_up(box);
                box.m_destroyed = true;
                m_audioPlayer->play(AudioFileID::bleep_1);
            } else {
                m_postProcessing.setValue("shake", true);
                m_audioPlayer->play(AudioFileID::solid);
            }


            auto dir = std::get<1>(collision);
            auto vec = std::get<2>(collision);

            if (!m_ball.wallbreaker && !m_ball.m_solid) {
                if (dir == Direction::left || dir == Direction::right) {
                    m_ball.velocity.x = -m_ball.velocity.x;
                    float penetration = m_ball.radius - std::abs(vec.x);
                    if (dir == Direction::left)
                        m_ball.m_pos.x += penetration;
                    else
                        m_ball.m_pos.x -= penetration;
                } else {
                    m_ball.velocity.y = -m_ball.velocity.y;
                    float penetration = m_ball.radius - std::abs(vec.y);
                    if (dir == Direction::up)
                        m_ball.m_pos.y -= penetration;
                    else
                        m_ball.m_pos.y += penetration;
                }
            }
        }
        collision = checkCollision(m_ball, m_paddle);

        if(!m_ball.stuck && std::get<0>(collision)) {
            float centerBoard = m_paddle.m_pos.x + m_paddle.m_size.x / 2.0f;
            float distance = (m_ball.m_pos.x + m_ball.radius) - centerBoard;
            float percentage = distance / (m_paddle.m_size.x / 2.0f);
            // then move accordingly

            float strength = 2.f;
            robot2D::vec2f oldVelocity = m_ball.velocity;
            m_ball.velocity.x = 100.f * percentage * strength;
            m_ball.velocity.y = -1.f * std::abs(-m_ball.velocity.y);
            m_ball.velocity = normalize(m_ball.velocity) * length(oldVelocity);
        }
    }

    for(auto& powerit: m_power_ups){
        auto collision = checkCollision(powerit, m_paddle);
        if(powerit.m_pos.y >= m_windowSize.y)
            powerit.m_destroyed = true;

        if(collision) {
            activate_power(powerit);
            powerit.m_destroyed = true;
            powerit.activated = true;
        }
    }
}

void GameState::reset_game() {
    if(m_lives == 0) {
        //m_machine.pushState(States::Intro);
        m_lives = m_gameConfiguration -> max_lives;
        return;
    }

    {
        --m_lives;
    }
    m_power_ups.clear();

    m_postProcessing.setValue("chaos", false);
    m_postProcessing.setValue("confuse", false);


    m_paddle.setPos(robot2D::vec2f(m_windowSize.x / 2.f - m_gameConfiguration ->paddle_size.x / 2,
                                   m_windowSize.y - m_gameConfiguration ->paddle_size.y));
    robot2D::vec2f ballPos = robot2D::vec2f(m_gameConfiguration ->paddle_size.x / 2.0f -
                                                m_gameConfiguration ->ball_radius + m_paddle.m_pos.x,
                                         -m_gameConfiguration ->ball_radius * 2.0f + m_paddle.m_pos.y);
    m_ball.stuck = true;
    m_ball.setPos(ballPos);
}

void GameState::spawn_power_up(GameObject& box) {
    PowerUp power_up;
    if (ShouldSpawn(75)) {
      power_up.m_type = PowerUpType::speed;
      power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Speed));
      power_up.color = robot2D::Color::from_gl(0.5f, 0.5f, 1.0f);
      power_up.duration = 0.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::sticky;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Sticky));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.5f, 1.0f);
        power_up.duration = 20.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::wallbreaker;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Wallbreaker));
        power_up.color = robot2D::Color::from_gl(0.5f, 1.0f, 0.5f);
        power_up.duration = 10.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::size;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Size));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.6f, 0.4f);
        power_up.duration = 0.f;
    }
    if (ShouldSpawn(15)) {
        power_up.m_type = PowerUpType::confuse;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Confuse));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.3f, 0.3f);
        power_up.duration = 15.f;
    }

    if (ShouldSpawn(15)){
        power_up.m_type = PowerUpType::chaos;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Chaos));
        power_up.color = robot2D::Color::from_gl(0.9f, 0.25f, 0.25f);
        power_up.duration = 15.f;
    }
    power_up.setPos(box.m_pos);
    power_up.setSize(robot2D::vec2f(60.0f, 20.0f));
    power_up.velocity = robot2D::vec2f(0.0f, 150.f);
    m_power_ups.emplace_back(power_up);
}

void GameState::activate_power(PowerUp& power) {
    switch (power.m_type) {
        case PowerUpType::chaos:
            m_postProcessing.setValue("chaos", true);
        case PowerUpType::confuse:
            m_postProcessing.setValue("confuse", true);
            break;
        case PowerUpType::wallbreaker:
            m_ball.color = robot2D::Color::from_gl(1.0f, 0.5f, 0.5f);
            m_ball.wallbreaker = true;
            break;
        case PowerUpType::sticky:
            //
            m_ball.color = robot2D::Color::from_gl(1.0f, 0.5f, 1.0f);
            m_ball.sticky = true;
        case PowerUpType::speed:
            m_ball.velocity = m_ball.velocity * 1.2;
            break;
        case PowerUpType::size:
            m_paddle.m_size.x += 50;
            m_paddle.setSize(m_paddle.m_size);
            break;
        default:
            break;
    }
}

void GameState::changeLevel() {
    m_state = mState::LevelChange;
    m_won.setText("Congratulations! You won!");
    reset_game();
}

// update stage //

void GameState::onResize(const robot2D::vec2f& size) {
    m_windowSize = robot2D::vec2u(size.x, size.y);
    m_background.setScale(robot2D::vec2f(size.x, size.y));
    //m_levels[currlevel].onResize(size);
}

void GameState::render() {
    //m_postProcessing.preRender();
    m_window.draw(m_parallax);
    m_window.draw(m_background);
    m_window.draw(m_levels[currlevel]);
    m_window.draw(m_paddle);
    m_window.draw(m_particleEmitter);
    m_window.draw(m_ball);

    for(auto& it:m_power_ups)
        m_window.draw(it);
    for(int it = 0; it < m_lives; ++it)
        m_window.draw( m_livesSprites[it]);
    m_window.draw(m_text);
    if(m_state == mState::LevelChange)
        m_window.draw(m_won);

    //m_postProcessing.afterRender();
    //m_window.draw(m_postProcessing);
}




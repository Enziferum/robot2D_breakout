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

#include <iostream>
#include <algorithm>
#include <ctime>

//todo own keys
#include <GLFW/glfw3.h>

#include "game/GameState.h"
#include "game/Collisions.h"
#include "game/Audio.h"
#include "game/States.h"

constexpr float speed = 500.f;
const float ball_radius = 12.5f;

GameState::GameState(robot2D::IStateMachine& machine):
    State(machine), m_keys(), m_keysProcessed() ,
    m_pause(false){
    std::cout << "Try to setup" <<std::endl;
    setup();
}

GameState::~GameState() {
}


void GameState::setup() {

    srand( time(0) );

    //load textures
    m_textures.loadFromFile("background", "res/textures/cityskyline.png");
    m_textures.loadFromFile("face","res/textures/awesomeface.png", true);
    m_textures.loadFromFile("block", "res/textures/block.png");
    m_textures.loadFromFile("block_solid", "res/textures/block_solid.png");
    m_textures.loadFromFile("paddle", "res/textures/paddle.png", true);
    m_textures.loadFromFile("particle", "res/textures/particle.png", true);

    //powerups
    m_textures.loadFromFile("chaos", "res/textures/powerup_chaos.png", true);
    m_textures.loadFromFile("confuse", "res/textures/powerup_confuse.png", true);
    m_textures.loadFromFile("size", "res/textures/powerup_increase.png", true);
    m_textures.loadFromFile("wallbreaker", "res/textures/powerup_passthrough.png", true);
    m_textures.loadFromFile("speed", "res/textures/powerup_speed.png", true);
    m_textures.loadFromFile("sticky", "res/textures/powerup_sticky.png", true);
    //powerups

    m_fonts.loadFromFile("game_font", "res/fonts/game_font.ttf");
    m_text.setText("Lives: " + std::to_string(m_lives));
    m_text.setPos(robot2D::vec2f(0, 10));
    m_text.setFont(m_fonts.get("game_font"));

    m_postProcessing.set_size(m_window.get_size());
    m_particleEmitter.setTexture(const_cast<robot2D::Texture &>(m_textures.get("particle")));
    auto size = m_window.get_size();

    //todo load all from special folder
    Level zero;
    Level one;
    Level two;
    Level three;
    Level four;

    //zero.loadLevel("res/levels/0.lvl", robot2D::vec2f(size.x, size.y / 2), m_textures);
    one.loadLevel("res/levels/1.lvl", robot2D::vec2f(size.x, size.y / 2), m_textures);
    two.loadLevel("res/levels/2.lvl", robot2D::vec2f(size.x, size.y / 2), m_textures);
    three.loadLevel("res/levels/3.lvl", robot2D::vec2f(size.x, size.y / 2), m_textures);
    four.loadLevel("res/levels/4.lvl", robot2D::vec2f(size.x, size.y / 2), m_textures);

    //m_levels.push_back(zero);
    m_levels.push_back(one);
    m_levels.push_back(two);
    m_levels.push_back(three);
    m_levels.push_back(four);

    //todo load all from special folder

    m_background.setTexture(m_textures.get("background"));
    m_background.setPosition(robot2D::vec2f(0.f, 0.f));
    m_background.setScale(robot2D::vec2f(size.x, size.y));

    m_paddle.m_sprite.setTexture(m_textures.get("paddle"));
    robot2D::vec2f paddle_size(100.f, 20.f);
    m_paddle.setPos(robot2D::vec2f(size.x / 2.f - paddle_size.x / 2,
                    size.y - paddle_size.y));
    m_paddle.setSize(paddle_size);



    robot2D::vec2f ballPos =  robot2D::vec2f(paddle_size.x / 2.0f - ball_radius + m_paddle.m_pos.x,
                                                    -ball_radius * 2.0f + m_paddle.m_pos.y);
    m_ball.m_sprite.setTexture(m_textures.get("face"));
    m_ball.velocity = robot2D::vec2f(100.0f, -350.0f);
    m_ball.border = size.x;
    m_ball.radius = ball_radius;
    m_ball.setPos(ballPos);
    m_ball.setSize(robot2D::vec2f(ball_radius * 2.f, ball_radius * 2.f));

    Audio::getInstanse() -> loadFile("res/audio/bleep_1.wav",
                                        "bleep_1", AudioType::sound);
    Audio::getInstanse() -> loadFile("res/audio/bleep.wav",
                                    "bleep", AudioType::sound);
    Audio::getInstanse() -> loadFile("res/audio/solid.wav",
                                    "solid", AudioType::sound);
    Audio::getInstanse() -> loadFile("res/audio/powerup.wav",
                                    "power_up", AudioType::sound);
}


void GameState::handleEvents(const robot2D::Event& event) {
    if(m_pause) {
        if(event.type == robot2D::Event::KeyPressed) {
            if (event.key.code == GLFW_KEY_ESCAPE)
                m_pause = false;
        }
        return;
    }

    if(event.type == robot2D::Event::KeyPressed){
        if (event.key.code == GLFW_KEY_ESCAPE)
            m_pause = true;
    }


    if(event.type == robot2D::Event::KeyPressed){
        m_keys[event.key.code] = true;
    }

    if(event.type == robot2D::Event::KeyReleased){
        m_keys[event.key.code] = false;
        m_keysProcessed[event.key.code] = false;
    }


    if(event.type == robot2D::Event::MouseButton){
        if(event.mouse.btn == robot2D::Event::MouseButtonEvent::left){
            robot2D::vec2f mouse_pos(event.mouse.x, event.mouse.y);
            std::cout << event.mouse.x << ":" << event.mouse.y << std::endl;
        }
    }

}

void GameState::update(float dt) {
    if(m_pause)
        return;
    process_input(dt);
    process_collisions(dt);
    m_ball.move(dt);

    //need reset level
    if(m_ball.m_pos.y >= 600)
        reset_game();

    if(last_lives != m_lives){
        m_text.setText("Lives: " + std::to_string(m_lives));
        last_lives = m_lives;
    }

    m_levels[currlevel].update(dt);
    m_parallax.update(dt);
    m_particleEmitter.update(dt, 2, m_ball, robot2D::vec2f(6.25f,
                                                         6.25f));
    update_powerups(dt);
    if(m_levels[currlevel].destroyed())
        changeLevel();
    m_postProcessing.update(dt);
    Audio::getInstanse() -> update_sounds();
}


void GameState::process_input(float dt) {
    // input update //
    float velocity = dt * speed;

    if(m_keys[GLFW_KEY_A]) {
        if (m_paddle.m_pos.x >= 0.f) {
            m_paddle.m_pos.x -= velocity;
            m_paddle.m_sprite.setPosition(m_paddle.m_pos);
            if (m_ball.stuck) {
                m_ball.m_pos.x -= velocity;
                m_ball.m_sprite.setPosition(m_ball.m_pos);
            }
        }
    }

    if(m_keys[GLFW_KEY_D]) {
        if (m_paddle.m_pos.x <= 800 - m_paddle.m_size.x) {
            m_paddle.m_pos.x += velocity;
            m_paddle.m_sprite.setPosition(m_paddle.m_pos);

            if (m_ball.stuck) {
                m_ball.m_pos.x += velocity;
                m_ball.m_sprite.setPosition(m_ball.m_pos);
            }
        }
    }

    if(m_keys[GLFW_KEY_SPACE]){
        m_ball.stuck = false;
    }
    // update input //
}

bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, PowerUpType type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const PowerUp &powerUp : powerUps)
    {
        if (powerUp.activated)
            if (powerUp.m_type == type)
                return true;
    }
    return false;
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

void GameState::render() {
    m_postProcessing.preRender();
    m_window.draw(m_parallax);
    m_window.draw(m_background);
    m_window.draw(m_levels[currlevel]);
    m_window.draw(m_paddle);
    m_window.draw(m_particleEmitter);
    m_window.draw(m_ball);
    for(auto& it:m_power_ups)
        m_window.draw(it);
    m_postProcessing.afterRender();
    m_window.draw(m_postProcessing);
    m_window.draw(m_text);
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
                Audio::getInstanse()->play("bleep_1");
            } else {
                m_postProcessing.setValue("shake", true);
                Audio::getInstanse()->play("solid");
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
        if(!m_ball.stuck && std::get<0>(collision)){
            float centerBoard = m_paddle.m_pos.x + m_paddle.m_size.x / 2.0f;
            float distance = (m_ball.m_pos.x + m_ball.radius) - centerBoard;
            float percentage = distance / (m_paddle.m_size.x / 2.0f);
            // then move accordingly

            float strength = 2.0f;
            robot2D::vec2f oldVelocity = m_ball.velocity;
            m_ball.velocity.x = 100.f * percentage * strength;
            m_ball.velocity.y = -1.0f * std::abs(-m_ball.velocity.y);
            m_ball.velocity = normalize(m_ball.velocity) * length(oldVelocity);
            //Audio::getInstanse() -> play("solid");
        }
    }

    for(auto& powerit: m_power_ups){
        auto collision = checkCollision(powerit, m_paddle);
        if(powerit.m_pos.y >= 600)
            powerit.m_destroyed = true;

        if(collision == true) {
            activate_power(powerit);
            powerit.m_destroyed = true;
            powerit.activated = true;
        }
    }
}

void GameState::reset_game() {
    if(m_lives == 0){
        m_machine.pushState(States::Intro);
        m_lives = 3;
        return;
    }

    --m_lives;
    m_postProcessing.setValue("chaos", false);
    m_postProcessing.setValue("confuse", false);


    auto size = m_window.get_size();
    robot2D::vec2f paddle_size(100.f, 20.f);
    m_paddle.setPos(robot2D::vec2f(size.x / 2.f - paddle_size.x / 2,
                                 size.y - paddle_size.y));
    robot2D::vec2f ballPos = robot2D::vec2f(paddle_size.x / 2.0f - ball_radius + m_paddle.m_pos.x,
                                         -ball_radius * 2.0f + m_paddle.m_pos.y);
    m_ball.stuck = true;
    m_ball.setPos(ballPos);
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}

void GameState::spawn_power_up(GameObject& box) {
    PowerUp power_up;
    if (ShouldSpawn(75)){
      power_up.m_type = PowerUpType::speed;
      power_up.m_sprite.setTexture(m_textures.get("speed"));
      power_up.color = robot2D::Color::from_gl(0.5f, 0.5f, 1.0f);
      power_up.duration = 0.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::sticky;
        power_up.m_sprite.setTexture(m_textures.get("sticky"));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.5f, 1.0f);
        power_up.duration = 20.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::wallbreaker;
        power_up.m_sprite.setTexture(m_textures.get("wallbreaker"));
        power_up.color = robot2D::Color::from_gl(0.5f, 1.0f, 0.5f);
        power_up.duration = 10.f;
    }
    if (ShouldSpawn(75)){
        power_up.m_type = PowerUpType::size;
        power_up.m_sprite.setTexture(m_textures.get("size"));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.6f, 0.4f);
        power_up.duration = 0.f;
    }
    if (ShouldSpawn(15)) {
        power_up.m_type = PowerUpType::confuse;
        power_up.m_sprite.setTexture(m_textures.get("confuse"));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.3f, 0.3f);
        power_up.duration = 15.f;
    }// Negative powerups should spawn more often

    if (ShouldSpawn(15)){
        power_up.m_type = PowerUpType::chaos;
        power_up.m_sprite.setTexture(m_textures.get("chaos"));
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
            //
            m_ball.color = robot2D::Color::from_gl(1.0f, 0.5f, 0.5f);
           // m_ball.wallbreaker = true;
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

    currlevel++;
    m_power_ups.clear();

    m_postProcessing.setValue("chaos", false);
    m_postProcessing.setValue("confuse", false);


    auto size = m_window.get_size();
    robot2D::vec2f paddle_size(100.f, 20.f);
    m_paddle.setPos(robot2D::vec2f(size.x / 2.f - paddle_size.x / 2,
                                 size.y - paddle_size.y));
    robot2D::vec2f ballPos = robot2D::vec2f(paddle_size.x / 2.0f - ball_radius + m_paddle.m_pos.x,
                                        -ball_radius * 2.0f + m_paddle.m_pos.y);
    m_ball.stuck = true;
    m_ball.setPos(ballPos);
}



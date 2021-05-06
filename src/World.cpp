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

#include <robot2D/Graphics/RenderTarget.h>
#include <robot2D/Core/Keyboard.h>

#include "game/World.hpp"
#include "game/Collisions.hpp"
#include "game/MessageIDs.hpp"


const float add_size = 50.f;
const float speed_multiplier = 1.2;
const float strength = 2.f;
const float collider_multipier = 100.f;
const auto emitter_offset = robot2D::vec2f(6.25f, 6.25f);
const unsigned score_multiplier = 10;

World::World(MessageBus &messageBus, robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures) :
        m_messageBus(messageBus),
        m_audioPlayer(nullptr),
        m_gameConfiguration(nullptr),
        m_keys(),
        m_keysProcessed(),
        m_textures(textures) {

    m_windowSize.x = 800;
    m_windowSize.y = 600;
}


bool World::setup(GameConfiguration *gameConfiguration, AudioPlayer *audioPlayer) {

    m_audioPlayer = audioPlayer;
    m_gameConfiguration = gameConfiguration;

    m_windowSize = robot2D::vec2u(800, 600);
    auto size = m_windowSize;

    m_powerupSystem.setCallback([this](const PowerUpType &type) {
        switch (type) {
            case PowerUpType::chaos:
                m_postProcessing.setValue("chaos", false);
                break;
            case PowerUpType::confuse:
                m_postProcessing.setValue("confuse", false);
                break;
            case PowerUpType::wallbreaker:
                m_ball.color = robot2D::Color::White;
                m_ball.wallbreaker = false;
                break;
            case PowerUpType::sticky:
                m_ball.sticky = false;
                m_ball.color = robot2D::Color::White;
                break;
            default:
                break;
        }
    });

    m_postProcessing.set_size(m_windowSize);

    m_particleEmitter.setTexture(const_cast<robot2D::Texture & >(m_textures.get(ResourceIDs::Particle)));

    m_background.setTexture(m_textures.get(ResourceIDs::Background));
    m_background.setScale(robot2D::vec2f(size.x, size.y));

    m_paddle.m_sprite.setTexture(m_textures.get(ResourceIDs::Paddle));
    m_paddle.setPos(robot2D::vec2f(size.x / 2.f - m_gameConfiguration->paddle_size.x / 2,
                                   size.y - m_gameConfiguration->paddle_size.y));
    m_paddle.setSize(m_gameConfiguration->paddle_size);


    robot2D::vec2f ballPos = robot2D::vec2f(m_gameConfiguration->paddle_size.x / 2.f -
                                            m_gameConfiguration->ball_radius + m_paddle.m_pos.x,
                                            -m_gameConfiguration->ball_radius * 2.f + m_paddle.m_pos.y);

    m_ball.m_sprite.setTexture(m_textures.get(ResourceIDs::Face));
    m_ball.velocity = m_gameConfiguration->ball_velocity;
    m_ball.border = size.x;
    m_ball.radius = m_gameConfiguration->ball_radius;
    m_ball.setPos(ballPos);
    m_ball.setSize(robot2D::vec2f(m_gameConfiguration->ball_radius * 2.f,
                                  m_gameConfiguration->ball_radius * 2.f));

    m_lives = m_gameConfiguration->max_lives;

    return true;
}


bool World::setupLevels(const std::vector<std::string> &paths,
                        robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures) {

    for (size_t it = 1; it < paths.size(); ++it) {
        Level level;
        level.loadLevel(paths[it], textures, robot2D::vec2f(m_windowSize.x,
                                                            m_windowSize.y / 2),
                        robot2D::vec2f(0.f, 50.f));
        m_levels.push_back(level);
    }

    return true;
}

void World::handleEvents(const robot2D::Event &event) {
    if (event.type == robot2D::Event::KeyPressed) {
        m_keys[event.key.code] = true;
    }

    if (event.type == robot2D::Event::KeyReleased) {
        m_keys[event.key.code] = false;
        m_keysProcessed[event.key.code] = false;
    }
}

void World::forwardMessage(const Message &message) {
    if (message.id == messageIDs::LevelChangeEnd) {
        auto msg = message.unpack<LevelEvent>();
        if (currlevel < m_levels.size() - 1 && msg.update_level) {
            ++currlevel;
        }
    }
}

void World::update(float dt) {
    float m_border = m_windowSize.y;

    process_input(dt);
    process_collisions();
    m_ball.move(dt);

    m_levels[currlevel].update(dt);
    m_parallax.update(dt);
    //m_particleEmitter.update(dt, m_gameConfiguration -> emitter_new_sz, m_ball, emitter_offset1);
    m_postProcessing.update(dt);
    m_audioPlayer->update_sounds();
    m_powerupSystem.update(dt);

    if (m_ball.m_pos.y >= m_border)
        reset_game();

    if (m_levels[currlevel].destroyed())
        change_level();
}

void World::process_input(float dt) {
    float velocity = dt * m_gameConfiguration->speed;

    if (m_keys[robot2D::A]) {
        if (m_paddle.m_pos.x >= 0.f) {
            m_paddle.m_pos.x -= velocity;
            m_paddle.m_sprite.setPosition(m_paddle.m_pos);
            if (m_ball.stuck) {
                m_ball.m_pos.x -= velocity;
                m_ball.m_sprite.setPosition(m_ball.m_pos);
            }
        }
    }

    if (m_keys[robot2D::D]) {
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

    if (m_keys[robot2D::SPACE]) {
        m_ball.stuck = false;
    }
}

void World::process_collisions() {

    for (auto &box: m_levels[currlevel].getTiles()) {
        if (box.m_destroyed)
            continue;

        auto collision = isCollide(m_ball, box);

        if (std::get<0>(collision)) {

            if (!box.m_solid) {
                m_powerupSystem.spawn_powerup(m_textures, box.m_pos);
                box.m_destroyed = true;
                auto msg = m_messageBus.post<ScoreEvent>(messageIDs::ScoreUpdate);
                msg->new_score = box.block_id * score_multiplier;
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

        collision = isCollide(m_ball, m_paddle);

        if (!m_ball.stuck && std::get<0>(collision)) {
            float centerBoard = m_paddle.m_pos.x + m_paddle.m_size.x / 2.f;
            float distance = (m_ball.m_pos.x + m_ball.radius) - centerBoard;
            float percentage = distance / (m_paddle.m_size.x / 2.f);


            robot2D::vec2f oldVelocity = m_ball.velocity;
            m_ball.velocity.x = collider_multipier * percentage * strength;
            m_ball.velocity.y = -1.f * std::abs(-m_ball.velocity.y);
            m_ball.velocity = normalize(m_ball.velocity) * length(oldVelocity);
        }
    }

    for (auto &powerit: m_powerupSystem.get()) {
        auto collision = isCollide(powerit, m_paddle);
        if (powerit.m_pos.y >= m_windowSize.y)
            powerit.m_destroyed = true;

        if (collision) {
            activate_power(powerit);
            powerit.m_destroyed = true;
            powerit.activated = true;
        }
    }

}

void World::activate_power(PowerUp &power) {
    switch (power.m_type) {
        case PowerUpType::chaos:
            m_postProcessing.setValue("chaos", true);
            break;
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
            break;
        case PowerUpType::speed:
            m_ball.velocity = m_ball.velocity * speed_multiplier;
            break;
        case PowerUpType::size:
            m_paddle.m_size.x += add_size;
            m_paddle.setSize(m_paddle.m_size);
            break;
        default:
            break;
    }
}

void World::reset_game(bool update_lifes) {
    if (m_lives == 0) {
        m_lives = m_gameConfiguration->max_lives;
        auto msg = m_messageBus.post<LivesEndEvent>(messageIDs::LivesEnd);
        return;
    }

    if (update_lifes) {
        --m_lives;
        auto msg = m_messageBus.post<LivesEvent>(messageIDs::LivesUpdate);
        msg->new_lives = m_lives;
    }

    m_powerupSystem.get().clear();

    m_postProcessing.setValue("chaos", false);
    m_postProcessing.setValue("confuse", false);

    m_paddle.setPos(robot2D::vec2f(m_windowSize.x / 2.f - m_gameConfiguration->paddle_size.x / 2,
                                   m_windowSize.y - m_gameConfiguration->paddle_size.y));

    robot2D::vec2f ballPos = robot2D::vec2f(m_gameConfiguration->paddle_size.x / 2.f -
                                            m_gameConfiguration->ball_radius + m_paddle.m_pos.x,
                                            -m_gameConfiguration->ball_radius * 2.f + m_paddle.m_pos.y);
    m_ball.stuck = true;
    m_ball.setPos(ballPos);
}

void World::change_level() {
    reset_game(false);
    auto msg = m_messageBus.post<LevelEvent>(messageIDs::LevelChange);
}


void World::draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const {
    target.draw(m_parallax);
    target.draw(m_background);
    target.draw(m_levels[currlevel]);
    target.draw(m_particleEmitter);
    target.draw(m_paddle);
    target.draw(m_ball);
    target.draw(m_powerupSystem);
}








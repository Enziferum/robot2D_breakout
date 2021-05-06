//
// Created by Necromant on 02.05.2021.
//

#include <robot2D/Graphics/RenderTarget.hpp>
#include <robot2D/Graphics/RenderStates.hpp>

#include <game/PowerupSystem.hpp>

const robot2D::vec2f powerup_size = robot2D::vec2f(60.0f, 20.0f);
const robot2D::vec2f powerup_velocity = robot2D::vec2f(0.f, 150.f);


PowerupSystem::PowerupSystem() :
        m_power_ups() {
}

void PowerupSystem::update(float dt) {
    for (auto &it: m_power_ups) {

        auto vel_copy = it.velocity;
        it.m_pos += vel_copy * dt;
        it.setPos(it.m_pos);

        it.duration -= dt;

        if (it.duration <= 0.0f) {
            it.activated = false;
            if (otherActive(it.m_type))
                break;

            m_func(it.m_type);
        }
    }

    m_power_ups.erase(std::remove_if(m_power_ups.begin(),
                                     m_power_ups.end(), [](const PowerUp &powerUp) {
                return powerUp.m_destroyed && !powerUp.activated;
            }), m_power_ups.end());
}


std::vector<PowerUp> &PowerupSystem::get() {
    return m_power_ups;
}


void PowerupSystem::spawn_powerup(robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &m_textures,
                                  const robot2D::vec2f &spawn_pos) {
    PowerUp power_up;

    if (randomize(75)) {
        power_up.m_type = PowerUpType::speed;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Speed));
        power_up.color = robot2D::Color::from_gl(0.5f, 0.5f, 1.0f);
        power_up.duration = 0.f;
    }

    if (randomize(75)) {
        power_up.m_type = PowerUpType::sticky;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Sticky));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.5f, 1.0f);
        power_up.duration = 20.f;
    }

    if (randomize(75)) {
        power_up.m_type = PowerUpType::wallbreaker;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Wallbreaker));
        power_up.color = robot2D::Color::from_gl(0.5f, 1.0f, 0.5f);
        power_up.duration = 10.f;
    }

    if (randomize(20)) {
        power_up.m_type = PowerUpType::size;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Size));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.6f, 0.4f);
        power_up.duration = 0.f;
    }

    if (randomize(15)) {
        power_up.m_type = PowerUpType::confuse;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Confuse));
        power_up.color = robot2D::Color::from_gl(1.0f, 0.3f, 0.3f);
        power_up.duration = 15.f;
    }

    if (randomize(15)) {
        power_up.m_type = PowerUpType::chaos;
        power_up.m_sprite.setTexture(m_textures.get(ResourceIDs::Chaos));
        power_up.color = robot2D::Color::from_gl(0.9f, 0.25f, 0.25f);
        power_up.duration = 15.f;
    }

    power_up.setPos(spawn_pos);
    power_up.setSize(powerup_size);
    power_up.velocity = powerup_velocity;
    m_power_ups.emplace_back(power_up);
}

bool PowerupSystem::randomize(const unsigned &chance) {
    unsigned int random = rand() % chance;
    return random == 0;
}

bool PowerupSystem::otherActive(const PowerUpType &type) {
    for (auto &it: m_power_ups) {
        if (it.activated) {
            if (it.m_type == type)
                return true;
        }
    }
    return false;
}

void PowerupSystem::setCallback(callback &&func) {
    m_func = func;
}

void PowerupSystem::draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const {
    for (auto &it: m_power_ups)
        target.draw(it);
}

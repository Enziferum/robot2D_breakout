//
// Created by support on 29.04.2021.
//

#include <game/Ball.hpp>

BallObject::BallObject() : GameObject(), stuck(true), wallbreaker(false),
                           velocity(), sticky(false) {

}

void BallObject::move(float dt) {
    if (stuck)
        return;
    // move the ball
    auto vel = velocity;
    m_pos += vel * dt;
    // check if outside window bounds; if so, reverse velocity and restore at correct position
    if (m_pos.x <= 0.0f) {
        velocity.x = -velocity.x;
        m_pos.x = 0.0f;
    } else if (m_pos.x + m_size.x >= border) {
        velocity.x = -velocity.x;
        m_pos.x = border - m_size.x;
    }
    if (m_pos.y <= 0.0f) {
        velocity.y = -velocity.y;
        m_pos.y = 0.0f;
    }

    m_sprite.setPosition(m_pos);
}

void BallObject::reset(const robot2D::vec2f &pos, const robot2D::vec2f &vel) {
    (void) pos;
    (void) vel;
}


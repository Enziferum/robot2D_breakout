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
#include "game/GameObject.h"
#include "robot2D/Graphics/RenderTarget.h"

GameObject::GameObject():
m_solid(false),
m_destroyed(false),
color(robot2D::Color::White) {

}

void GameObject::draw(robot2D::RenderTarget& target,
                      robot2D::RenderStates states) const {
    states.color = color;
    target.draw(m_sprite, states);
}

void GameObject::setSize(const robot2D::vec2f& size) {
    m_size = size;
    m_sprite.setScale(m_size);
}

void GameObject::setPos(const robot2D::vec2f &pos) {
    m_pos = pos;
    m_sprite.setPosition(m_pos);
}


void BallObject::move(float dt) {
    if(stuck)
        return;
    // move the ball
    auto vel = velocity;
    m_pos += vel * dt;
    // check if outside window bounds; if so, reverse velocity and restore at correct position
    if (m_pos.x <= 0.0f)
    {
        velocity.x = -velocity.x;
        m_pos.x = 0.0f;
    }
    else if (m_pos.x + m_size.x >= border)
    {
        velocity.x = -velocity.x;
        m_pos.x = border - m_size.x;
    }
    if (m_pos.y <= 0.0f)
    {
        velocity.y = -velocity.y;
        m_pos.y = 0.0f;
    }

    m_sprite.setPosition(m_pos);
}

void BallObject::reset(const robot2D::vec2f &pos, const robot2D::vec2f &vel) {

}

BallObject::BallObject():GameObject(), stuck(true), wallbreaker(false), sticky(false),
        velocity(){

}

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

#include "game/IntroState.h"
#include "game/States.h"

constexpr float second = 1000.f;

Timer::Timer():
to_time(1.f), t(0.f),
m_endless(false)
{

}

void Timer::update(float dt) {
    if(t < to_time)
        t += dt;

    else {
        if (m_callback)
            m_callback(0);

        if(m_endless)
            reset();
    }
}

float Timer::elapsed() const {
    return t;
}

void Timer::onTick(std::function<void(float)> function) {
    m_callback = std::move(function);
}

void Timer::reset(float time) {
    t = 0.f;
    if(time != 0.f)
        to_time = time;
}


IntroState::IntroState(robot2D::IStateMachine& machine) :
    State(machine),
    m_timer(){

    setup();
}

void IntroState::setup() {
    auto size = m_window.get_size();

    if(!m_texture.loadFromFile("res/textures/robot2D.png", true)) {
        std::cout << "cant load texture" <<std::endl;
        return;
    }

    m_background.setTexture(m_texture);
    m_background.setScale(robot2D::vec2f(512.f, 512.f));
    auto tx_size = m_texture.get_size();

    m_background.setPosition(robot2D::vec2f(size.x / 2.f - tx_size.x / 2,
                                          size.y / 2.f - tx_size.y / 2));

    //m_background.setColor(robot2D::Color::Green);
    m_timer.onTick([this](float dt){
        m_timer.reset();
        m_machine.pushState(States::Game);
    });
}

void IntroState::handleEvents(const robot2D::Event& event) {
    if(event.type == robot2D::Event::KeyPressed &&
    event.key.code == GLFW_KEY_SPACE){
        if(m_timer.elapsed() >= 1.f){
            m_machine.pushState(2);
        }
    }
}

void IntroState::update(float dt) {
    m_timer.update(dt);
}

void IntroState::render() {
    m_window.draw(m_background);
}



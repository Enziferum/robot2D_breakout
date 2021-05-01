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

#include <robot2D/Util/Logger.h>
#include <robot2D/Core/Keyboard.h>

#include "game/IntroState.hpp"
#include "game/States.hpp"

IntroState::IntroState(robot2D::IStateMachine& machine) :
    State(machine),
    m_timer(){

    setup();
}

void IntroState::setup() {
    auto size = m_window.get_size();
    auto tex_path = "res/textures/robot2D.png";
    auto font_path = "res/fonts/game_font.ttf";

    if(!m_texture.loadFromFile(tex_path, true)) {
        LOG_ERROR("Cant load texture %", tex_path);
        return;
    }

    if(!m_font.loadFromFile(font_path, 20)) {
        LOG_ERROR("Cant load font %", font_path);
        return;
    }

    auto tx_size = robot2D::vec2f(256.f, 256.f);
    m_background.setTexture(m_texture);
    m_background.setScale(tx_size);


    m_background.setPosition(robot2D::vec2f(size.x / 2.f - tx_size.x / 2,
                                          size.y / 2.f - tx_size.y / 2));

    auto back_size = m_background.getPosition();
    m_text.setFont(m_font);
    m_text.setText("Robot2D Engine");
    m_text.setPos(robot2D::vec2f(back_size + tx_size));


    m_timer.onTick([this](float dt){
        m_timer.reset();
        m_machine.pushState(States::Menu);
    });
}

void IntroState::handleEvents(const robot2D::Event& event) {
    if(event.type == robot2D::Event::KeyPressed && event.key.code == robot2D::SPACE){
        if(m_timer.elapsed() >= 1.f){
            m_machine.pushState(States::Game);
        }
    }
}

void IntroState::update(float dt) {
    m_timer.update(dt);
}

void IntroState::render() {
    m_window.draw(m_background);
    m_window.draw(m_text);
}



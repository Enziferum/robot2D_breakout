/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D_game - Zlib license.
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

#include "game/States.h"
#include "game/MenuState.h"
#include "game/gui/Button.h"

const std::string back_path = "res/textures/cityskyline.png";
const std::string menu_buttonPath = "res/textures/menu_button.png";

MenuState::MenuState(robot2D::IStateMachine& machine) :
State(machine) {
    setup();
}

void MenuState::handleEvents(const robot2D::Event& event) {
    //todo resize UI
    if(event.type == robot2D::Event::Resized)
        1==1;

    m_gui.handleEvents(event);
}

void MenuState::update(float dt) {
    m_gui.update(dt);
}

void MenuState::render() {
    m_window.draw(m_background);
    //m_window.draw(m_name);
    m_window.draw(m_gui);
}

void MenuState::load_resources() {
    if(!m_textures.loadFromFile("back", back_path)){
        LOG_ERROR("Can't load texture %", back_path.c_str())
    }

    if(!m_textures.loadFromFile("button", menu_buttonPath, true)){
        LOG_ERROR("Can't load texture %",  menu_buttonPath.c_str())
    }
}

void MenuState::setup() {
    load_resources();

    auto size = m_window.get_size();

    const auto button_size = robot2D::vec2f(100, 50);

    auto start_btn = gui::Button::create();
    start_btn -> setTexture(m_textures.get("button"));
    start_btn -> setPosition(robot2D::vec2f(size.x / 2 - button_size.x / 2, size.y / 2
                                                                    - button_size.y / 2));
    start_btn -> setScale(button_size);
    start_btn -> onTouch([this]() {
       m_machine.pushState(States::Game);
    });


    auto end_btn = gui::Button::create();
    end_btn -> setTexture(m_textures.get("button"));
    end_btn -> setScale(button_size);
    auto pos = start_btn -> getPosition();
    end_btn -> setPosition(robot2D::vec2f(pos.x, pos.y + 100 - button_size.y / 2));
    end_btn -> onTouch([this](){
        //todo set close callback
        m_window.close();
    });

    m_gui.pack(start_btn);
    m_gui.pack(end_btn);

    m_background.setTexture(m_textures.get("back"));
    m_background.setScale(robot2D::vec2f(size.x, size.y));
}



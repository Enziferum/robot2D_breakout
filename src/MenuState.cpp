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

#include <robot2D/Util/Logger.hpp>

#include "game/FileManager.hpp"
#include "game/States.hpp"
#include "game/MenuState.hpp"
#include "game/gui/TextButton.hpp"


const std::string back_path = "res/textures/cityskyline.png";
const std::string menu_buttonPath = "res/textures/menu_button.png";
const std::string font_path = "res/fonts/game_font.ttf";

const auto button_size = robot2D::vec2f(200, 50);


MenuState::MenuState(robot2D::IStateMachine& machine, AppContext<ContextID>& context) :
State(machine),
m_context(context) {
    setup();
}

void MenuState::handleEvents(const robot2D::Event &event) {
    if (event.type == robot2D::Event::Resized) {}
    m_gui.handleEvents(event);
}

void MenuState::update(float dt) {
    m_gui.update(dt);
}

void MenuState::render() {
    m_window.draw(m_background);
    m_window.draw(m_gui);
}

void MenuState::load_resources() {
    FileManager fm;
    auto configuration = *(Configuration*)m_context.getBuffer(ContextID::Configuration);
    fm.setConfiguration(configuration.getResourceConfiguration());

    if(!m_textures.loadFromFile(ResourceIDs::Background,
                                fm.combinePath(ResourceType::Texture,
                                               "cityskyline.png"))){
        LOG_ERROR("Can't load texture % \n", back_path.c_str())
    }

    if(!m_textures.loadFromFile(ResourceIDs::Button, fm.combinePath(ResourceType::Texture,
                                                                    "menu_button.png"), true)){
        LOG_ERROR("Can't load texture % \n",  menu_buttonPath.c_str())
    }

    if(!m_fonts.loadFromFile(ResourceIDs::Font, fm.combinePath(ResourceType::Font,
                                                               "game_font.ttf"))) {
        LOG_ERROR("Can't load font % \n", font_path.c_str())
    }
}

void MenuState::setup() {
    load_resources();

    auto size = m_window.get_size();

    auto start_label = gui::Label::create();
    start_label -> setFont(m_fonts.get(ResourceIDs::Font));
    start_label -> setText("Play");

    auto start_btn = gui::TextButton::create();
    start_btn -> setTexture(m_textures.get(ResourceIDs::Button));
    start_btn -> setPosition(robot2D::vec2f(size.x / 2 - button_size.x / 2, size.y / 2 - 100
                                                                        - button_size.y / 2));

    auto sz = m_textures.get(ResourceIDs::Button).get_size();
    start_btn -> scale(robot2D::vec2f (button_size.x / sz.x, button_size.y / sz.y));

    auto start_bounds = start_btn -> getGlobalBounds();
    auto center = robot2D::vec2f(start_bounds.lx + start_bounds.width / 2 - 12,
                                 start_bounds.ly + start_bounds.height / 2 - 8);
    start_label -> setPosition(center);

    start_btn -> setLabel(start_label);

    start_btn -> onTouch([this]() {
        m_machine.popState();
        m_machine.pushState(States::Game);
    });

    auto end_label = gui::Label::create();
    end_label -> setFont(m_fonts.get(ResourceIDs::Font));
    end_label -> setText("Quit");

    auto end_btn = gui::TextButton::create();
    end_btn -> setTexture(m_textures.get(ResourceIDs::Button));
    end_btn -> setScale(robot2D::vec2f (button_size.x / sz.x, button_size.y / sz.y));

    auto end_pos = start_btn -> getPosition();
    end_btn -> setPosition(robot2D::vec2f(end_pos.x, end_pos.y + 100 - button_size.y / 2));

    auto end_bounds = end_btn -> getGlobalBounds();
    center = robot2D::vec2f(end_bounds.lx + end_bounds.width / 2 - 12,
                            end_bounds.ly + end_bounds.height / 2 - 8);

    end_label -> setPosition(center);
    end_btn -> setLabel(end_label);

    end_btn -> onTouch([this](){
       m_window.close();
    });

    m_gui.pack(start_btn);
    m_gui.pack(end_btn);


    m_background.setTexture(m_textures.get(ResourceIDs::Background));
    m_background.setScale(robot2D::vec2f(size.x, size.y));
}




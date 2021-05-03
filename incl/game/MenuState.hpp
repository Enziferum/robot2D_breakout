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

#pragma once

#include "robot2D/Extra/State.h"
#include "robot2D/Extra/IStateMachine.h"

#include "robot2D/Graphics/Text.h"
#include "robot2D/Graphics/Sprite.h"
#include "robot2D/Util/ResourceHandler.h"

#include "game/gui/Gui.hpp"

class MenuState: public robot2D::State{
public:
    MenuState(robot2D::IStateMachine& machine);
    ~MenuState()override = default;

    void handleEvents(const robot2D::Event& event);
    void update(float dt) override;
    void render() override;
private:
    void setup();
    void load_resources();
private:
    robot2D::ResourceHandler<robot2D::Texture> m_textures;
    robot2D::Sprite m_background;
    robot2D::Font m_font;
    robot2D::Text m_name;

    gui::Gui m_gui;
};
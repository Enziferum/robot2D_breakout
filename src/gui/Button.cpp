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

#include <robot2D/Graphics/RenderTarget.h>
#include <robot2D/Graphics/RenderStates.h>
#include "game/gui/Button.hpp"

namespace gui{
    Button::Button(): INode(),
    m_function(nullptr),
    m_texture(nullptr)
    {

    }

    std::shared_ptr<Button> Button::create() {
        return std::make_shared<Button>();
    }

    void Button::onPress(const robot2D::vec2f& mpos) {

    }

    void Button::onHover(const robot2D::vec2f& mpos) {

    }

    void Button::update(float dt) {
        if(m_pressed && m_function){
            m_function();
            m_pressed = false;
        }
    }

    void Button::setTexture(const robot2D::Texture& texture) {
        m_texture = &texture;
    }

    void Button::onTouch(std::function<void()>&& function) {
        m_function = function;
    }

    void Button::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
        if(!m_texture)
            return;
        states.transform *= getTransform();
        states.texture = m_texture;
        states.color = robot2D::Color::White;
        target.draw(states);
    }

}
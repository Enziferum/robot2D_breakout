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

#include <robot2D/Graphics/RenderTarget.hpp>
#include <game/gui/Label.hpp>

namespace gui {
    Label::Label() {}

    void Label::onPress(const robot2D::vec2f &f) {}

    void Label::onHover(const robot2D::vec2f &f) {}

    void Label::update(float dt) {}

    void Label::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const
    {

        target.draw(m_text);
    }

    Label::Ptr Label::create() {
        return std::make_shared<Label>();
    }

    void Label::setPosition(const robot2D::vec2f& pos) {
        Transformable::setPosition(pos);
        m_text.setPos(pos);
    }

    void Label::setFont(const robot2D::Font& font) {
        m_text.setFont(font);
    }

    void Label::setText(const std::string& text) {
        m_text.setText(text);
    }

    void Label::setScale(const robot2D::vec2f& factor) {
        Transformable::setScale(factor);
    }

}
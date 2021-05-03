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
#include "game/gui/Gui.hpp"

namespace gui{

    Gui::Gui():
        m_nodes() {

    }

    void Gui::handleEvents(const robot2D::Event& event) {
        if(event.type == robot2D::Event::MousePressed) {
            if(event.mouse.btn == robot2D::MouseLeft) {
                for(auto& it: m_nodes)
                    it->onPress(robot2D::vec2f(event.mouse.x, event.mouse.y));
            }
        }
        if(event.type == robot2D::Event::MouseMoved) {
            for(auto& it: m_nodes)
                it->onHover(robot2D::vec2f(event.mouse.x, event.mouse.y));
        }
    }


    void Gui::update(float dt) {
        for(auto& it: m_nodes)
            it->update(dt);
    }

    void Gui::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
        for(auto& it: m_nodes)
            target.draw(*it);
    }

    void Gui::pack(INode::Ptr node) {
        m_nodes.emplace_back(node);
    }


}
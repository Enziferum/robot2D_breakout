//
// Created by support on 02.04.2021.
//

#include <robot2D/Graphics/RenderTarget.h>
#include "game/gui/Gui.h"

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
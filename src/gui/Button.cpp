//
// Created by support on 02.04.2021.
//

#include <robot2D/Graphics/RenderTarget.h>
#include <robot2D/Graphics/RenderStates.h>
#include "game/gui/Button.h"

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
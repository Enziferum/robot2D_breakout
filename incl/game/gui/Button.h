//
// Created by support on 02.04.2021.
//

#pragma once

#include <functional>
#include <robot2D/Graphics/Sprite.h>

#include "INode.h"


namespace gui {

    class Button: public INode{
    public:
        Button();
        virtual ~Button() override = default;

        static std::shared_ptr<Button> create();
        void onTouch(std::function<void()>&& function);

        void onPress(const robot2D::vec2f&) override;
        void onHover(const robot2D::vec2f&) override;
        void update(float dt) override;

        void setTexture(const robot2D::Texture& texture);
    protected:
        void draw(robot2D::RenderTarget &target, robot2D::RenderStates states)
                const override;

    private:
        enum class State{
            Normal,
            Hovered,
            Pressed
        };

        std::function<void()> m_function;
        const robot2D::Texture* m_texture;
    };
}

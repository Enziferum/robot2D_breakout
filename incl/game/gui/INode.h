//
// Created by support on 02.04.2021.
//

#pragma once

#include <memory>
#include <robot2D/Core/Vector2.h>
#include <robot2D/Graphics/Drawable.h>
#include <robot2D/Graphics/Transformable.h>

namespace gui{

    class INode: public robot2D::Drawable, public robot2D::Transformable {
    public:
        using Ptr = std::shared_ptr<INode>;
    public:
        INode();
        virtual ~INode() = 0;


        virtual void onPress(const robot2D::vec2f&);
        virtual void onHover(const robot2D::vec2f&);
        virtual void update(float dt);
    protected:
        void draw(robot2D::RenderTarget &target, robot2D::RenderStates states)
        const override = 0;

    protected:
        bool m_pressed;
        bool m_hover;
    };

}

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

#include <memory>
#include <robot2D/Core/Vector2.h>
#include <robot2D/Graphics/Drawable.h>
#include <robot2D/Graphics/Transformable.h>

namespace gui {

    class INode : public robot2D::Drawable, public robot2D::Transformable {
    public:
        using Ptr = std::shared_ptr<INode>;
    public:
        INode();

        virtual ~INode() = 0;


        virtual void onPress(const robot2D::vec2f &);

        virtual void onHover(const robot2D::vec2f &);

        virtual void update(float dt);

    protected:
        void draw(robot2D::RenderTarget &target, robot2D::RenderStates states)
        const override = 0;

    protected:
        bool m_pressed;
        bool m_hover;
    };

}

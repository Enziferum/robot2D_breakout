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

#include <robot2D/Graphics/Text.h>
#include "INode.hpp"

namespace gui {
    class Label : public INode {
    public:
        using Ptr = std::shared_ptr<Label>;
    public:
        Label();

        ~Label() override = default;

        void onPress(const robot2D::vec2f &f) override;

        void onHover(const robot2D::vec2f &f) override;

        void update(float dt) override;

        void setText(const std::string& text);

        void setFont(const robot2D::Font& font);

        void setPosition(const robot2D::vec2f &pos) override;

        void setScale(const robot2D::vec2f &factor) override;

        static Ptr create();
    protected:
        void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override;
        robot2D::Text m_text;
    };
}

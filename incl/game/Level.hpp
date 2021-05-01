/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D - Zlib license.
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

#include <vector>
#include <string>

#include "robot2D/Graphics/Drawable.h"
#include "robot2D/Core/Vector2.h"
#include "robot2D/Util/ResourceHandler.h"
#include <robot2D/Graphics/Shader.h>

#include "GameObject.hpp"
#include "IDs.hpp"

class Level: public robot2D::Drawable {
public:
    Level();
    ~Level() = default;


    bool loadLevel(const std::string& path, const robot2D::ResourceHandler<robot2D::Texture, ResourceIDs>& handler,
                   const robot2D::vec2f& size, const robot2D::vec2f& offset = robot2D::vec2f());

    void update(float dt);
    void onResize(const robot2D::vec2f& size);

    bool destroyed() const;
    std::vector<GameObject>& getTiles();
protected:

    void draw(robot2D::RenderTarget& target,
              robot2D::RenderStates states) const override;
private:
    std::vector<GameObject> m_tiles;
    robot2D::vec2f m_size;

    //size of level
    size_t rw, rh;

    robot2D::ShaderHandler m_shader;

    size_t last_size = 0;
};

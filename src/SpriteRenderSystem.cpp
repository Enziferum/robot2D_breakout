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

#include <iostream>

#include "robot2D/Graphics/RenderTarget.h"

#include "game/Components.h"
#include "game/SpriteRenderSystem.h"
#include "game/Scene.h"

namespace ecs{

    SpriteRenderSystem::SpriteRenderSystem() {
        check_component<TransformComponent>();
        check_component<SpriteComponent>();
    }

    void SpriteRenderSystem::debug() {
        std::cout << "SpriteRenderSystem is part of scene now" <<std::endl;
    }

    void SpriteRenderSystem::process(float dt) {
    }

    void SpriteRenderSystem::on_addEntity(Entity entity) {
    }

    void SpriteRenderSystem::draw(robot2D::RenderTarget& target,
                                  robot2D::RenderStates states) const {
        auto& entities = getEntites();
        for(auto& entity: entities){
            auto& transform = entity.getComponent<TransformComponent>();
            auto& sprite = entity.getComponent<SpriteComponent>();

            states.transform *= transform.getTransform();
            states.texture = &sprite.getTexture();

            target.draw(states);
        }

    }


}
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

#include "game/EcsState.h"
#include "game/Components.h"
#include "game/SpriteRenderSystem.h"


EcsState::EcsState(robot2D::IStateMachine &machine) : State(machine) {
    setup();
}

void EcsState::handleEvents(const robot2D::Event& event) {
    m_scene.forwardEvent(event);
}

void EcsState::update(float dt) {
    m_scene.update(dt);
}

void EcsState::render() {
   // m_window.draw(m_scene);
}

void EcsState::setup() {
    m_textures.loadFromFile("face","res/textures/awesomeface.png",
                            true);

    setup_ecs();
}

void EcsState::setup_ecs() {
    //base system to render sprites

    // concept entites to be render by spritesystem should have
    // component SpriteRender and TransformComponent
    m_scene.addSystem<ecs::SpriteRenderSystem>().debug();

    auto entity = m_scene.addEntity();
    entity.addComponent<ecs::TransformComponent>();
    entity.addComponent<ecs::SpriteComponent>();

    if(!entity.hasComponent<ecs::SpriteComponent>()){
        std::cout << "face Entity don't have sprite component" <<std::endl;
    }
//
//    entity.getComponent<ecs::TransformComponent>()
//            .setPosition(robot2D::vec2f(100.f, 100.f));
//    entity.getComponent<ecs::TransformComponent>()
//            .setScale(robot2D::vec2f(50.f, 50.f));
//    entity.getComponent<ecs::SpriteComponent>()
//            .setTexture(m_textures.get("face"));

    // just now we should see sprite object with scale and position
}



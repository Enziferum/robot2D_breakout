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

#include <vector>
#include <stdexcept>

#include "robot2D/Graphics/Drawable.h"
#include "robot2D/Core/Event.h"

#include "Entity.h"
#include "System.h"

namespace ecs {
    class Scene: public robot2D::Drawable {
    public:
        Scene();
        ~Scene() = default;


        void forwardEvent(const robot2D::Event& event);
        void update(float dt);

        //ecs part //
        Entity addEntity();

        void destroyEntity(Entity entity);

        template<typename T, typename ...Args>
        T& addSystem(Args&& ...args);
        //ecs part //
    protected:
        virtual void draw(robot2D::RenderTarget &target, robot2D::RenderStates state) const override;

    private:
        EntityManager m_entityManager;
        std::vector<Entity> m_entityBuffer;
        std::vector<System::Ptr> m_systems;
        std::vector<robot2D::Drawable*> m_drawables;
    };

    template<typename T, typename... Args>
    T& Scene::addSystem(Args&&... args) {
        //todo assert

        System::Ptr system = std::make_shared<T>(std::forward<Args>(args)...);
        //tobe able use scene methods
        system->m_scene = this;

        if(std::is_base_of<robot2D::Drawable, T>::value){
            m_drawables.push_back(dynamic_cast<robot2D::Drawable*>(system.get()));
        }

        m_systems.push_back(system);

        return *static_cast<T*>(system.get());
    }
}

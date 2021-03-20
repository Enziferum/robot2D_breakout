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
#include <memory>
#include "robot2D/Drawable.h"

#include "System.h"
#include "Entity.h"

namespace robot2D{
    class Scene: public Drawable{
    public:
        Scene();
        ~Scene() = default;


        template<typename T, typename ...Args>
        void addSystem(Args&&...args);
    protected:
        void draw(RenderTarget &target, RenderStates states) const override;

    private:
        //renderable content
        SystemManager m_systemManager;
        EntityManager m_entityManager;

        std::vector<Drawable*> m_drawables;
    };


    template<typename T, typename... Args>
    void Scene::addSystem(Args&&... args) {
        auto system = std::make_shared<T>(std::forward<Args>(args)...);
        if(std::is_base_of<Drawable, T>::value){
            m_drawables.push_back(dynamic_cast<Drawable*>(system));
        }
    }
}

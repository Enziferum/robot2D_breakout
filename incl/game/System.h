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
#include <vector>
#include "Entity.h"

namespace ecs{
    class Scene;
    class System{
    public:
        System();
        virtual ~System() = 0;

        virtual void process(float dt);
        void setActive(const bool& status);


        //get mask of system
    protected:
        virtual void on_addEntity(Entity entity);
        //check required components
        template<typename T>
        void check_component();

        std::vector<Entity>& getEntites();

        //for const marked functions
        const std::vector<Entity>& getEntites() const;

    private:
        void addEntity(Entity entity);
    protected:
        Scene* m_scene;
    private:
        friend class Scene;
        using Ptr = std::shared_ptr<System>;

        std::vector<Entity> m_entities;
        bool m_active;
    };


    struct TypeInfoComparer{
        bool operator()(const std::type_info* a, const std::type_info* b) const{
            return a->before(*b);
        }
    };



    template<typename T>
    void System::check_component() {
        //get type_info about class
        auto& id = typeid(T);


    }


    // how add entites to systems ??//
    // Systems works with required components
    // solution - entity.get_mask() == system.get_mask();

}


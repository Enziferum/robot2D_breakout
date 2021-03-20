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
#include <unordered_map>

namespace ecs{
    using u8 = unsigned int;



    //memory pool of components
    class ComponentPool{
        //bitmask of component
        //Component type
    };

    class EntityManager;
    class Entity{
    public:
        Entity();
        ~Entity() = default;

        template<typename T>
        void addComponent(const T&);

        template<typename T, typename ...Args>
        T& addComponent(Args&& ...args);

        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        template<typename T>
        const T& getComponent() const;

    private:
        friend class EntityManager;
        friend class Scene;
        EntityManager* m_entityManager;
        u8 m_id;
    };



    class EntityManager{
    public:
        EntityManager();
        EntityManager(const EntityManager&) = delete;
        EntityManager(const EntityManager&&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&&) = delete;
        ~EntityManager() = default;

        Entity addEntity();
        void destroyEntity(Entity entity);

        template<typename T>
        void addComponent(Entity entity, T);

        template<typename T, typename ...Args>
        T& addComponent(Entity entity, Args&& ...args);

        template<typename T>
        bool hasComponent(Entity entity);

        template<typename T>
        T& getComponent(Entity entity);

        template<typename T>
        const T& getComponent(Entity entity) const;
    private:
        std::unordered_map<u8, Entity> m_entites;
    };


    template<typename T>
    void Entity::addComponent(const T& component) {
        m_entityManager -> addComponent<T>(*this, component);
    }

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&& ... args) {
        return m_entityManager -> addComponent<T>(*this,
                                               std::forward<Args>(args)...);
    }

    template<typename T>
    bool Entity::hasComponent() {
        return m_entityManager -> hasComponent<T>(*this);
    }

    template<typename T>
    T& Entity::getComponent() {
        return m_entityManager -> getComponent<T>(*this);
    }

    template<typename T>
    const T& Entity::getComponent() const {
        return m_entityManager -> getComponent<T>(*this);
    }



    template<typename T>
    void EntityManager::addComponent(Entity entity, T component) {
        //store to componentManager

        //entity id to find
        entity.m_id;

      //  std::unordered_map<u8, T>;
    }

    template<typename T, typename... Args>
    T& EntityManager::addComponent(Entity entity, Args &&... args) {
        //here we create in memory component, because we have args
        T component(std::forward<Args>(args)...);
        addComponent<T>(entity, std::move(component));

        return getComponent<T>(entity);
    }

    template<typename T>
    bool EntityManager::hasComponent(Entity entity) {
        //find by ID
        // and return boolean param
        return false;
    }

    template<typename T>
    T& EntityManager::getComponent(Entity entity) {
        //find in store by entity
        //and return it
    }

    template<typename T>
    const T& EntityManager::getComponent(Entity entity) const {
        return getComponent<T>(entity);
    }

#include "Entity.inl"

}

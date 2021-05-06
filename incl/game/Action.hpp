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
#include <list>
#include <utility>
#include <functional>
#include <robot2D/Core/Event.hpp>

namespace robot2D {
    class Action {
    public:
        enum Type {
            //1
            RealTime = 1,
            //2
            Pressed = 1 << 1,
            //4
            Released = 1 << 2
        };


        bool test() const;

        bool operator==(const Event &event) const;

        bool operator==(const Action &other) const;

    private:
        template<typename> friend
        class ActionTarget;

        robot2D::Event event;
        int _type;
    };


    template<typename T = int>
    class ActionMap {
    public:
        ActionMap(const ActionMap<T> &) = delete;

        ActionMap<T> &operator=(const ActionMap<T> &) = default;

        ActionMap() = default;

        void map(const T &key, const Action &action);

        const Action &get(const T &key) const;

    private:
        std::unordered_map<T, Action> m_map;
    };

    template<typename T>
    void ActionMap<T>::map(const T &key, const Action &action) {
        m_map.emplace(key, action);
    }

    template<typename T>
    const Action &ActionMap<T>::get(const T &key) const {
        return m_map.at(key);
    }


    template<typename T = int>
    class ActionTarget {
    public:
        ActionTarget(const ActionTarget<T> &) = delete;

        ActionTarget<T> &operator=(const ActionTarget<T> &) = delete;

        using FuncType = std::function<void(const robot2D::Event &)>;

        ActionTarget(const ActionMap<T> &map);

        bool processEvent(const robot2D::Event &event) const;

        void processEvents() const;

        void bind(const T &key, const FuncType &callback);

        void unbind(const T &key);

    private:
        std::list<std::pair<T, FuncType>> m_eventsRealTime;
        std::list<std::pair<T, FuncType>> m_eventsPoll;
        const ActionMap<T> m_actionMap;
    };

    template<typename T>
    ActionTarget<T>::ActionTarget(const ActionMap<T> &map): m_actionMap(map) {}

    template<typename T>
    bool ActionTarget<T>::processEvent(const robot2D::Event &event) const {
        bool res = false;
        for (auto &pair: m_eventsPoll) {
            if (m_actionMap.get(pair.first) == event) {
                pair.second(event);
                res = true;
                break;
            }
        }
        return res;
    }

    template<typename T>
    void ActionTarget<T>::processEvents() const {
        for (auto &pair: m_eventsRealTime) {
            const Action &action = m_actionMap.get(pair.first);
            if (action.test())
                pair.second(action.event);
        }
    }

    template<typename T>
    void ActionTarget<T>::bind(const T &key, const FuncType &callback) {
        const Action &action = m_actionMap.get(key);
        if (action._type & Action::Type::RealTime)
            m_eventsRealTime.template emplace_back(key, callback);
        else
            m_eventsPoll.template emplace_back(key, callback);
    }

    template<typename T>
    void ActionTarget<T>::unbind(const T &key) {
        auto remove_func = [&key](const std::pair<T, FuncType> &pair) {
            return pair.first = key;
        };

        const Action &action = m_actionMap.get(key);
        if (action._type & Action::Type::RealTime)
            m_eventsRealTime.remove_if(remove_func);
        else
            m_eventsPoll.remove_if(remove_func);
    }

}

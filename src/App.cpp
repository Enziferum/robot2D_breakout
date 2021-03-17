/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
hakka - Zlib license.
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

#include "game/App.h"
#include "game/Audio.h"

App::App() {
    setup();
}

void App::run() {
    hakka::Event event;
    float dt = 0;

    while (m_window.isOpen()){
        handleEvents(event);
        update(dt);
        render();
    }

}

void App::handleEvents(hakka::Event& event) {
    while (m_window.pollEvents(event)){
//        if(event.type == hakka::Event::KeyPressed){
//            if (event.key.code == GLFW_KEY_ESCAPE)
//                m_window.close();
//        }

//        for(auto& it: m_states)
//            it.second->handleEvents(event);

        m_states[m_current_state] -> handleEvents(event);
    }
}

void App::update(float dt) {
    float currentTime = float(glfwGetTime());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

//    for(auto& it:m_states)
//        it.second->update(deltaTime);

    m_states[m_current_state] ->update(deltaTime);
}

void App::render() {
    m_window.clear();
//    for(auto& it:m_states)
//        it.second->render();

    m_states[m_current_state] -> render();

    m_window.display();
}

void App::setup() {
    Audio::getInstanse() -> loadFile("res/audio/breakout.wav",
                                     "breakout", AudioType::music);
    Audio::getInstanse() -> play("breakout", true);
    Audio::getInstanse() -> setVolume("breakout", 80.f);
}

void App::addState(const int id, State::Ptr ptr) {
    if(ptr == nullptr)
        return;
    auto found = m_states.find(id);
    if(found != m_states.end())
        return;

    m_states.insert(std::pair<int,
                    State::Ptr>(id, ptr));
}

void App::setCurrentState(const int& id) {
    m_current_state = id;
}


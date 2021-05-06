//
// Created by support on 02.04.2021.
//

#include "game/Timer.hpp"


constexpr float second = 1000.f;

Timer::Timer() :
        m_end(1.f), m_start(0.f),
        m_endless(false) {

}


Timer::Timer(const float &endTime, bool endless) :
        m_end(endTime), m_start(0.f),
        m_endless(endless) {

}

void Timer::update(float dt) {
    if (m_start < m_end)
        m_start += dt;

    else {
        if (m_callback)
            m_callback(0);

        if (m_endless)
            reset();
    }
}

float Timer::elapsed() const {
    return m_start;
}

void Timer::onTick(std::function<void(float)> function) {
    m_callback = std::move(function);
}

void Timer::reset(float time) {
    m_start = 0.f;
    if (time <= 0.f)
        return;
    m_end = time;
}


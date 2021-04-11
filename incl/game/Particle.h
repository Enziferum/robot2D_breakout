//
// Created by support on 02.04.2021.
//

#pragma once

#include <robot2D/Core/Vector2.h>
#include <robot2D/Graphics/Color.h>

struct Particle {
    Particle();
    ~Particle() = default;

    bool is_life() const;

    robot2D::vec2f m_pos, m_velocity;
    //todo make color
    float r, g, b, alpha;
    float lifeTime;
};
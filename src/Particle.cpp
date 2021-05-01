//
// Created by support on 02.04.2021.
//

#include "game/Particle.hpp"

Particle::Particle(): m_pos(), m_velocity(),
                      r(1.f), g(1.f), b(1.f), alpha(1.f),
                      lifeTime(0.f)
{

}

bool Particle::is_life() const {
    return lifeTime > 0.f;
}

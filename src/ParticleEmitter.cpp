/*********************************************************************
(c) Alex Raag 2021
https://github.com/Enziferum
robot2D - Zlib license.
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


#include "robot2D/Graphics/RenderTarget.h"
#include "game/ParticleEmitter.h"

constexpr int new_particle_sz = 2;

ParticleEmitter::ParticleEmitter(): m_particles(),
m_texture(nullptr){
    setup();
}

void ParticleEmitter::setup() {
    m_particleShader.createShader(0x8B31,
                                  "res/shaders/particle.vs");

    m_particleShader.createShader(0x8B30,
                                  "res/shaders/particle.fs");

    // init particles //
    int particles_sz = 500;
    for(int it = 0; it < particles_sz; ++it)
        m_particles.emplace_back(Particle());
}


void ParticleEmitter::update(float dt, int new_sz, const BallObject& bind,
                             const robot2D::vec2f& offset) {
    //add new particles
    for(int it = 0; it < new_sz; ++it){
        int unusedParticle = find_first_unused();
        respawn_particle(m_particles[unusedParticle], bind, offset);
    }

    //update current
    for(auto& it: m_particles){
        it.lifeTime -= dt;
        if(it.is_life()){
            auto vec_copy = it.m_velocity;
            it.m_pos -= vec_copy * dt;
            it.alpha -= dt * 2.5f;
        }
    }
}


void ParticleEmitter::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_particleShader.use();

    for(auto& it: m_particles){
        if(!it.is_life())
            continue;

        m_particleShader.set_parameter("offset", it.m_pos);
        m_particleShader.set_parameter("color", it.r, it.g, it.b, it.alpha);

        states.texture = m_texture;
        states.shader = const_cast<robot2D::ShaderHandler*>(&m_particleShader);
        //states.customVao = &VAO;

        target.draw(states);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int lastUsedParticle = 0;
int ParticleEmitter::find_first_unused() {
    for (unsigned int i = lastUsedParticle; i < 500; ++i){
        if (m_particles[i].lifeTime <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (m_particles[i].lifeTime <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken,
    // override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleEmitter::respawn_particle(Particle& particle, const BallObject& obj,
                                       const robot2D::vec2f& offset) {
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.m_pos.x = obj.m_pos.x + random + offset.x;
    particle.m_pos.y = obj.m_pos.y + random + offset.y;
    particle.r = rColor;
    particle.g = rColor;
    particle.b = rColor;
    particle.alpha = 1.f;
    particle.lifeTime = 1.0f;
    auto vel_copy = obj.velocity;
    particle.m_velocity = vel_copy * 0.1f;
}

void ParticleEmitter::setup_GL() {

}




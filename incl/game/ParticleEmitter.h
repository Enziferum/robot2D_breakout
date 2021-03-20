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

#include <vector>

#include <robot2D/Core/Vector2.h>
#include <robot2D/Graphics/Shader.h>
#include <robot2D/Graphics/Drawable.h>

#include "GameObject.h"

struct Particle{
    Particle(): m_pos(), m_velocity(),
    r(1.f), g(1.f), b(1.f), alpha(1.f),
    lifeTime(0.f)
    {}

    robot2D::vec2f m_pos, m_velocity;
    float r, g, b, alpha;
    float lifeTime;

    bool is_life() const {
        return lifeTime > 0.f;
    }
};


class ParticleEmitter: public robot2D::Drawable{
public:
    ParticleEmitter();
    ~ParticleEmitter() = default;

    void setTexture(robot2D::Texture& tex){
        m_texture = &tex;
    }
    void update(float dt, int new_sz, const BallObject& bind,
                const robot2D::vec2f& offset);
protected:
    void draw(robot2D::RenderTarget &target,
              robot2D::RenderStates states) const override;
    void setup();
    void setup_GL();

    int find_first_unused();
    void respawn_particle(Particle& particle, const BallObject& bind, const robot2D::vec2f& offset);
private:
    std::vector<Particle> m_particles;
    robot2D::ShaderHandler m_particleShader;

    robot2D::Texture* m_texture;
};
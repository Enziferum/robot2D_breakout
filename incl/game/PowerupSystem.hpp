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
#include <functional>

#include <robot2D/Graphics/Drawable.h>
#include <robot2D/Util/ResourceHandler.h>

#include "PowerUp.hpp"
#include "IDs.hpp"

class PowerupSystem : public robot2D::Drawable {
public:
    using callback = std::function<void(const PowerUpType &)>;
public:
    PowerupSystem();

    ~PowerupSystem() = default;

    void update(float dt);

    void spawn_powerup(robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &m_texture,
                       const robot2D::vec2f &spawn_pos);

    void setCallback(callback &&func);

    std::vector<PowerUp> &get();

protected:
    void draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const override;

private:
    bool otherActive(const PowerUpType &type);

    bool randomize(const unsigned int &chance);

private:
    std::vector<PowerUp> m_power_ups;
    callback m_func;
};

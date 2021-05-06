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

#include <robot2D/Util/ResourceHandler.h>
#include <robot2D/Graphics/Texture.h>
#include <robot2D/Graphics/Drawable.h>
#include <robot2D/Core/Event.h>


#include "MessageBus.hpp"
#include "PowerupSystem.hpp"
#include "Level.hpp"
#include "AudioPlayer.hpp"
#include "Configuration.hpp"
#include "ParallaxEffect.hpp"
#include "ParticleEmitter.hpp"
#include "PostProcessing.hpp"

#include "IDs.hpp"

class World : public robot2D::Drawable {
public:
    World(MessageBus &messageBus,
          robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures);

    World(const World &) = delete;

    World(const World &&) = delete;

    World &operator=(const World &) = delete;

    World &operator=(const World &&) = delete;

    ~World() = default;

    bool setup(
            GameConfiguration *gameConfiguration,
            AudioPlayer *audioPlayer);

    bool setupLevels(const std::vector<std::string> &paths,
                     robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures);

    void handleEvents(const robot2D::Event &event);

    void forwardMessage(const Message &message);

    void update(float dt);

protected:
    void draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const override;

private:
    void process_input(float dt);

    void process_collisions();

    void activate_power(PowerUp &power);

    void reset_game(bool update_lifes = true);

    void change_level();

private:
    MessageBus &m_messageBus;

    AudioPlayer *m_audioPlayer;
    PowerupSystem m_powerupSystem;

    GameConfiguration *m_gameConfiguration;
    robot2D::vec2u m_windowSize;
    unsigned int currlevel = 0;
    unsigned int m_lives;

    robot2D::Sprite m_background;
    GameObject m_paddle;
    BallObject m_ball;
    std::vector<Level> m_levels;

    PostProcessing m_postProcessing;
    ParticleEmitter m_particleEmitter;
    ParallaxEffect m_parallax;

    bool m_keys[1024];
    bool m_keysProcessed[1024];

    robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &m_textures;
};

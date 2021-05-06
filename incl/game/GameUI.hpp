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

#include <robot2D/Graphics/Drawable.hpp>
#include <robot2D/Graphics/Text.hpp>
#include <robot2D/Util/ResourceHandler.hpp>

#include "IDs.hpp"
#include "Message.hpp"
#include "Configuration.hpp"

class GameUI : public robot2D::Drawable {
public:
    GameUI();

    ~GameUI() = default;

    void setup(robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures, Configuration &configuration);

    void handleMessage(const Message &msg);

    void update(float dt);

protected:
    void draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const override;

private:
    unsigned int m_lives = 0;
    unsigned int m_score = 0;

    robot2D::Text m_livesText;
    robot2D::Text m_winText;
    robot2D::Text m_scoreText;
    robot2D::Text m_timeText;

    robot2D::ResourceHandler<robot2D::Font, ResourceIDs> m_fonts;
    std::vector<robot2D::Sprite> m_livesSprites;
};
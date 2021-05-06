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
#include <robot2D/Util/Logger.h>
#include <robot2D/Graphics/RenderTarget.h>
#include <robot2D/Graphics/RenderStates.h>

#include <game/GameUI.hpp>
#include <game/MessageIDs.hpp>
#include <game/FileManager.hpp>

const robot2D::vec2f lives_pos = robot2D::vec2f(650, 10);
const auto live_sz = robot2D::vec2f(robot2D::vec2f(30.f, 30.f));

GameUI::GameUI() {

}

void GameUI::setup(robot2D::ResourceHandler<robot2D::Texture, ResourceIDs> &textures, Configuration &configuration) {
    FileManager fm;
    fm.setConfiguration(configuration.getResourceConfiguration());
    m_fonts.loadFromFile(ResourceIDs::Font,
                         fm.combinePath(ResourceType::Font, "game_font.ttf"));

    m_livesText.setText("Lives:");
    m_livesText.setPos(lives_pos);
    m_livesText.setFont(m_fonts.get(ResourceIDs::Font));

    auto live_start_pos = robot2D::vec2f(m_livesText.getPos().x + 50, 5);

    m_lives = configuration.getGameConfiguration().max_lives;
    for (auto it = 0; it < m_lives; ++it) {
        robot2D::Sprite sprite;
        sprite.setTexture(textures.get(ResourceIDs::Face));
        sprite.setScale(live_sz);
        sprite.setPosition(robot2D::vec2f(live_start_pos.x + live_sz.x * it,
                                          live_start_pos.y));
        m_livesSprites.push_back(sprite);
    }


    m_scoreText.setText("Score: 0");
    m_scoreText.setPos(robot2D::vec2f(10.f, 10.f));
    m_scoreText.setFont(m_fonts.get(ResourceIDs::Font));
}

void GameUI::handleMessage(const Message &msg) {
    if (msg.id == messageIDs::LivesUpdate) {
        auto data = msg.unpack<LivesEvent>();
        m_lives = data.new_lives;
    }

    if (msg.id == messageIDs::ScoreUpdate) {
        auto data = msg.unpack<ScoreEvent>();
        m_score += data.new_score;
        m_scoreText.setText("Score: " + std::to_string(m_score));
    }
}

void GameUI::update(float dt) {
    (void) dt;
}

void GameUI::draw(robot2D::RenderTarget &target, robot2D::RenderStates states) const {
    target.draw(m_livesText);
    target.draw(m_scoreText);
    for (int it = 0; it < m_lives; ++it)
        target.draw(m_livesSprites[it]);
}



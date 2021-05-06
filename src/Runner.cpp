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

#include <robot2D/Util/Logger.hpp>

#include "game/IntroState.hpp"
#include "game/MenuState.hpp"
#include "game/GameState.hpp"
#include "game/PauseState.hpp"
#include "game/States.hpp"
#include "game/Runner.hpp"
#include "game/FileManager.hpp"

const std::string resourceiniPath = "res/config.ini";
const std::string gameiniPath = "res/game.ini";

Runner::Runner() : m_bus(),
                   my_app{robot2D::vec2u(800, 600),
                          "robot2D Game", true} {
    init();
}

FileManager fm_r;

void Runner::init() {
    logger::debug = true;


    if (!m_configuration.loadResources(resourceiniPath)) {
        return;
    }

    if (!m_configuration.loadGameSettings(gameiniPath)) {
        return;
    }

    if (!m_context.storeInBuffer(ContextID::Configuration, (void *) (&m_configuration))) {
        return;
    }
    if (!m_context.storeInBuffer(ContextID::Audio, (void *) (&m_audioPlayer))) {
        return;
    }


    fm_r.setConfiguration(m_configuration.getResourceConfiguration());
    m_audioPlayer.loadFile(fm_r.combinePath(ResourceType::Audio, "breakout.wav"),
                           AudioFileID::breakout, AudioType::music);

    m_audioPlayer.play(AudioFileID::breakout, true);
    m_audioPlayer.setVolume(AudioFileID::breakout, 100.f);
}

void Runner::run() {
    robot2D::ResourceHandler<robot2D::Texture, Icon> g_icons;
    g_icons.loadFromFile(Icon::Default, fm_r.combinePath(ResourceType::Texture,
                                                         "icon.png"), true);
    std::vector<robot2D::Texture> icons = {g_icons.get(Icon::Default)};
    my_app.setIcon(icons);


    my_app.register_state<IntroState>(States::Intro, my_app);
    my_app.register_state<MenuState>(States::Menu, my_app, m_context);
    my_app.register_state<GameState>(States::Game, my_app, m_context, m_bus);
    my_app.register_state<PauseState>(States::Pause, my_app, m_context);
    my_app.setCurrent(States::Intro);

    my_app.run();
}
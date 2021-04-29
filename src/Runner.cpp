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

#include <robot2D/Util/Logger.h>

#include "game/IntroState.h"
#include "game/MenuState.h"
#include "game/GameState.h"
#include "game/States.h"
#include "game/Runner.hpp"


const std::string resourceiniPath = "res/config.ini";
const std::string gameiniPath = "res/game.ini";
const std::string iconPath = "res/textures/icon.png";
const std::string musicPath = "res/audio/breakout.wav";

Runner::Runner(): my_app{robot2D::vec2u(800, 600),
                         "robot2D Game", true } {
    init();
}


void Runner::init() {
    logger::debug = true;

    m_audioPlayer.loadFile(musicPath.c_str(),
                           AudioFileID::breakout, AudioType::music);

    m_audioPlayer.play(AudioFileID::breakout, true);
    m_audioPlayer.setVolume(AudioFileID::breakout, 100.f);

    if(!m_configuration.loadResources(resourceiniPath)){
        return;
    }

    if(!m_configuration.loadGameSettings(gameiniPath)){
        return;
    }

    if(!m_context.storeInBuffer(ContextID::Configuration, (void*)(&m_configuration))) {
        return;
    }
    if(!m_context.storeInBuffer(ContextID::Audio, (void*)(&m_audioPlayer))){
        return;
    }

}

void Runner::run() {
    robot2D::ResourceHandler<robot2D::Texture, Icon> g_icons;
    g_icons.loadFromFile(Icon::Default, iconPath, true);
    std::vector<robot2D::Texture> icons = {g_icons.get(Icon::Default)};
    my_app.setIcon(icons);


    my_app.register_state<IntroState>(States::Intro, my_app);
    my_app.register_state<MenuState>(States::Menu, my_app);
    my_app.register_state<GameState>(States::Game, my_app, m_context);
    my_app.setCurrent(States::Menu);

    my_app.run();
}
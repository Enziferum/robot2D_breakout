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


#include "game/Audio.h"
#include "game/States.h"


#include "Runner.hpp"

Runner::Runner(): my_app{robot2D::vec2u(800, 600),
                         "robot2D Game", true} {
    init();
}

void Runner::run() {

    my_app.register_state<IntroState>(States::Intro, my_app);
    my_app.register_state<MenuState>(States::Menu, my_app);
    my_app.register_state<GameState>(States::Game, my_app);
    my_app.setCurrent(States::Game);

    robot2D::ResourceHandler<robot2D::Texture, Icon> g_icons;
    g_icons.loadFromFile(Icon::Default, "res/textures/icon.png", true);

    std::vector<robot2D::Texture> icons = {g_icons.get(Icon::Default)};

    my_app.setIcon(icons);

    my_app.run();
}

void Runner::init() {
    logger::debug = true;

    Audio::getInstanse() -> loadFile("res/audio/breakout.wav",
                                     AudioFileID::breakout, AudioType::music);
    Audio::getInstanse() -> play(AudioFileID::breakout, true);
    Audio::getInstanse() -> setVolume(AudioFileID::breakout, 80.f);
}

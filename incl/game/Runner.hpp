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

#include "robot2D/Extra/App.hpp"
#include <robot2D/Util/ResourceHandler.hpp>

#include "AppContext.hpp"
#include "ContextIDs.hpp"
#include "Configuration.hpp"
#include "AudioPlayer.hpp"
#include "MessageBus.hpp"

namespace {
    enum Icon {
        Default
    };
}


class Runner {
public:
    Runner();

    Runner(const Runner &) = delete;

    Runner(const Runner &&) = delete;

    Runner &operator=(const Runner &) = delete;

    Runner &operator=(const Runner &&) = delete;

    ~Runner() = default;

    void run();

private:
    void init();

private:
    //robot2D::ResourceHandler<robot2D::Texture, Icon> icons;
    MessageBus m_bus;
    robot2D::App my_app;

    AppContext<ContextID> m_context;
    AudioPlayer m_audioPlayer;
    Configuration m_configuration;

};

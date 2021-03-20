#include <iostream>
#include "robot2D/Core/App.h"

#include "game/GameState.h"
#include "game/IntroState.h"
//demo state
#include "game/EcsState.h"
#include "game/Audio.h"
#include "game/States.h"


int main() {
    Audio::getInstanse() -> loadFile("res/audio/breakout.wav",
                                     "breakout", AudioType::music);
    Audio::getInstanse() -> play("breakout", true);
    Audio::getInstanse() -> setVolume("breakout", 80.f);

    robot2D::App my_app;

    my_app.register_state<IntroState>(States::Intro, my_app);
    my_app.register_state<GameState>(States::Game, my_app);

    my_app.setCurrent(States::Intro);

    my_app.run();

    return 0;
}

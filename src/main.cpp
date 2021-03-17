#include "game/App.h"

#include "game/GameState.h"
#include "game/IntroState.h"
#include "game/MenuState.h"

enum States{
    Intro = 0,
    Menu = 1,
    Game,
    Pause
};

int main() {
    App app;

    MenuState::Ptr  menu_state = std::make_shared<MenuState>(app);
    GameState::Ptr game_state = std::make_shared<GameState>(app);
    IntroState::Ptr introState = std::make_shared<IntroState>(app);

    app.addState(States::Intro, introState);
    app.addState(States::Menu, menu_state);
    app.addState(States::Game, game_state);

    app.setCurrentState(States::Intro);
    app.run();

    return 0;
}

#include "gameState.h"
#include "gameStateConfig.h"

void gameStateUpdateByKeyPress(GameState &gameState, Clock &deltaTime)
{
    switch (gameState)
    {
    case menu:
        if (Keyboard::isKeyPressed(Keyboard::Return))
            gameState = GameState::reloadGame;
        if (Keyboard::isKeyPressed(Keyboard::Space))
            gameState = GameState::instructions;
        break;

    case instructions:
        if ((Keyboard::isKeyPressed(Keyboard::BackSpace)) || (Keyboard::isKeyPressed(Keyboard::Escape)))
            gameState = GameState::menu;
        break;

    case gameRun:
        if (Keyboard::isKeyPressed(Keyboard::Escape) && deltaTime.getElapsedTime().asSeconds() > 0.2)
        {
            gameState = GameState::pause;
            deltaTime.restart();
        }
        break;

    case pause:
        if (Keyboard::isKeyPressed(Keyboard::Escape) && deltaTime.getElapsedTime().asSeconds() > 0.2)
        {
            gameState = GameState::gameRun;
            deltaTime.restart();
        }
        break;

    case endGame:
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            gameState = GameState::menu;
        break;

    case gameOver:
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            gameState = GameState::menu;
        break;
    }
}

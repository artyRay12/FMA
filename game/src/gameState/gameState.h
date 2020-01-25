#pragma once
using namespace sf;
using namespace std;

enum GameState
{
    menu,
    instructions,
    reloadGame,
    gameRun,
    pause,
    dead,
    gameOver,
    endGame
};

struct gameBackgrounds
{
    Sprite mainMenu;
    Sprite instructions;
    Sprite pause;
    Sprite gameOver;
    RectangleShape darkPause;
    Sprite endGame;
    Sprite titres;
};

map<String, Texture> getGameBackgroundTextures()
{
    map<String, Texture> gameStateTextures;

    Texture menuImageTexture;
    menuImageTexture.loadFromFile("game/wallPaper/menu-screen.png");

    Texture instructionTexture;
    instructionTexture.loadFromFile("game/wallPaper/insructions-screen.png");

    Texture pauseScreenTexture;
    pauseScreenTexture.loadFromFile("game/wallPaper/pause-screen.png");

    Texture gameOverScreenTexture;
    gameOverScreenTexture.loadFromFile("game/wallPaper/game-over.jpg");

    Texture endGameTexture;
    endGameTexture.loadFromFile("game/wallPaper/end-game-screen.png"); //test.png

    Texture titresTexture;
    titresTexture.loadFromFile("game/wallPaper/titres.png");

    gameStateTextures.emplace("mainMenu", menuImageTexture);
    gameStateTextures.emplace("instuctions", instructionTexture);
    gameStateTextures.emplace("pause", pauseScreenTexture);
    gameStateTextures.emplace("gameOver", gameOverScreenTexture);
    gameStateTextures.emplace("endGame", endGameTexture);
    gameStateTextures.emplace("titres", titresTexture);

    return gameStateTextures;
}

void gameBackgroundsInit(struct gameBackgrounds &gameBackgrounds, std::map<String, Texture> &backgroundTextures)
{
    gameBackgrounds.mainMenu.setTexture(backgroundTextures["mainMenu"]);
    gameBackgrounds.instructions.setTexture(backgroundTextures["instuctions"]);
    gameBackgrounds.pause.setTexture(backgroundTextures["pause"]);
    gameBackgrounds.gameOver.setTexture(backgroundTextures["gameOver"]);
    gameBackgrounds.endGame.setTexture(backgroundTextures["endGame"]);
    gameBackgrounds.darkPause.setSize({2000, 2000});
    gameBackgrounds.darkPause.setFillColor(Color(0, 0, 0, 2));
    gameBackgrounds.titres.setTexture(backgroundTextures["titres"]);
}
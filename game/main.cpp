
#include "src/edward/edward.h"
#include "src/effect/effects.h"
#include "src/enemies/combatUtility.h"
#include "src/map/mapController.h"

#define BLOCK_SIZE 32;

using namespace std;
using namespace sf;

constexpr unsigned ANTIALIASING_LEVEL = 8;
constexpr unsigned WINDOW_WIDTH = 1480;
constexpr unsigned WINDOW_HEIGHT = 900;

enum gameState
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

void createWindow(RenderWindow &window)
{
    ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    window.create(
        VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "FullMetal Alchiemist", sf::Style::Default, settings);
}

void pollEvents(sf::RenderWindow &window, Player &edward, gameState &gameState)
{
    Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case Event::Closed:
            window.close();
        }
    }
}

void render(RenderWindow &window, Player &edward, Map &map, Effect &dust, Effect &magicCircle, vector<MeleeEnemy> &meleeEnemys, vector<RangeEnemy> &rangeEnemies, Sprite &playerFrame, int currentLevel, Effect &bloodSplatter, Boss &boss)
{
    window.clear();
    drawMap(window, map, edward, currentLevel);
    drawEffect(window, dust, magicCircle, bloodSplatter);
    drawMeleeEnemy(window, meleeEnemys, edward);
    drawRangeEnemy(window, rangeEnemies, edward);
    bossDraw(window, boss);
    window.draw(playerFrame);
    drawPlayer(window, edward);
    window.draw(playerFrame);
    window.display();
}

void pressKeyUpdate(Player &edward, Effect &dust, Effect &magicCircle, Effect &bloodSplatter)
{
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        edwardPressRightUpdates(edward);
    }

    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        edwardPressLeftUpdates(edward);
    }

    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        effectsPressUpUpdates(dust, magicCircle, edward);
        edwardPressUpUpdates(edward);
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        effectsPressSpaceUpdates(bloodSplatter, edward);
        edwardPressSpaceUpdates(edward);
    }

    if (Keyboard::isKeyPressed(Keyboard::Num1))
    {
        edwardPressOneUpdates(edward);
        effectsPressOneUpdates(edward, bloodSplatter);
    }
}

void gameEndChecker(Boss boss, gameState &gameState)
{
    if (!boss.alive)
    {
        gameState = gameState::endGame;
        boss.state = bossState::none;
    }
}

void menuUpdate(gameState &gameState, Clock &deltaTime)
{
    switch (gameState)
    {
    case menu:
        if (Keyboard::isKeyPressed(Keyboard::Return))
            gameState = gameState::reloadGame;
        if (Keyboard::isKeyPressed(Keyboard::Space))
            gameState = gameState::instructions;
        break;
    case instructions:
        if ((Keyboard::isKeyPressed(Keyboard::BackSpace)) || (Keyboard::isKeyPressed(Keyboard::Escape)))
            gameState = gameState::menu;
        break;
    case gameRun:
        if (Keyboard::isKeyPressed(Keyboard::Escape) && deltaTime.getElapsedTime().asSeconds() > 0.2)
        {
            gameState = gameState::pause;
            deltaTime.restart();
        }
        break;
    case pause:
        if (Keyboard::isKeyPressed(Keyboard::Escape) && deltaTime.getElapsedTime().asSeconds() > 0.2)
        {
            gameState = gameState::gameRun;
            deltaTime.restart();
        }
        break;
    case endGame:
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            gameState = gameState::menu;
        break;
    case gameOver:
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            gameState = gameState::menu;
        break;
    }
}

void levelChanger(int &currentLevel, Player &edward, Map &map, Clock &levelChanger, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, std::map<String, Texture> &mapTextures)
{
    if (((edward.nextLevel) && levelChanger.getElapsedTime().asSeconds() > 2))
    {
        currentLevel++;
        edwardInit(edward);
        mapInit(mapTextures, map, currentLevel);
        map.titleDraw = true;
        map.mapTimer.restart();
        edward.nextLevel = false;
        enemiesInit(meleeEnemies, rangeEnemies, currentLevel);
        levelChanger.restart();
    }
}

void deathChecker(int &currentLevel, Player &edward, Map &map, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, std::map<String, Texture> &mapTextures, gameState &gameState)
{
    if (edward.health == 0)
    {
        edwardInit(edward);
        mapInit(mapTextures, map, currentLevel);
        map.titleDraw = true;
        enemiesInit(meleeEnemies, rangeEnemies, currentLevel);
        edward.lives--;
        switch (edward.lives)
        {
        case 1:
            edward.livesImage.setTexture(*edward.live1Texture);
            break;
        case 2:
            edward.livesImage.setTexture(*edward.live2Texture);
            break;
        case 3:
            edward.livesImage.setTexture(*edward.live3Texture);
            break;
        }
    }

    if (edward.lives == 0)
    {
        gameState = gameState::gameOver;
    }
}

void update(float deltaTime, Player &edward, Effect &dust, Effect &magicCircle, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, Map &map, int &currentLevel, Effect &bloodSplatter, Boss &boss, gameState &gameState, std::map<String, Texture> &mapTextures)
{
    deathChecker(currentLevel, edward, map, meleeEnemies, rangeEnemies, mapTextures, gameState);
    effectUpdate(dust, magicCircle, edward, deltaTime, bloodSplatter);
    edwardUpdate(deltaTime, edward, map.solidPtr, map.blockSize, currentLevel, map.Xlimits, map.Ylimits);
    pressKeyUpdate(edward, dust, magicCircle, bloodSplatter);
    bossUpdate(edward, boss, deltaTime);
    gameEndChecker(boss, gameState);
    mapUpdate(map, edward, deltaTime);
    enemiesUpdate(edward, deltaTime, meleeEnemies, rangeEnemies, map.solidPtr, map.blockSize);
}

int main()
{
    /* ИГРА */
    sf::RenderWindow window;
    createWindow(window);
    Clock timer;
    int currentLevel = 1;
    gameState gameState = gameState::menu;
    Clock levelChangerTimer;
    Clock gameStateTimer;
    float gameSpeed = 800;

    /* ВРАГИ */
    map<String, Texture> meleeEnemyTextures = getMeleeEnemyTextures();
    map<String, Texture> rangeEnemyTextures = getRangeEnemyTextures();
    map<String, Texture> bossTextures = getBossTextures();
    vector<MeleeEnemy> meleeEnemies(6, meleeEnemyTextures);
    vector<RangeEnemy> rangeEnemies(6, rangeEnemyTextures);

    /* БОСС */
    Boss boss(bossTextures);

    /* ИГРОК */
    map<String, Texture> playerTextures = getPlayerTextures();
    Player edward(playerTextures);

    /* ЕФФЕКТЫ */
    Effect dust("dust", "game/sprites/effects/dust.png");
    Effect magicCircle("magic-circle", "game/sprites/Effects/magic-circle.png");
    Effect bloodSplatter("blood-splatter", "game/sprites/Effects/blood-splatter.png");
    map<String, Effect *> effects;
    effects.emplace("dust", &dust);
    effects.emplace("magicCircle", &magicCircle);
    effects.emplace("bloodSplatter", &bloodSplatter);

    /* КАРТА */
    map<String, Texture> mapTextures = getMapTextures();
    Map map;
    Clock levelChangerTime;

    /* ИНТЕРФЕЙС */
    Sprite playerFrame;
    Texture playerFrameTexture;

    Sprite menuImage;
    Texture menuImageTexture;
    menuImageTexture.loadFromFile("game/wallPaper/menu-screen.png");
    menuImage.setTexture(menuImageTexture);

    Sprite instructionImage;
    Texture instructionTexture;
    instructionTexture.loadFromFile("game/wallPaper/insructions-screen.png");
    instructionImage.setTexture(instructionTexture);

    Sprite pauseScreen;
    Texture pauseScreenTexture;
    pauseScreenTexture.loadFromFile("game/wallPaper/pause-screen.png");
    pauseScreen.setTexture(pauseScreenTexture);

    Sprite gameOverScreen;
    Texture gameOverScreenTexture;
    gameOverScreenTexture.loadFromFile("game/wallPaper/game-over.jpg");
    gameOverScreen.setTexture(gameOverScreenTexture);

    RectangleShape darkTheme;
    darkTheme.setSize({2000, 2000});
    darkTheme.setFillColor(Color(0, 0, 0, 2));

    Sprite endGameImage;
    Texture endGameTexture;
    endGameTexture.loadFromFile("game/wallPaper/end-game-screen.png"); //test.png
    endGameImage.setTexture(endGameTexture);

    Sprite titres;
    Texture titresTexture;
    titresTexture.loadFromFile("game/wallPaper/titres.png");
    titres.setTexture(titresTexture);

    playerFrameTexture.loadFromFile("game/sprites/Interface/player-frame.png");
    playerFrame.setTexture(playerFrameTexture);

    float titresY = 900;

    while (window.isOpen())
    {
        switch (gameState)
        {
        case menu:
            timer.restart();
            window.clear();
            window.draw(menuImage);
            window.display();
            break;
        case instructions:
            timer.restart();
            window.clear();
            window.draw(instructionImage);
            window.display();
            break;
        case endGame:
            timer.restart();
            titres.setPosition(800, titresY);
            titresY = titresY - 0.03;
            window.clear();
            window.draw(endGameImage);
            window.draw(titres);
            window.display();
            break;
        case pause:
            window.draw(darkTheme);
            window.draw(pauseScreen);
            window.display();
            timer.restart();
            break;
        case gameOver:
            boss.view.reset(FloatRect(0, 0, 1440, 900));
            window.draw(gameOverScreen);
            window.setView(boss.view);
            window.display();
            break;
        case reloadGame:
            titresY = 900;
            currentLevel = 3;
            edward.lives = 3;
            mapInit(mapTextures, map, currentLevel);
            enemiesInit(meleeEnemies, rangeEnemies, currentLevel);
            bossInit(boss);
            edwardInit(edward);
            gameState = gameState::gameRun;
            break;
        case gameRun:
            levelChanger(currentLevel, edward, map, levelChangerTimer, meleeEnemies, rangeEnemies, mapTextures);
            float time = timer.getElapsedTime().asMicroseconds();
            timer.restart();
            time = time / gameSpeed;
            update(time, edward, dust, magicCircle, meleeEnemies, rangeEnemies, map, currentLevel, bloodSplatter, boss, gameState, mapTextures);
            render(window, edward, map, dust, magicCircle, meleeEnemies, rangeEnemies, playerFrame, currentLevel, bloodSplatter, boss);
            break;
        }
        menuUpdate(gameState, gameStateTimer);
        pollEvents(window, edward, gameState);
    }
}

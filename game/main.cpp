#include "src/edward/edward.h"
#include "src/effect/effects.h"
#include "src/enemies/combatUtility.h"
#include "src/map/mapController.h"
#include "src/gameState/gameStateController.h"
#include "src/gameState/gameState.h"

#define BLOCK_SIZE 32;

using namespace std;
using namespace sf;

constexpr unsigned ANTIALIASING_LEVEL = 8;
constexpr unsigned WINDOW_WIDTH = 1480;
constexpr unsigned WINDOW_HEIGHT = 900;
const int GAME_SPEED = 800;

void createWindow(RenderWindow &window)
{
    ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    window.create(
        VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "FullMetal Alchiemist", sf::Style::Default, settings);
}

void pollEvents(sf::RenderWindow &window, Player &edward)
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

void render(RenderWindow &window, Player &edward, Map &map, Effect &dust, Effect &magicCircle, vector<MeleeEnemy> &meleeEnemys, vector<RangeEnemy> &rangeEnemies, Sprite &playerFrame, int currentLevel, Effect &bloodSplatter, Boss &boss, gameBackgrounds &gameBackgrounds, GameState &gameState)
{
    if (gameState != GameState::pause)
        window.clear();
    switch (gameState)
    {
    case menu:
        window.draw(gameBackgrounds.mainMenu);
        break;
    case instructions:
        window.draw(gameBackgrounds.instructions);
        break;
    case endGame:
        boss.view.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        window.setView(boss.view);
        window.draw(gameBackgrounds.endGame);
        window.draw(gameBackgrounds.titres);
        break;
    case pause:
        boss.view.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        window.draw(gameBackgrounds.darkPause);
        window.draw(gameBackgrounds.pause);
        break;
    case gameOver:
        boss.view.reset(FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        window.setView(boss.view);
        window.draw(gameBackgrounds.gameOver);
        break;
    case gameRun:
        drawMap(window, map, edward, currentLevel);
        drawEffect(window, dust, magicCircle, bloodSplatter);
        drawMeleeEnemy(window, meleeEnemys, edward);
        drawRangeEnemy(window, rangeEnemies, edward);
        bossDraw(window, boss);
        window.draw(playerFrame);
        drawPlayer(window, edward);
        window.draw(playerFrame);
    }
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

void gameEndChecker(Boss boss, GameState &gameState)
{
    if (!boss.alive)
    {
        gameState = GameState::endGame;
        boss.state = BossState::none;
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

void deathChecker(int &currentLevel, Player &edward, Map &map, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, std::map<String, Texture> &mapTextures, GameState &gameState)
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
        gameState = GameState::gameOver;
    }
}

void update(float deltaTime, Player &edward, Effect &dust, Effect &magicCircle, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, Map &map, int &currentLevel, Effect &bloodSplatter, Boss &boss, GameState &gameState, std::map<String, Texture> &mapTextures)
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
    GameState gameState = GameState::menu;
    Clock levelChangerTimer;
    Clock gameStateTimer;
    float gameSpeed = GAME_SPEED;

    map<String, Texture> gameBackgroundTextures = getGameBackgroundTextures();
    struct gameBackgrounds gameBackgrounds;
    gameBackgroundsInit(gameBackgrounds, gameBackgroundTextures);

    /* ВРАГИ */
    map<String, Texture> meleeEnemyTextures = getMeleeEnemyTextures();
    map<String, Texture> rangeEnemyTextures = getRangeEnemyTextures();

    vector<MeleeEnemy> meleeEnemies(6, meleeEnemyTextures);
    vector<RangeEnemy> rangeEnemies(6, rangeEnemyTextures);

    /* БОСС */
    map<String, Texture> bossTextures = getBossTextures();
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

    float titresY = TITRES_INITIAL_POS_Y;

    while (window.isOpen())
    {
        switch (gameState)
        {
        case endGame:
            titresY = titresY - TITRES_SPEED * timer.getElapsedTime().asSeconds();
            gameBackgrounds.titres.setPosition(TITRES_INITIAL_POS_X, titresY);
            break;

        case reloadGame:
            titresY = TITRES_INITIAL_POS_Y;
            currentLevel = 1;
            edward.lives = EDWARD_INITIAL_LIVES;
            mapInit(mapTextures, map, currentLevel);
            enemiesInit(meleeEnemies, rangeEnemies, currentLevel);
            bossInit(boss);
            edwardInit(edward);
            gameState = GameState::gameRun;
            break;
        case gameRun:
            levelChanger(currentLevel, edward, map, levelChangerTimer, meleeEnemies, rangeEnemies, mapTextures);
            float time = timer.getElapsedTime().asMicroseconds();
            time = time / gameSpeed;
            update(time, edward, dust, magicCircle, meleeEnemies, rangeEnemies, map, currentLevel, bloodSplatter, boss, gameState, mapTextures);
            break;
        }
        timer.restart();
        render(window, edward, map, dust, magicCircle, meleeEnemies, rangeEnemies, playerFrame, currentLevel, bloodSplatter, boss, gameBackgrounds, gameState);
        gameStateUpdateByKeyPress(gameState, gameStateTimer);
        pollEvents(window, edward);
    }
}

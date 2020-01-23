#include "firstLevelMap.h"
#include "secondLevelMap.h"
#include "thirdLevelMap.h"
#include "map.h"

mapInit(map<String, Texture> &mapTextures, Map &map, int currentLevel)
{
    map.title.setPosition(700, 300);
    map.title.setOrigin(385, 70);
    map.closedDoor = &mapTextures["doorClose"];
    map.doorClose.setTexture(*map.closedDoor);
    map.doorClose.setOrigin(0, 220);

    map.openDoor = &mapTextures["doorOpen"];
    map.doorOpen.setTexture(*map.openDoor);
    map.doorOpen.setOrigin(0, 220);

    map.keyTexture = &mapTextures["keyTexture"];
    map.key.setTexture(*map.keyTexture);
    switch (currentLevel)
    {
    case 1:
        map.solidPtr = firstLevelMap;
        map.mapTexture = &mapTextures["firstLevel"];
        map.sprite.setTexture(*map.mapTexture);
        map.title.setTexture(mapTextures["firstLevelTitle"]);
        map.blockSize = 32;

        map.Xlimits.emplace("left", 700);
        map.Xlimits.emplace("right", 6800);
        map.Ylimits.emplace("top", 400);
        map.Ylimits.emplace("bottom", 1100);

        map.Xlimits["left"] = 700;
        map.Xlimits["right"] = 6800;
        map.Ylimits["top"] = 400;
        map.Ylimits["bottom"] = 1100;

        break;
    case 2:
        map.solidPtr = secondLevelMap;
        map.mapTexture = &mapTextures["secondLevel"];
        map.sprite.setTexture(*map.mapTexture);
        map.title.setTexture(mapTextures["secondLevelTitle"]);
        map.blockSize = 32;

        map.Xlimits["right"] = 8800;
        break;
    case 3:
        map.solidPtr = thirdLevelMap;
        map.mapTexture = &mapTextures["thirdLevel"];
        map.sprite.setTexture(*map.mapTexture);
        map.title.setTexture(mapTextures["thirdLevelTitle"]);
        map.blockSize = 24;

        map.Xlimits["left"] = 700;
        map.Xlimits["right"] = 13600;
        map.Ylimits["top"] = 400;
        map.Ylimits["bottom"] = 700;
        break;
    }
}

map<String, Texture> getMapTextures()
{
    map<String, Texture> mapTextures;

    Texture firstLevel;
    firstLevel.loadFromFile("game/maps/level-1.png");

    Texture secondLevel;
    secondLevel.loadFromFile("game/maps/level-2.png");

    Texture thirdLevel;
    thirdLevel.loadFromFile("game/maps/level-3.png");

    Texture firstLevelTitle;
    firstLevelTitle.loadFromFile("game/maps/mapTitles/level-1-title.png");

    Texture secondLevelTitle;
    secondLevelTitle.loadFromFile("game/maps/mapTitles/level-2-title.png");

    Texture thirdLevelTitle;
    thirdLevelTitle.loadFromFile("game/maps/mapTitles/level-3-title.png");

    Texture doorClose;
    doorClose.loadFromFile("game/maps/interactions/door-close.png");

    Texture doorOpen;
    doorOpen.loadFromFile("game/maps/interactions/door-open.png");

    Texture keyTexture;
    keyTexture.loadFromFile("game/maps/interactions/key.png");

    mapTextures.emplace("firstLevel", firstLevel);
    mapTextures.emplace("secondLevel", secondLevel);
    mapTextures.emplace("thirdLevel", thirdLevel);
    mapTextures.emplace("firstLevelTitle", firstLevelTitle);
    mapTextures.emplace("secondLevelTitle", secondLevelTitle);
    mapTextures.emplace("thirdLevelTitle", thirdLevelTitle);
    mapTextures.emplace("doorClose", doorClose);
    mapTextures.emplace("doorOpen", doorOpen);
    mapTextures.emplace("keyTexture", keyTexture);

    return mapTextures;
}

void titleAnimations(Map &map, float time)
{
    float mapTime = map.mapTimer.getElapsedTime().asMicroseconds();
    if (mapTime > 1200000)
    {
        map.titleTransparent -= time * 0.2;
        map.titleScale += time * 0.001;
    }
    if (map.titleTransparent < 20)
    {
        map.titleTransparent = 255;
        map.titleScale = 1;
        map.titleDraw = false;
    }
    map.title.setScale(map.titleScale, map.titleScale);
    map.title.setColor(sf::Color(255, 255, 255, map.titleTransparent));
};

void mapUpdate(Map &map, Player &edward, float time)
{
    if (map.titleDraw)
        titleAnimations(map, time);

    map.sprite.setPosition(0 - edward.offsetX, 0 - edward.offsetY);
}

void drawMap(sf::RenderWindow &window, Map &map, Player edward, int currentLevel)
{
    RectangleShape rectangle(Vector2f(map.blockSize, map.blockSize));

    Sprite healthRect;
    Texture healthTexture;
    healthTexture.loadFromFile("game/sprites/Interface/health.png");
    healthRect.setTexture(healthTexture);
    window.draw(map.sprite);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
        {
            if (map.solidPtr[i][j] == 'B')
            {
                rectangle.setFillColor(Color::Transparent);
                rectangle.setPosition(j * map.blockSize - edward.offsetX, i * map.blockSize - edward.offsetY);
                window.draw(rectangle);
            }

            if (map.solidPtr[i][j] == 'H')
            {
                healthRect.setPosition(j * map.blockSize - edward.offsetX, i * map.blockSize - edward.offsetY - 30);
                window.draw(healthRect);
            }

            if (map.solidPtr[i][j] == 's')
            {
                map.doorClose.setPosition(j * map.blockSize - edward.offsetX, i * map.blockSize - edward.offsetY);
                window.draw(map.doorClose);
            }

            if (map.solidPtr[i][j] == 'o')
            {
                map.doorOpen.setPosition(j * map.blockSize - edward.offsetX, i * map.blockSize - edward.offsetY);
                window.draw(map.doorOpen);
            }

            if (map.solidPtr[i][j] == 'K')
            {
                map.key.setPosition(j * map.blockSize - edward.offsetX, i * map.blockSize - edward.offsetY - 30);
                map.keyAnimation();
                window.draw(map.key);
            }
        }

    if (map.titleDraw)
        window.draw(map.title);
}
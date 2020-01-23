
#include <iostream>
#include <vector>
#include "meleeEnemy/meleeEnemy.h"
#include "rangeEnemy/rangeEnemy.h"
#include "boss/boss.h"
using namespace sf;
using namespace std;

enemiesInit(vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, int currentLevel)
{
    /* MELEE COORD */
    Vector2f startMeleeEnemyCoords[6]{
        {530, 580},
        {807, 1025},
        {3000, 1160},
        {4160, 770},
        {6300, 415},
        {6300, 1312}};

    Vector2f MeleeEnemySecondLevelCoords[6]{
        {300, 1220},
        {1285, 1252},
        {2845, 1156},
        {6748, 1122},
        {6506, 708},
        {8015, 1028}};

    Vector2f MeleeEnemyThirdLevelCoords[6]{
        {560, 410},
        {2000, 690},
        {2964, 410},
        {6554, 210},
        {7773, 940},
        {11598, 221}};

    /* RANGE COORD */
    Vector2f startRangeEnemyCoords[6]{
        {95, 920},
        {2378, 1174},
        {3565, 980},
        {4250, 1460},
        {5263, 1322},
        {7370, 1172}};

    Vector2f RangeEnemySecondLevelCoords[6]{
        {1525, 750},
        {1666, 1200},
        {40391, 9021},
        {5034, 1062},
        {7642, 1158},
        {8135, 4861}};

    Vector2f RangeEnemyThirdLevelCoords[6]{
        {537, 910},
        {1666, 1240},
        {5176, 678},
        {10294, 822},
        {8480, 222},
        {12481, 774}};

    for (int i = 0; i < meleeEnemies.size(); i++)
    {
        switch (currentLevel)
        {
        case 1:
            meleeEnemies[i].pos = startMeleeEnemyCoords[i];
            rangeEnemies[i].pos = startRangeEnemyCoords[i];
            break;
        case 2:
            meleeEnemies[i].pos = MeleeEnemySecondLevelCoords[i];
            rangeEnemies[i].pos = RangeEnemySecondLevelCoords[i];
            break;
        case 3:
            meleeEnemies[i].pos = MeleeEnemyThirdLevelCoords[i];
            rangeEnemies[i].pos = RangeEnemyThirdLevelCoords[i];
            break;
        }
        meleeEnemies[i].sprite.setPosition(meleeEnemies[i].pos.x, meleeEnemies[i].pos.y);
        meleeEnemies[i].speedX = 0.1;
        meleeEnemies[i].serialNumber = i;
        meleeEnemies[i].sprite.setOrigin(75, 0);
        meleeEnemies[i].alive = true;
        meleeEnemies[i].deathAnimation = false;
        meleeEnemies[i].ripTime.restart();
        meleeEnemies[i].deathLeftRect.left = 100;
        meleeEnemies[i].deathRightRect.left = 0;
        meleeEnemies[i].state = enemyState::patrol;

        rangeEnemies[i].sprite.setPosition(rangeEnemies[i].pos.x, rangeEnemies[i].pos.y);
        rangeEnemies[i].speedX = 0;
        rangeEnemies[i].serialNumber = i;
        rangeEnemies[i].fireBallPos = rangeEnemies[i].pos;
        rangeEnemies[i].alive = true;
        rangeEnemies[i].deathAnimation = false;
        rangeEnemies[i].deathLeftRect.left = 100;
        rangeEnemies[i].deathRightRect.left = 0;
        rangeEnemies[i].ripTime.restart();
    }
}

void enemiesUpdate(Player &edward, float deltaTime, vector<MeleeEnemy> &meleeEnemies, vector<RangeEnemy> &rangeEnemies, String *solidPtr, float blockSize)
{
    int enemySize = rangeEnemies.size();
    for (int i = 0; i < enemySize; i++)
    {
        if ((meleeEnemies[i].pos.x < edward.pos.x + 700) && (meleeEnemies[i].pos.x > edward.pos.x - 700) && (meleeEnemies[i].alive))
        {
            meleeEnemyUpdate(edward, deltaTime, meleeEnemies[i], solidPtr, blockSize);
        }

        if (isRangeEnemyOnScreen(edward, rangeEnemies[i]))
        {
            rangeEnemyUpdate(edward, deltaTime, rangeEnemies[i], solidPtr);
        }
    }
}

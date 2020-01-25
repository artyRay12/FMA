#include <iostream>
#include "meleeEnemyConfig.h"

using namespace sf;
using namespace std;

enum EnemyState
{
    patrol,
    stand,
    combat,
    combatStay,
    death,
};

class MeleeEnemy
{
  public:
    MeleeEnemy(map<String, Texture> &meleeEnemyTextures)
    {
        standTexture = &meleeEnemyTextures["stand"];
        runTexture = &meleeEnemyTextures["run"];
        attackTexture = &meleeEnemyTextures["attack"];
        deathTexture = &meleeEnemyTextures["death"];
    }
    Sprite sprite;

    Texture *standTexture;
    Texture *runTexture;
    Texture *attackTexture;
    Texture *deathTexture;

    IntRect standRightRect{STAND_RIGHT_SIZE};
    IntRect standLeftRect{STAND_LEFT_SIZE};

    IntRect runRightRect{RUN_RIGHT_SIZE};
    IntRect runLeftRect{RUN_LEFT_SIZE};

    IntRect attackRightRect{ATTACK_RIGHT_SIZE};
    IntRect attackLeftRect{ATTACK_LEFT_SIZE};

    IntRect deathRightRect{DEATH_RIGHT_SIZE};
    IntRect deathLeftRect{DEATH_LEFT_SIZE};

    EnemyState state = EnemyState::patrol;
    LineOfSight sight = LineOfSight::right;

    Clock ripTime;
    Clock patrolTime;
    Clock meleeEnemyTime;
    LineOfSight posRelative;

    bool alive = true;
    bool attackZone = false;
    bool hitConfirm = false;
    bool deathAnimation;

    Vector2f pos;

    float speedX, speedY;
    float offsetX = 0, offsetY = 0;
    float time = meleeEnemyTime.restart().asSeconds();
    int serialNumber;

    void animation(Texture *&texture, IntRect &rect, int FrameGap, int minFrame, int maxFrame, float interval)
    {
        sprite.setTexture((*texture));
        sprite.setTextureRect(rect);
        if (meleeEnemyTime.getElapsedTime().asSeconds() > interval)
        {
            if (rect.left == maxFrame)
            {
                rect.left = minFrame;
            }
            else
                rect.left += FrameGap;
            meleeEnemyTime.restart();
        }
    }

    void attackRight()
    {
        sprite.setTexture((*attackTexture));
        sprite.setTextureRect(attackRightRect);
        if (meleeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (attackRightRect.left == 1050)
            {
                attackRightRect.left = 0;
                hitConfirm = false;
            }
            else
                attackRightRect.left += 150;
            if (attackRightRect.left == 750)
            {
                hitConfirm = true;
            }
            meleeEnemyTime.restart();
        }
    }

    void attackLeft()
    {
        sprite.setTexture((*attackTexture));
        sprite.setTextureRect(attackLeftRect);
        if (meleeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (attackLeftRect.left == 1200)
            {
                attackLeftRect.left = 150;
                hitConfirm = false;
            }
            else
                attackLeftRect.left += 150;
            if (attackLeftRect.left == 750)
            {
                hitConfirm = true;
            }
            meleeEnemyTime.restart();
        }
    }

    void deathRight()
    {
        sprite.setTexture((*deathTexture));
        sprite.setTextureRect(deathRightRect);
        if (meleeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (deathRightRect.left == 600)
            {
                deathRightRect.left += 0;
            }
            else
                deathRightRect.left += 100;
            meleeEnemyTime.restart();
        }
    }

    void deathLeft()
    {
        sprite.setTexture((*deathTexture));
        sprite.setTextureRect(deathLeftRect);
        if (meleeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (deathLeftRect.left == 700)
            {
                deathLeftRect.left += 0;
            }
            else
                deathLeftRect.left += 100;
            meleeEnemyTime.restart();
        }
    }
};

bool isPlayerNear(Player &edward, MeleeEnemy &meleeEnemy)
{
    if (((meleeEnemy.pos.x - edward.pos.x > -500) && (meleeEnemy.pos.x - edward.pos.x < 500)) &&
        ((meleeEnemy.pos.y - edward.pos.y > -40) && (meleeEnemy.pos.y - edward.pos.y < 40)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isPlayerInAttackZone(Player edward, MeleeEnemy &meleeEnemy)
{
    if (((meleeEnemy.pos.x - edward.pos.x > -70) &&
         (meleeEnemy.pos.x - edward.pos.x < 0) &&
         (meleeEnemy.sight == LineOfSight::right)) ||
        ((meleeEnemy.pos.x - edward.pos.x < 70) &&
         (meleeEnemy.pos.x - edward.pos.x > 0) &&
         (meleeEnemy.sight == LineOfSight::left)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

String *getMap(String *arr)
{
    return arr;
}

LineOfSight getRelativeSide(Player edward, MeleeEnemy &meleeEnemy)
{
    if (edward.pos.x - meleeEnemy.pos.x > 0)
    {
        return LineOfSight::right;
    }
    else
    {
        return LineOfSight::left;
    }
}

void meleehitCheck(Player &edward, MeleeEnemy &meleeEnemy)
{
    /* PUNCH */
    if ((meleeEnemy.pos.y - edward.pos.y > -40) &&
        (meleeEnemy.pos.y - edward.pos.y < 40))
    {
        if ((edward.pos.x - meleeEnemy.pos.x < 100) &&
            (edward.pos.x - meleeEnemy.pos.x > 10) && (edward.sight == LineOfSight::left) && (edward.hitConfirm))

        {
            edward.isEdwardHitEnemy = true;
            edward.enemyWhoDie.x = meleeEnemy.pos.x - edward.offsetX;
            edward.enemyWhoDie.y = meleeEnemy.pos.y - edward.offsetY;
            meleeEnemy.deathAnimation = true;
            edward.hitConfirm = false;
            meleeEnemy.state = EnemyState::death;
        }

        else if ((edward.pos.x - meleeEnemy.pos.x < 10) &&
                 (edward.pos.x - meleeEnemy.pos.x > -100) && (edward.sight == LineOfSight::right) && (edward.hitConfirm))
        {
            edward.isEdwardHitEnemy = true;
            edward.enemyWhoDie.x = meleeEnemy.pos.x - edward.offsetX;
            edward.enemyWhoDie.y = meleeEnemy.pos.y - edward.offsetY;
            meleeEnemy.deathAnimation = true;
            edward.hitConfirm = false;
            meleeEnemy.state = EnemyState::death;
        }
    }

    /* SHELL */
    if ((((meleeEnemy.sprite.getPosition().x - edward.offsetX) - edward.shell.getPosition().x < 5) && ((meleeEnemy.sprite.getPosition().x - edward.offsetX) - edward.shell.getPosition().x > -5)) &&
        (((meleeEnemy.sprite.getPosition().y - edward.offsetY) - edward.shell.getPosition().y < 80) && ((meleeEnemy.sprite.getPosition().y - edward.offsetY) - edward.shell.getPosition().y > -80)))

    {
        edward.isShellFlying = false;
        edward.isEdwardHitEnemy = true;
        edward.enemyWhoDie.x = meleeEnemy.pos.x - edward.offsetX;
        edward.enemyWhoDie.y = meleeEnemy.pos.y - edward.offsetY;
        meleeEnemy.deathAnimation = true;
        meleeEnemy.state = EnemyState::death;
    }

    if (meleeEnemy.hitConfirm)
    {
        edwardLoseHisHealth(edward);
        meleeEnemy.hitConfirm = false;
    }
}

void updatesByAnimation(MeleeEnemy &meleeEnemy)
{
    if ((meleeEnemy.deathRightRect.left != 600) && (meleeEnemy.deathLeftRect.left != 700))
    {
        meleeEnemy.ripTime.restart().asSeconds();
    }

    if (meleeEnemy.ripTime.getElapsedTime().asSeconds() > 5)
    {
        meleeEnemy.alive = false;
    }
}

map<String, Texture> getMeleeEnemyTextures()
{
    map<String, Texture> meleeEnemyTextures;
    Texture stand;
    stand.loadFromFile("game/sprites/Enemies/melee-enemy-stand.png");

    Texture run;
    run.loadFromFile("game/sprites/Enemies/melee-enemy-run.png");

    Texture attack;
    attack.loadFromFile("game/sprites/Enemies/melee-enemy-attack.png");

    Texture death;
    death.loadFromFile("game/sprites/Enemies/melee-enemy-death.png");

    meleeEnemyTextures.emplace("stand", stand);
    meleeEnemyTextures.emplace("run", run);
    meleeEnemyTextures.emplace("attack", attack);
    meleeEnemyTextures.emplace("death", death);

    return meleeEnemyTextures;
}

void meleeEnemyAnimation(MeleeEnemy &meleeEnemy)
{

    if (meleeEnemy.deathAnimation)
    {
        if (meleeEnemy.sight == LineOfSight::left)
        {
            meleeEnemy.deathRight();
        }
        else
        {
            meleeEnemy.deathLeft();
        }
    }
    else
    {
        if (meleeEnemy.speedX != 0)
        {
            if (meleeEnemy.speedX > 0)
            {
                meleeEnemy.animation(meleeEnemy.runTexture, meleeEnemy.runRightRect, 90, 0, 630, 0.1);
            }
            else
            {
                meleeEnemy.animation(meleeEnemy.runTexture, meleeEnemy.runLeftRect, 90, 90, 720, 0.1);
            }
        }
        else
        {
            if (meleeEnemy.attackZone)
            {
                if (meleeEnemy.sight == LineOfSight::right)
                {
                    meleeEnemy.attackRight();
                }
                else
                {
                    meleeEnemy.attackLeft();
                }
            }
            else
            {
                if (meleeEnemy.sight == LineOfSight::right)
                {
                    meleeEnemy.animation(meleeEnemy.standTexture, meleeEnemy.attackRightRect, 60, 0, 300, 0.2);
                }
                else
                {
                    meleeEnemy.animation(meleeEnemy.standTexture, meleeEnemy.attackLeftRect, 60, 60, 360, 0.2);
                }
            }
        }
    }
}

void patrolMode(Player edward, float deltaTime, MeleeEnemy &meleeEnemy)
{
    if (meleeEnemy.speedX == 0)
        meleeEnemy.sight == LineOfSight::right ? meleeEnemy.speedX = 0.1 : meleeEnemy.speedX = -0.1;

    if (meleeEnemy.speedX > 0)
    {
        meleeEnemy.sight = LineOfSight::right;
    }
    else
    {
        meleeEnemy.sight = LineOfSight::left;
    }

    if (!meleeEnemy.deathAnimation)
        meleeEnemy.pos.x += meleeEnemy.speedX * deltaTime;

    if (isPlayerNear(edward, meleeEnemy))
    {
        meleeEnemy.state = EnemyState::combat;
    }

    meleeEnemy.sprite.setPosition(meleeEnemy.pos.x - edward.offsetX, meleeEnemy.pos.y - edward.offsetY);
}

void combatMode(Player &edward, float deltaTime, MeleeEnemy &meleeEnemy)
{

    if (!isPlayerNear(edward, meleeEnemy))
    {
        meleeEnemy.state = EnemyState::patrol;
    }

    if (isPlayerInAttackZone(edward, meleeEnemy))
    {
        meleeEnemy.speedX = 0;
        meleeEnemy.attackZone = true;
    }
    else
    {
        meleeEnemy.posRelative = getRelativeSide(edward, meleeEnemy);
        meleeEnemy.attackZone = false;
        meleeEnemy.attackLeftRect.left = 0;
        meleeEnemy.attackRightRect.left = 0;

        if (meleeEnemy.posRelative == LineOfSight::right)
        {
            meleeEnemy.speedX = 0.1;
            meleeEnemy.sight = LineOfSight::right;
        }
        else
        {
            meleeEnemy.speedX = -0.1;
            meleeEnemy.sight = LineOfSight::left;
        }
    }

    if (!meleeEnemy.deathAnimation)
        meleeEnemy.pos.x += meleeEnemy.speedX * deltaTime;
    meleeEnemy.sprite.setPosition(meleeEnemy.pos.x - edward.offsetX, meleeEnemy.pos.y - edward.offsetY);
}

void combatStayMode(Player edward, float deltaTime, MeleeEnemy &meleeEnemy)
{
    if (!isPlayerNear(edward, meleeEnemy))
    {
        meleeEnemy.state = EnemyState::patrol;
    }

    if (isPlayerInAttackZone(edward, meleeEnemy))
    {
        meleeEnemy.state = EnemyState::combat;
        meleeEnemy.speedX = 0;
        meleeEnemy.attackZone = true;
    }
    meleeEnemy.sprite.setPosition(meleeEnemy.pos.x - edward.offsetX, meleeEnemy.pos.y - edward.offsetY);
}

void drawMeleeEnemy(RenderWindow &window, vector<MeleeEnemy> &meleeEnemies, Player edward)
{
    for (int i = 0; i < meleeEnemies.size(); i++)
    {
        if ((meleeEnemies[i].pos.x < edward.pos.x + 700) && (meleeEnemies[i].pos.x > edward.pos.x - 700) && (meleeEnemies[i].alive))
        {
            window.draw(meleeEnemies[i].sprite);
        }
    }
}

void collisionEnemy(MeleeEnemy &meleeEnemy, Player edward, String *solidPtr, float blockSize)
{
    for (int i = meleeEnemy.pos.y / blockSize; i < (meleeEnemy.pos.y + meleeEnemy.standRightRect.height) / blockSize; i++)
        for (int j = meleeEnemy.pos.x / blockSize; j < (meleeEnemy.pos.x + meleeEnemy.standRightRect.width) / blockSize; j++)
        {
            if (solidPtr[i][j] == '1')
            {
                if ((meleeEnemy.speedX > 0))
                    meleeEnemy.pos.x = j * blockSize - meleeEnemy.standRightRect.width;
                if ((meleeEnemy.speedX < 0))
                    meleeEnemy.pos.x = j * blockSize + blockSize;
                if (meleeEnemy.state == EnemyState::patrol)
                {
                    meleeEnemy.speedX *= -1;
                }
                else
                {
                    meleeEnemy.speedX = 0;
                    meleeEnemy.state = EnemyState::combatStay;
                }
            }
        }
    meleeEnemy.sprite.setPosition(meleeEnemy.pos.x, meleeEnemy.pos.y);
}

void deathMode(Player &edward, float deltaTime, MeleeEnemy &meleeEnemy)
{
    meleeEnemy.sprite.setPosition(meleeEnemy.pos.x - edward.offsetX, meleeEnemy.pos.y - edward.offsetY);
}

void meleeEnemyUpdate(Player &edward, float deltaTime, MeleeEnemy &meleeEnemy, String *mapPtr, float blockSize)
{
    updatesByAnimation(meleeEnemy);
    meleeEnemyAnimation(meleeEnemy);

    collisionEnemy(meleeEnemy, edward, mapPtr, blockSize);
    meleehitCheck(edward, meleeEnemy);
    switch (meleeEnemy.state)
    {
    case patrol:
        patrolMode(edward, deltaTime, meleeEnemy);
        break;
    case combat:
        combatMode(edward, deltaTime, meleeEnemy);
        break;
    case combatStay:
        combatStayMode(edward, deltaTime, meleeEnemy);
        break;
    case death:
        deathMode(edward, deltaTime, meleeEnemy);
        break;
    }
}

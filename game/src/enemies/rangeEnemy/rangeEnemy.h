#include <string>
#include <list>
#include "rangeEnemyConfig.h"

using namespace sf;
using namespace std;

class RangeEnemy
{
  public:
    RangeEnemy(map<String, Texture> &rangeEnemyTextures)
    {
        standTexture = &rangeEnemyTextures["stand"];
        attackTexture = &rangeEnemyTextures["attack"];
        fireBallTexture = &rangeEnemyTextures["fireball"];
        deathTexture = &rangeEnemyTextures["death"];
    }

    Vector2f pos;
    Vector2f fireBallPos;
    Sprite sprite;
    Sprite spellSprite;

    Texture *standTexture;
    Texture *attackTexture;
    Texture *fireBallTexture;
    Texture *deathTexture;

    IntRect standRightRect{RANGE_STAND_RIGHT_SIZE};
    IntRect standLeftRect{RANGE_STAND_LEFT_SIZE};

    IntRect runRightRect{RANGE_RUN_RIGHT_SIZE};
    IntRect runLeftRect{RANGE_RUN_LEFT_SIZE};

    IntRect attackRightRect{RANGE_ATTACK_RIGHT_SIZE};
    IntRect attackLeftRect{RANGE_ATTACK_LEFT_SIZE};

    IntRect deathRightRect{RANGE_DEATH_RIGHT_SIZE};
    IntRect deathLeftRect{RANGE_DEATH_LEFT_SIZE};

    IntRect fireBallRightRect{FIREBALL_RIGHT_SIZE};
    IntRect fireBallLeftRect{FIREBALL_LEFT_SIZE};

    EnemyState state = EnemyState::patrol;

    LineOfSight sight = LineOfSight::right;
    LineOfSight posRelative;

    Clock ripTime;
    Clock rangeEnemyTime;

    Clock fireBallAnimationTime;
    Clock fireBallFlyingTime;

    Vector2f fireBallDirection;

    bool alive = true;
    bool attackZone = false;
    bool fireBallFlying = false;
    bool deathAnimation = false;

    float fireBallAngle = 0;

    float speedX, speedY;
    float offsetX = 0, offsetY = 0;
    float time = rangeEnemyTime.restart().asSeconds();
    int serialNumber;

    void animation(Texture *&texture, IntRect &rect, int FrameGap, int minFrame, int maxFrame, float interval)
    {
        sprite.setTexture((*texture));
        sprite.setTextureRect(rect);
        if (rangeEnemyTime.getElapsedTime().asSeconds() > interval)
        {
            if (rect.left == maxFrame)
            {
                rect.left = minFrame;
            }
            else
                rect.left += FrameGap;
            rangeEnemyTime.restart();
        }
    }

    void fireBallFlyingRight()
    {
        spellSprite.setTexture((*fireBallTexture));
        spellSprite.setTextureRect(fireBallRightRect);
        if (fireBallAnimationTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (fireBallRightRect.left == 250)
            {
                fireBallRightRect.left = 0;
            }
            else
                fireBallRightRect.left += 50;
            fireBallAnimationTime.restart();
        }
    }

    void fireBallFlyingLeft()
    {
        spellSprite.setTexture((*fireBallTexture));
        spellSprite.setTextureRect(fireBallLeftRect);
        if (fireBallAnimationTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (fireBallLeftRect.left == 300)
            {
                fireBallLeftRect.left = 50;
            }
            else
                fireBallLeftRect.left += 50;
            fireBallAnimationTime.restart();
        }
    }

    void deathRight()
    {
        sprite.setTexture((*deathTexture));
        sprite.setTextureRect(deathRightRect);
        if (rangeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (deathRightRect.left == 300)
            {
                deathRightRect.left += 0;
            }
            else
                deathRightRect.left += 100;
            fireBallAnimationTime.restart();
        }
    }

    void deathLeft()
    {
        sprite.setTexture((*deathTexture));
        sprite.setTextureRect(deathLeftRect);
        if (rangeEnemyTime.getElapsedTime().asSeconds() > 0.1f)
        {
            if (deathLeftRect.left == 400)
            {
                deathLeftRect.left += 0;
            }
            else
                deathLeftRect.left += 100;
            rangeEnemyTime.restart();
        }
    }
};

map<String, Texture> getRangeEnemyTextures()
{
    map<String, Texture> rangeEnemyTextures;
    Texture stand;
    stand.loadFromFile("game/sprites/Enemies/range-enemy-stand.png");

    Texture attack;
    attack.loadFromFile("game/sprites/Enemies/range-enemy-attack.png");

    Texture fireball;
    fireball.loadFromFile("game/sprites/Enemies/fireBall.png");

    Texture death;
    death.loadFromFile("game/sprites/Enemies/range-enemy-dead.png");

    rangeEnemyTextures.emplace("stand", stand);
    rangeEnemyTextures.emplace("attack", attack);
    rangeEnemyTextures.emplace("fireball", fireball);
    rangeEnemyTextures.emplace("death", death);

    return rangeEnemyTextures;
}

bool isRangeEnemyOnScreen(Player edward, RangeEnemy &rangeEnemy)
{
    if (((rangeEnemy.pos.x < edward.pos.x + 700) && ((rangeEnemy.pos.x > edward.pos.x - 700)) && (rangeEnemy.alive)) &&
            ((rangeEnemy.pos.y - edward.pos.y > -400) && (rangeEnemy.pos.y - edward.pos.y < 400)) ||
        (rangeEnemy.fireBallFlying))
    {
        return true;
    }
    else
    {
        rangeEnemy.fireBallFlying = false;
        return false;
    }
}

void updatesByAnimation(RangeEnemy &rangeEnemy)
{
    if ((rangeEnemy.deathRightRect.left != 300) && (rangeEnemy.deathLeftRect.left != 400))
    {
        rangeEnemy.ripTime.restart().asSeconds();
    }

    if (rangeEnemy.ripTime.getElapsedTime().asSeconds() > 5)
    {
        rangeEnemy.alive = false;
    }
}

LineOfSight getRangeEnemySightDir(Player edward, RangeEnemy &rangeEnemy)
{
    if (rangeEnemy.pos.x - edward.pos.x > 0)
    {
        return LineOfSight::left;
    }
    else
    {
        return LineOfSight::right;
    }
}

bool getAttackZoneState(Player edward, RangeEnemy &rangeEnemy)
{
    if (((rangeEnemy.pos.x < edward.pos.x + 500) && (rangeEnemy.pos.x > edward.pos.x - 500)) && (rangeEnemy.alive))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void rangeEnemyAnimation(Player edward, float deltaTime, RangeEnemy &rangeEnemy)
{

    if (rangeEnemy.deathAnimation)
    {
        if (rangeEnemy.sight = LineOfSight::right)
            rangeEnemy.deathRight();
        else
            rangeEnemy.deathLeft();
    }
    else
    {
        if (rangeEnemy.attackZone)
        {
            if (rangeEnemy.sight == LineOfSight::right)
            {
                rangeEnemy.animation(rangeEnemy.attackTexture, rangeEnemy.attackRightRect, 70, 0, 420, 0.2);
            }
            else
            {
                rangeEnemy.animation(rangeEnemy.attackTexture, rangeEnemy.attackLeftRect, 70, 70, 490, 0.2);
            }
        }
        else
        {
            if (rangeEnemy.sight == LineOfSight::right)
            {
                rangeEnemy.animation(rangeEnemy.standTexture, rangeEnemy.standRightRect, 70, 0, 770, 0.1);
            }
            else
            {
                rangeEnemy.animation(rangeEnemy.standTexture, rangeEnemy.standLeftRect, 70, 70, 840, 0.1);
            }
        }
    }
}

float getFireBallAngle(Player edward, RangeEnemy &rangeEnemy)
{
    Vector2f deltaPos = edward.pos - rangeEnemy.pos;
    float radians = atan2(deltaPos.y, deltaPos.x);
    return float(double(radians) * 180.0 / 3.14);
}

Vector2f getFireBallDirection(Player edward, RangeEnemy &rangeEnemy)
{
    Vector2f deltaPos = edward.pos - rangeEnemy.pos;
    Vector2f direction;
    direction.x = deltaPos.x / sqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2));
    direction.y = deltaPos.y / sqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2));
    return direction;
}

bool fireBallHitCheck(Player &edward, RangeEnemy &rangeEnemy)
{
    if (edward.sprite.getGlobalBounds().intersects(rangeEnemy.spellSprite.getGlobalBounds()))
    {
        edwardLoseHisHealth(edward);
        if (edward.getDamageTimer.getElapsedTime().asSeconds() > 0.5)
        {
            edward.isEdwardGetDamage = true;
            edward.getDamageTimer.restart();
        }

        return true;
    }
    else
    {
        return false;
    }
}

void fireballDataUpdate(Player edward, RangeEnemy &rangeEnemy)
{
    rangeEnemy.fireBallPos = rangeEnemy.pos;
    rangeEnemy.posRelative = rangeEnemy.sight;
    rangeEnemy.fireBallAngle = getFireBallAngle(edward, rangeEnemy);
    rangeEnemy.fireBallDirection = getFireBallDirection(edward, rangeEnemy);
    rangeEnemy.spellSprite.setRotation(rangeEnemy.fireBallAngle);
    rangeEnemy.fireBallFlying = true;
}

void fireBallMovingUpdate(Player &edward, float deltaTime, RangeEnemy &rangeEnemy, String *mapPtr)
{
    if (rangeEnemy.posRelative == LineOfSight::left)
    {
        rangeEnemy.fireBallFlyingLeft();
    }
    else
    {
        rangeEnemy.fireBallFlyingRight();
    }

    rangeEnemy.fireBallPos.x += rangeEnemy.fireBallDirection.x * deltaTime * 0.3;
    rangeEnemy.fireBallPos.y += rangeEnemy.fireBallDirection.y * deltaTime * 0.3;

    if ((rangeEnemy.fireBallFlyingTime.getElapsedTime().asSeconds() > 3) || (fireBallHitCheck(edward, rangeEnemy)))
    {
        rangeEnemy.fireBallFlyingTime.restart().asSeconds();
        rangeEnemy.fireBallFlying = false;
    }

    rangeEnemy.spellSprite.setPosition(rangeEnemy.fireBallPos.x - edward.offsetX, rangeEnemy.fireBallPos.y - edward.offsetY);
}

void rangehitCheck(Player &edward, RangeEnemy &rangeEnemy)
{
    /*PUNCH */
    if ((rangeEnemy.pos.y - edward.pos.y > -40) &&
        (rangeEnemy.pos.y - edward.pos.y < 40))
    {
        if ((edward.pos.x - rangeEnemy.pos.x < 100) &&
            (edward.pos.x - rangeEnemy.pos.x > 10) && (edward.sight == LineOfSight::left) && (edward.hitConfirm))
        {
            edward.isEdwardHitEnemy = true;
            edward.enemyWhoDie = rangeEnemy.sprite.getPosition();
            rangeEnemy.deathAnimation = true;
            edward.hitConfirm = false;
        }

        else if ((edward.pos.x - rangeEnemy.pos.x < 10) &&
                 (edward.pos.x - rangeEnemy.pos.x > -100) && (edward.sight == LineOfSight::right) && (edward.hitConfirm))
        {
            edward.isEdwardHitEnemy = true;
            edward.enemyWhoDie = rangeEnemy.sprite.getPosition();
            rangeEnemy.deathAnimation = true;
            edward.hitConfirm = false;
        }
    }

    /*SHELL */
    if (rangeEnemy.sprite.getGlobalBounds().intersects(edward.shell.getGlobalBounds()))
    {
        edward.isShellFlying = false;
        edward.isEdwardHitEnemy = true;
        edward.enemyWhoDie = rangeEnemy.sprite.getPosition();
        rangeEnemy.alive = false;
    }
}

void rangeEnemyUpdate(Player &edward, float deltaTime, RangeEnemy &rangeEnemy, String *mapPtr)
{
    updatesByAnimation(rangeEnemy);
    rangeEnemy.sight = getRangeEnemySightDir(edward, rangeEnemy);
    rangeEnemy.attackZone = getAttackZoneState(edward, rangeEnemy);
    rangehitCheck(edward, rangeEnemy);

    if (rangeEnemy.attackZone && !rangeEnemy.fireBallFlying)
    {
        if (rangeEnemy.attackRightRect.left == 140 || rangeEnemy.attackLeftRect.left == 140)
        {
            fireballDataUpdate(edward, rangeEnemy);
        }
    }

    rangeEnemy.sprite.setPosition(rangeEnemy.pos.x - edward.offsetX, rangeEnemy.pos.y - edward.offsetY);
    rangeEnemyAnimation(edward, deltaTime, rangeEnemy);

    if (rangeEnemy.fireBallFlying)
    {
        fireBallMovingUpdate(edward, deltaTime, rangeEnemy, mapPtr);
    }
}

void drawRangeEnemy(RenderWindow &window, vector<RangeEnemy> &rangeEnemies, Player edward)
{
    for (int i = 0; i < rangeEnemies.size(); i++)
    {
        if (isRangeEnemyOnScreen(edward, rangeEnemies[i]))
        {
            window.draw(rangeEnemies[i].sprite);
        }

        if (rangeEnemies[i].fireBallFlying)
        {
            window.draw(rangeEnemies[i].spellSprite);
        }
    }
}

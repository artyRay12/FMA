#include <iostream>
#include <SFML/Graphics.hpp>
#include "bossConfig.h"

using namespace sf;
using namespace std;

enum BossState
{
    none,
    intro,
    stay,
    run,
    phaseChanger,
    fight,
    legAttack,
};

class Boss
{
  public:
    Boss(map<String, Texture> &bossTextures)
    {
        introTexture = &bossTextures["intro"];
        runTexture = &bossTextures["run"];
        legAttackTexture = &bossTextures["legAttack"];
        standTexture = &bossTextures["stay"];
        healthBarTexture = &bossTextures["healthBar"];
        phaseTexture = &bossTextures["phase"];
        healthBar.setTexture(*healthBarTexture);
        sprite.setOrigin(BOSS_HEALTH_BAR_ORIGIN);
        healthBar.setPosition(BOSS_HEALTH_BAR_POS);
        healthLine.setSize({BOSS_HEALTH_LINE_HEIGHT, BOSS_INITIAL_HEALTH});
        healthLine.setPosition(BOSS_HEALTH_LINE_START_POS);
        healthLine.setFillColor(Color::Red);
        healthLine.setRotation(BOSS_HEALTH_LINE_ROTATION);
    };

    Vector2f pos = BOSS_START_POS;
    Sprite sprite;
    Sprite healthBar;

    RectangleShape healthLine;

    Texture *introTexture;
    Texture *standTexture;
    Texture *runTexture;
    Texture *legAttackTexture;
    Texture *attackTexture;
    Texture *deathTexture;
    Texture *healthBarTexture;
    Texture *phaseTexture;

    View view;

    float speedX = 0;
    float health = BOSS_INITIAL_HEALTH;
    int zoomCounter = 0;
    int phase = 0;

    bool hitConfirm = false;
    bool isBossActive = false;
    bool alive;
    bool isPhase = false;
    bool isZoomed = false;

    BossState state = BossState::none;

    IntRect introRect{BOSS_INTRO_SRPITE_SIZE};

    IntRect standRightRect{BOSS_STAND_RIGHT_SIZE};
    IntRect standLeftRect{BOSS_STAND_LEFT_SIZE};

    IntRect runRightRect{BOSS_RUN_RIGHT_SIZE};
    IntRect runLeftRect{BOSS_RUN_LEFT_SIZE};

    IntRect legAttackRightRect{BOSS_LEGATTACK_RIGHT_SIZE};
    IntRect legAttackLeftRect{BOSS_LEGATTACK_LEFT_SIZE};

    IntRect phaseRect{BOSS_PHASE_CHANGE_SIZE};

    LineOfSight sight = LineOfSight::right;
    LineOfSight posRelative;

    Clock bossTime;
    Clock hpLoseTimer;
    Clock zoomTimer;

    float time = bossTime.restart().asSeconds();

    void animation(Texture *&texture, IntRect &rect, int FrameGap, int minFrame, int maxFrame, float Interval)
    {
        sprite.setTexture((*texture));
        sprite.setTextureRect(rect);
        if (bossTime.getElapsedTime().asSeconds() > Interval)
        {
            if (rect.left == maxFrame)
            {
                rect.left = minFrame;
            }
            else
                rect.left += FrameGap;
            bossTime.restart();
        }
    }
};

void bossInit(Boss &boss)
{
    Vector2f pos = BOSS_START_POS;
    boss.sprite.setPosition(boss.pos);
    boss.isBossActive = false;
    boss.alive = true;
    boss.health = 420;
    boss.phase = 0;
    boss.state = BossState::none;
    boss.view.reset(sf::FloatRect(0, 0, 1440, 900));
}

void updateByAnimations(Boss &boss)
{
    if ((boss.introRect.left == 1680))
    {
        boss.view.reset(sf::FloatRect(0, 0, 1440, 900));
        boss.state = BossState::run;
        boss.introRect.left = 0;
    }

    if ((boss.view.getRotation() > 182) && (boss.phase == 0))
    {
        boss.state = BossState::run;
        boss.phaseRect.left = 0;
        boss.view.setRotation(180);
        boss.health--;
        boss.view.move(0, -200);
        boss.phase = 1;
    }

    if ((boss.view.getRotation() < 1) && (boss.phase == 1))
    {
        boss.state = BossState::run;
        boss.health--;
        boss.view.reset(sf::FloatRect(0, 0, 1440, 900));
        boss.phase = 2;
    }

    if (boss.sight == LineOfSight::right)
    {
        if (boss.legAttackRightRect.left >= 1600)
        {
            boss.hitConfirm = true;
        }
        else
        {
            boss.hitConfirm = false;
        }
    }
    else
    {
        if (boss.legAttackLeftRect.left >= 1400)
        {
            boss.hitConfirm = true;
        }
        else
        {
            boss.hitConfirm = false;
        }
    }
}

void bossAnimation(Boss &boss)
{

    if (boss.state == BossState::intro)
    {
        boss.animation(boss.introTexture, boss.introRect, 120, 1320, 1680, 0.2f);
    }
    if (boss.speedX != 0)
    {
        if (boss.sight == LineOfSight::right)
            boss.animation(boss.runTexture, boss.runRightRect, 120, 0, 720, 0.1f);
        if (boss.sight == LineOfSight::left)
            boss.animation(boss.runTexture, boss.runLeftRect, 120, 120, 840, 0.1f);
    }

    if (boss.state == BossState::phaseChanger)
    {
        boss.animation(boss.phaseTexture, boss.phaseRect, 180, 1620, 1980, 0.2f);
    }

    if (boss.state == BossState::fight)
    {
        if (boss.sight == LineOfSight::right)
            //boss.legAttackRight();
            boss.animation(boss.legAttackTexture, boss.legAttackRightRect, 200, 0, 2800, 0.08f);
        if (boss.sight == LineOfSight::left)
            //    boss.legAttackLeft();
            boss.animation(boss.legAttackTexture, boss.legAttackLeftRect, 200, 200, 3000, 0.08f);
    }

    if (boss.state == BossState::stay)
    {
        if (boss.sight == LineOfSight::right)
            boss.animation(boss.standTexture, boss.standRightRect, 90, 0, 180, 0.2f);
        if (boss.sight == LineOfSight::left)
            boss.animation(boss.standTexture, boss.standLeftRect, 90, 90, 270, 0.2f);
    }
}

map<String, Texture> getBossTextures()
{
    map<String, Texture> bossTextures;

    Texture introTexture;
    introTexture.loadFromFile("game/sprites/Enemies/boss-intro.png");

    Texture runTexture;
    runTexture.loadFromFile("game/sprites/Enemies/boss-run.png");

    Texture legAttackTexture;
    legAttackTexture.loadFromFile("game/sprites/Enemies/boss-leg-attack.png");

    Texture stayTexture;
    stayTexture.loadFromFile("game/sprites/Enemies/boss-stay.png");

    Texture bossHealthBar;
    bossHealthBar.loadFromFile("game/sprites/Enemies/boss-health-bar.png");

    Texture phaseTexture;
    phaseTexture.loadFromFile("game/sprites/Enemies/boss-phase.png");

    bossTextures.emplace("intro", introTexture);
    bossTextures.emplace("run", runTexture);
    bossTextures.emplace("legAttack", legAttackTexture);
    bossTextures.emplace("stay", stayTexture);
    bossTextures.emplace("healthBar", bossHealthBar);
    bossTextures.emplace("phase", phaseTexture);

    return bossTextures;
}

bool isPlayerNear(Player edward, Boss &boss)
{
    if (((boss.pos.x - edward.pos.x > -500) && (boss.pos.x - edward.pos.x < 500)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

LineOfSight getRelativeSide(Player edward, Boss &boss)
{
    if (edward.pos.x - boss.pos.x > 0)
    {
        return LineOfSight::right;
    }
    else
    {
        return LineOfSight::left;
    }
}

bool isPlayerInAttackZone(Player edward, Boss &boss)
{
    if (((boss.pos.x - edward.pos.x > -40) &&
         (boss.pos.x - edward.pos.x < 0) &&
         (boss.sight == LineOfSight::right)) ||
        ((boss.pos.x - edward.pos.x < 80) &&
         (boss.pos.x - edward.pos.x > 0) &&
         (boss.sight == LineOfSight::left)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void bossPositionUpdate(Boss &boss, Player edward, float deltaTime)
{
    if (boss.state == BossState::none)
        boss.speedX = 0;
    if (boss.posRelative == LineOfSight::right)
    {
        boss.sight = LineOfSight::right;
        boss.pos.x += boss.speedX * deltaTime;
    }
    else
    {
        boss.sight = LineOfSight::left;
        boss.pos.x -= boss.speedX * deltaTime;
    }

    if (boss.state == BossState::intro)
    {
        boss.sprite.setPosition(boss.pos.x - edward.offsetX, boss.pos.y - edward.offsetY + 100);
    }
    else
    {
        boss.sprite.setPosition(boss.pos.x - edward.offsetX, boss.pos.y - edward.offsetY);
    }
}

void bossLoseHisHealth(Boss &boss)
{
    if ((boss.hpLoseTimer.getElapsedTime().asSeconds() > 0.6))
    {
        boss.health -= 30;
        boss.healthLine.setSize({20, boss.health});
        boss.hpLoseTimer.restart();
    }
}

void animationRefresh(Boss &boss)
{
    boss.legAttackLeftRect.left = 200;
    boss.legAttackRightRect.left = 0;
}

void zoomScene(Boss &boss, Player edward, float deltaTime)
{
    boss.zoomTimer.restart();
    if (!boss.isZoomed)
    {
        if (boss.zoomTimer.getElapsedTime().asMicroseconds() > 0.2f)
        {
            boss.view.zoom(0.93f);
            boss.zoomCounter++;
            if (boss.zoomCounter > 10)
                boss.isZoomed = true;
        }
    }
    boss.view.setCenter(boss.pos.x - edward.offsetX, boss.pos.y - edward.offsetY + 40);
}

void phaseChangerMode(Boss &boss, Player edward, float deltaTime)
{
    boss.speedX = 0;
    switch (boss.phase)
    {
    case 0:
        zoomScene(boss, edward, deltaTime);
        boss.sprite.setOrigin(100, 150);
        boss.view.rotate(0.05 * deltaTime);
        break;
    case 1:
        zoomScene(boss, edward, deltaTime);
        boss.sprite.setOrigin(100, 150);
        boss.view.rotate(0.05 * deltaTime);
        break;
    }
}

void introMode(Boss &boss)
{
    boss.sprite.setPosition(boss.pos.x, boss.pos.y + 400);
}

void runMode(Boss &boss, Player edward, float deltaTime)
{
    boss.sprite.setOrigin(100, 0);
    animationRefresh(boss);

    boss.speedX = BOSS_SPEEDX;
    if (isPlayerInAttackZone(edward, boss))
    {
        boss.state = BossState::fight;
    }

    if (!isPlayerNear(edward, boss))
    {
        boss.isBossActive = false;
        boss.state = BossState::stay;
    }
    else
    {
        boss.isBossActive = true;
    }
}

void stayMode(Boss &boss, Player edward)
{
    boss.speedX = 0;
    if (isPlayerNear(edward, boss))
    {
        boss.state = BossState::run;
    }
}

void fightMode(Boss &boss, Player edward)
{
    boss.speedX = 0;
    if ((!isPlayerInAttackZone(edward, boss)) && (boss.legAttackLeftRect.left == 200 && boss.legAttackRightRect.left == 0))
    {
        boss.state = BossState::run;
    }
}

void bossHitCheck(Player &edward, Boss &boss)
{
    if ((boss.pos.y - edward.pos.y > -80) &&
        (boss.pos.y - edward.pos.y < 40))
    {
        if ((edward.pos.x - boss.pos.x < 100) &&
            (edward.pos.x - boss.pos.x > 10))
        {
            if ((edward.hitConfirm) && (edward.sight == LineOfSight::left))
            {
                edward.isEdwardHitEnemy = true;
                edward.enemyWhoDie.x = boss.pos.x - edward.offsetX;
                edward.enemyWhoDie.y = boss.pos.y - edward.offsetY;
                edward.hitConfirm = false;
                bossLoseHisHealth(boss);
            }

            if ((boss.hitConfirm))
            {
                edwardLoseHisHealth(edward);
            }
        }

        else if ((edward.pos.x - boss.pos.x < 10) &&
                 (edward.pos.x - boss.pos.x > -100))
        {
            if ((edward.hitConfirm) && (edward.sight == LineOfSight::right))
            {
                edward.isEdwardHitEnemy = true;
                edward.enemyWhoDie.x = boss.pos.x - edward.offsetX;
                edward.enemyWhoDie.y = boss.pos.y - edward.offsetY;
                edward.hitConfirm = false;
                bossLoseHisHealth(boss);
            }

            if (boss.hitConfirm)
            {
                edwardLoseHisHealth(edward);
            }
        }
    }

    /* SHELL */
    if (boss.sprite.getGlobalBounds().intersects(edward.shell.getGlobalBounds()))
    {
        edward.isShellFlying = false;
        edward.isEdwardHitEnemy = true;
        edward.enemyWhoDie.x = boss.pos.x - edward.offsetX;
        edward.enemyWhoDie.y = boss.pos.y - edward.offsetY;
        bossLoseHisHealth(boss);
    }
}

void updateByHealth(Boss &boss)
{
    if (boss.health < 0)
    {
        boss.alive = false;
        boss.view.reset(sf::FloatRect(0, 0, 1440, 900));
    }

    if (boss.health == 330)
    {
        boss.state = BossState::phaseChanger;
    }

    if (boss.health == 209)
    {
        boss.state = BossState::phaseChanger;
    }

    if (boss.health == 118)
    {
        boss.phase = 3;
    }

    if (boss.health < 80)
    {
        boss.phase = 4;
    }
}

void bossUpdate(Player &edward, Boss &boss, float deltaTime)
{

    LineOfSight state = getRelativeSide(edward, boss);
    if (state != boss.posRelative)
    {
        boss.legAttackLeftRect.left = 200;
        boss.legAttackRightRect.left = 0;
        boss.posRelative = state;
    }

    bossAnimation(boss);
    updateByAnimations(boss);
    bossHitCheck(edward, boss);
    updateByHealth(boss);

    if (boss.phase == 3)
    {
        boss.view.rotate(0.1 * deltaTime);
    }

    switch (boss.state)
    {
    case none:
        if (isPlayerNear(edward, boss))
            boss.state = BossState::intro;
        break;
    case intro:
        introMode(boss);
        zoomScene(boss, edward, deltaTime);
        break;
    case run:
        runMode(boss, edward, deltaTime);
        break;
    case fight:
        fightMode(boss, edward);
        break;
    case phaseChanger:
        phaseChangerMode(boss, edward, deltaTime);
        break;
    case stay:
        stayMode(boss, edward);
        break;
    }
    bossPositionUpdate(boss, edward, deltaTime);
}

void bossDraw(RenderWindow &window, Boss &boss)
{

    window.draw(boss.sprite);
    if (boss.isBossActive)
    {
        window.draw(boss.healthLine);
        window.draw(boss.healthBar);
    }
    window.setView(boss.view);
}

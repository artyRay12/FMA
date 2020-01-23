#include <iostream>
#include <SFML/Graphics.hpp>
#include "edwardConfig.h"
using namespace sf;
using namespace std;

enum lineOfSight
{
    right,
    left
};

enum cannonState
{
    uncast,
    casting,
    ready
};

class Player
{
  public:
    Player(map<String, Texture> &playerTextures)
    {
        standTexture = &playerTextures["stand"];
        runTexture = &playerTextures["run"];
        jumpTexture = &playerTextures["jump"];
        jumpAttackTexture = &playerTextures["jumpAttack"];
        circleHitTexture = &playerTextures["circleHit"];
        castCircleTexture = &playerTextures["castCircle"];
        getDamageTexture = &playerTextures["getDamage"];
        cannonTexture = &playerTextures["cannon"];
        shellTexture = &playerTextures["shell"];
        keyHelperTexture = &playerTextures["keyHelper"];
        live1Texture = &playerTextures["live1"];
        live2Texture = &playerTextures["live2"];
        live3Texture = &playerTextures["live3"];
    }

    Vector2f startPos = {EDWARD_START_POS}; // 12800 100
    Vector2f pos;                           //5900 1200
    Vector2f curPos;
    Vector2f shellPos;

    Texture *standTexture;
    Texture *runTexture;
    Texture *jumpTexture;
    Texture *jumpAttackTexture;
    Texture *circleHitTexture;
    Texture *getDamageTexture;
    Texture *castCircleTexture;
    Texture *cannonTexture;
    Texture *shellTexture;
    Texture *keyHelperTexture;
    Texture *live1Texture;
    Texture *live2Texture;
    Texture *live3Texture;

    RectangleShape healthBar;

    IntRect standRightRect{EDWARD_STAND_RIGHT_SIZE};
    IntRect standLeftRect{EDWARD_STAND_LEFT_SIZE};

    IntRect runRightRect{EDWARD_RUN_RIGHT_SIZE};
    IntRect runLeftRect{EDWARD_RUN_LEFT_SIZE};

    IntRect jumpRightRect{EDWARD_JUMP_RIGHT_SIZE};
    IntRect jumpLeftRect{EDWARD_JUMP_LEFT_SIZE};

    IntRect circleHitRightRect{EDWARD_CIRCLE_HIT_RIGHT_SIZE};
    IntRect circleHitLeftRect{EDWARD_CIRCLE_HIT_LEFT_SIZE};

    IntRect jumpAttackRightRect{EDWARD_JUMP_ATTACK_RIGHT_SIZE};
    IntRect jumpAttackLeftRect{EDWARD_JUMP_ATTACK_LEFT_SIZE};

    IntRect castCircleRightRect{EDWARD_MAGIC_CIRCLE_CAST_RIGHT_SIZE};
    IntRect castCircleLeftRect{EDWARD_MAGIC_CIRCLE_CAST_LEFT_SIZE};

    IntRect getDamageRightRect{EDWARD_GET_DAMAGE_RIGHT_SIZE};
    IntRect getDamageLeftRect{EDWARD_GET_DAMAGE_LEFT_SIZE};

    IntRect cannonRigthRect{CANNON_RIGHT_SIZE};
    IntRect cannonLeftRect{CANNON_LEFT_SIZE};

    IntRect shellRect{SHELL_SIZE};

    lineOfSight sight = lineOfSight::right;
    lineOfSight sightSaved = lineOfSight::right;
    cannonState cannonState = cannonState::uncast;

    Sprite sprite;
    Sprite spell;
    Sprite cannon;
    Sprite shell;
    Sprite keyHelper;
    Sprite liveIcon;
    Sprite livesImage;

    Clock clock;
    Clock hpLoseTimer;
    Clock hpGetTimer;
    Clock getDamageTimer;
    Clock keyHaveTimer;
    Clock keyAdvice;

    int offsetX = 0, offsetY = 0;
    int lives = 3;
    Vector2f enemyWhoDie;

    bool meleeAttack = false;
    bool canIJumpAttack = false;
    bool hitConfirm = false;
    bool onGround = false;
    bool canICastCircle = false;
    bool nextLevel = false;
    bool isEdwardGetDamage = false;
    bool isEdwardHitEnemy = false;
    bool isCannonCasting = false;
    bool isCannonReady = false;
    bool isShellFlying = false;
    bool isKey = false;
    bool isDoorOpen = false;
    bool showKeyAdvice = false;

    float speedX, speedY;
    float time = clock.restart().asSeconds();
    float jumpHeight;
    float health;

    void animation(Texture *&texture, IntRect &rect, int FrameGap, int minFrame, int maxFrame, float interval)
    {
        sprite.setTexture((*texture));
        sprite.setTextureRect(rect);
        if (clock.getElapsedTime().asSeconds() > interval)
        {
            if (rect.left == maxFrame)
            {
                rect.left = minFrame;
            }
            else
                rect.left += FrameGap;
            clock.restart();
        }
    }

    void jumpLeft()
    {
        sprite.setTexture((*jumpTexture));
        sprite.setTextureRect(jumpLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.15f)
        {
            if (jumpLeftRect.left == 350)
            {
                jumpRightRect.left = 280;
                jumpLeftRect.left = 350;
                meleeAttack = false;
            }
            else
            {
                jumpRightRect.left += 70;
                jumpLeftRect.left += 70;
            }
            clock.restart();
        }
    }

    void jumpRight()
    {
        sprite.setTexture((*jumpTexture));
        sprite.setTextureRect(jumpRightRect);
        if (clock.getElapsedTime().asSeconds() > 0.15f)
        {
            if (jumpRightRect.left == 280)
            {
                jumpRightRect.left = 280;
                jumpLeftRect.left = 350;
                meleeAttack = false;
            }
            else
            {
                jumpLeftRect.left += 70;
                jumpRightRect.left += 70;
            }
            clock.restart();
        }
    }

    void circleHitRight()
    {
        sprite.setTexture((*circleHitTexture));
        sprite.setTextureRect(circleHitRightRect);
        if (clock.getElapsedTime().asSeconds() > 0.03f)
        {
            if (circleHitRightRect.left == 1820)
            {
                meleeAttack = false;
                circleHitRightRect.left = 0;
                hitConfirm = false;
                sprite.setScale(1, 1);
            }
            else
                circleHitRightRect.left += 130;
            if (circleHitRightRect.left > 900)
            {
                hitConfirm = true;
            }
            clock.restart();
        }
    }

    void circleHitLeft()
    {
        sprite.setTexture((*circleHitTexture));
        sprite.setTextureRect(circleHitLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.03f)
        {
            if (circleHitLeftRect.left == 1950)
            {
                meleeAttack = false;
                circleHitLeftRect.left = 130;
                hitConfirm = false;
            }
            else
                circleHitLeftRect.left += 130;
            if (circleHitLeftRect.left > 900)
            {
                hitConfirm = true;
            }
            clock.restart();
        }
    }

    void jumpAttackRight()
    {
        sprite.setTexture((*jumpAttackTexture));
        sprite.setTextureRect(jumpAttackRightRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (jumpAttackRightRect.left == 340)
            {
                canIJumpAttack = false;
                hitConfirm = false;
                jumpAttackRightRect.left = 0;
            }
            else
                jumpAttackRightRect.left += 85;
            if (jumpAttackRightRect.left == 255)
            {
                hitConfirm = true;
            }
            clock.restart();
        }
    }

    void jumpAttackLeft()
    {
        sprite.setTexture((*jumpAttackTexture));
        sprite.setTextureRect(jumpAttackLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (jumpAttackLeftRect.left == 425)
            {
                canIJumpAttack = false;
                hitConfirm = false;
                jumpAttackLeftRect.left = 85;
            }
            else
                jumpAttackLeftRect.left += 85;
            if (jumpAttackLeftRect.left == 255)
            {
                hitConfirm = true;
            }
            clock.restart();
        }
    }

    void castCircleRight()
    {
        sprite.setTexture((*castCircleTexture));
        sprite.setTextureRect(castCircleRightRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (castCircleRightRect.left == 810)
            {
                canICastCircle = false;
                castCircleRightRect.left = 0;
            }
            else
                castCircleRightRect.left += 90;
            clock.restart();
        }
    }

    void castCircleLeft()
    {
        sprite.setTexture((*castCircleTexture));
        sprite.setTextureRect(castCircleLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (castCircleLeftRect.left == 900)
            {
                canICastCircle = false;
                castCircleLeftRect.left = 90;
            }
            else
                castCircleLeftRect.left += 90;
            clock.restart();
        }
    }

    void getDamageRight()
    {
        sprite.setTexture((*getDamageTexture));
        sprite.setTextureRect(getDamageRightRect);
        if (clock.getElapsedTime().asSeconds() > 0.1f)
        {
            if (getDamageRightRect.left == 160)
            {
                isEdwardGetDamage = false;
                getDamageRightRect.left = 0;
            }
            else
            {
                getDamageRightRect.left += 80;
            }
            clock.restart();
        }
    }

    void getDamageLeft()
    {
        sprite.setTexture((*getDamageTexture));
        sprite.setTextureRect(getDamageLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.10f)
        {
            if (getDamageLeftRect.left == 240)
            {
                isEdwardGetDamage = false;
                getDamageLeftRect.left = 80;
            }
            else
            {
                getDamageLeftRect.left += 80;
            }
            clock.restart();
        }
    }

    void cannonShootingRight()
    {
        cannon.setTexture((*cannonTexture));
        cannon.setTextureRect(cannonRigthRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (cannonRigthRect.left == 840)
            {
                cannonState = cannonState::uncast;
                cannonRigthRect.left = 0;
            }
            else
            {
                cannonRigthRect.left += 140;
            }
            clock.restart();
        }
    }

    void cannonShootingLeft()
    {
        cannon.setTexture((*cannonTexture));
        cannon.setTextureRect(cannonLeftRect);
        if (clock.getElapsedTime().asSeconds() > 0.07f)
        {
            if (cannonLeftRect.left == 980)
            {
                cannonState = cannonState::uncast;
                cannonLeftRect.left = 140;
            }
            else
            {
                cannonLeftRect.left += 140;
            }
            clock.restart();
        }
    }

    void shellFlying()
    {
        shell.setTexture((*shellTexture));
        shell.setTextureRect(shellRect);
    }
};

void edwardInit(Player &edward)
{
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
    edward.livesImage.setPosition(EDWARD_LIVES_COUNTER_POS);

    edward.cannon.setOrigin(CANNON_ORIGIN);
    edward.speedX = 0;
    edward.speedY = 0;
    edward.offsetX = 0;
    edward.offsetY = 0;
    edward.health = EDWARD_INITIAL_HEALTH;
    edward.isEdwardGetDamage = false;
    edward.sprite.setPosition(edward.startPos);
    edward.pos = edward.sprite.getPosition();
    edward.healthBar.setSize({15, edward.health});
    edward.healthBar.setPosition({EDWARD_HEALTH_BAR_POS});
    edward.healthBar.setFillColor(Color::Green);
    edward.healthBar.setRotation(EDWARD_HEALTH_BAR_ROTATE);
    edward.keyHelper.setTexture((*edward.keyHelperTexture));
}

map<String, Texture> getPlayerTextures()
{
    map<String, Texture> playerTextures;
    Texture stand;
    stand.loadFromFile("game/sprites/Edward/edward-stand.png");

    Texture run;
    run.loadFromFile("game/sprites/Edward/edward-run.png");

    Texture jump;
    jump.loadFromFile("game/sprites/Edward/edward-jump.png");

    Texture jumpAttack;
    jumpAttack.loadFromFile("game/sprites/Edward/edward-jumpAttack.png");

    Texture circleHit;
    circleHit.loadFromFile("game/sprites/Edward/edward-circle-hit.png");

    Texture castCircle;
    castCircle.loadFromFile("game/sprites/Edward/edward-cast-circle.png");

    Texture getDamage;
    getDamage.loadFromFile("game/sprites/Edward/edward-get-damage.png");

    Texture cannon;
    cannon.loadFromFile("game/sprites/Edward/cannon.png");

    Texture shell;
    shell.loadFromFile("game/sprites/Edward/shell.png");

    Texture keyHelper;
    keyHelper.loadFromFile("game/sprites/Edward/key-Helper.png");

    Texture live1;
    live1.loadFromFile("game/sprites/Interface/lives-1.png");

    Texture live2;
    live2.loadFromFile("game/sprites/Interface/lives-2.png");

    Texture live3;
    live3.loadFromFile("game/sprites/Interface/lives-3.png");

    playerTextures.emplace("stand", stand);
    playerTextures.emplace("run", run);
    playerTextures.emplace("jump", jump);
    playerTextures.emplace("jumpAttack", jumpAttack);
    playerTextures.emplace("circleHit", circleHit);
    playerTextures.emplace("castCircle", castCircle);
    playerTextures.emplace("getDamage", getDamage);
    playerTextures.emplace("cannon", cannon);
    playerTextures.emplace("shell", shell);
    playerTextures.emplace("keyHelper", keyHelper);
    playerTextures.emplace("live1", live1);
    playerTextures.emplace("live2", live2);
    playerTextures.emplace("live3", live3);

    return playerTextures;
}

void edwardLoseHisHealth(Player &edward)
{
    if ((edward.hpLoseTimer.getElapsedTime().asSeconds() > 0.6) && (edward.health > 0))
    {
        edward.health -= 20;
        edward.healthBar.setSize({15, edward.health});
        edward.hpLoseTimer.restart();
    }
}

void edwardGetHisHealth(Player &edward)
{
    if ((edward.hpGetTimer.getElapsedTime().asSeconds() > 1) && edward.health != 120)
    {
        edward.health += 20;
        edward.healthBar.setSize({15, edward.health});
        edward.hpLoseTimer.restart();
    }
}

void edwardAnimations(Player &edward)
{
    if (edward.isShellFlying)
    {
        edward.shellFlying();
    }

    if (edward.cannonState == cannonState::ready)
    {
        if (edward.sight == lineOfSight::right)
        {
            edward.cannon.setScale(1, 1);
            edward.cannonShootingRight();
        }
        else
        {
            edward.cannon.setScale(-1, 1);
            edward.cannonShootingRight();
        }
    }
    if (edward.speedY != 0)
    {
        if (edward.canIJumpAttack == true)
        {
            if (edward.sight == lineOfSight::right)
            {
                edward.jumpAttackRight();
            }
            else
            {
                edward.jumpAttackLeft();
            }
        }
        else
        {
            if (edward.sight == lineOfSight::right)
            {
                edward.jumpRight();
            }
            else
            {
                edward.jumpLeft();
            }
        }
    }
    else
    {
        if (edward.meleeAttack == true)
        {
            if (edward.sight == lineOfSight::right)
            {
                edward.circleHitRight();
            }
            else
            {
                edward.sprite.setOrigin(25, 0);
                edward.sprite.setScale(-1, 1);
                edward.circleHitRight();
            }
        }
        else
        {
            if (edward.speedX > 0)
            {
                edward.animation(edward.runTexture, edward.runRightRect, 90, 0, 450, 0.13);
                //edward.runRight();
            }
            else
            {
                if (edward.speedX < 0)
                {
                    edward.animation(edward.runTexture, edward.runLeftRect, 90, 90, 540, 0.13);
                    //edward.runLeft();
                }
                else
                {
                    if (!edward.canICastCircle)
                    {
                        if (edward.isEdwardGetDamage)
                        {
                            if (edward.sight == lineOfSight::right)
                            {
                                edward.getDamageLeft();
                            }
                            else
                            {

                                edward.getDamageRight();
                            }
                        }
                        else
                        {

                            if (edward.sight == lineOfSight::right)
                            {
                                edward.animation(edward.standTexture, edward.standRightRect, 50, 0, 250, 0.20);
                                // edward.standRight();
                            }
                            else
                            {
                                edward.animation(edward.standTexture, edward.standLeftRect, 50, 50, 300, 0.20);
                                //edward.standLeft();
                            }
                        }
                    }
                    else
                    {
                        if (edward.sight == lineOfSight::right)
                        {
                            edward.castCircleRight();
                        }
                        else
                        {
                            edward.castCircleLeft();
                        }
                    }
                }
            }
        }
    }
}

void updatesByMove(Player &edward)
{
    if ((edward.speedX != 0) || (edward.speedY != 0))
    {
        edward.getDamageLeftRect.left = 0;
        edward.getDamageRightRect.left = 0;

        edward.castCircleLeftRect.left = 90;
        edward.castCircleRightRect.left = 0;

        edward.cannonLeftRect.left = 140;
        edward.cannonRigthRect.left = 0;

        edward.canICastCircle = false;

        edward.cannonState = cannonState::uncast;
    }

    if (edward.speedY == 0)
    {
        edward.canIJumpAttack = false;
    }

    if (edward.isDoorOpen)
    {
        if (edward.keyHaveTimer.getElapsedTime().asSeconds() > 3)
        {
            edward.isDoorOpen = false;
            edward.isKey = false;
        }
    }
}

void edwardPressRightUpdates(Player &edward)
{
    edward.speedX = 0.3; //0.3;
    edward.sight = lineOfSight::right;
    edward.isEdwardGetDamage = false;
}

void edwardPressLeftUpdates(Player &edward)
{
    edward.speedX = -0.3; //-0.3;
    edward.sight = lineOfSight::left;
    edward.isEdwardGetDamage = false;
}

void edwardPressUpUpdates(Player &edward)
{
    edward.canICastCircle = false;
    if (edward.onGround)
    {
        edward.castCircleRightRect.left = 0;
        edward.castCircleLeftRect.left = 90;
        edward.speedY = -0.4; // -0.38
        edward.onGround = false;
        edward.meleeAttack = false;
        edward.jumpRightRect.left = 0;
        edward.jumpLeftRect.left = 70;
        edward.circleHitRightRect.left = 0;
        edward.circleHitLeftRect.left = 130;
        edward.isEdwardGetDamage = false;
    }
}

void edwardPressSpaceUpdates(Player &edward)
{
    edward.castCircleRightRect.left = 0;
    edward.isEdwardGetDamage = false;
    if (edward.speedY != 0)
    {
        edward.canIJumpAttack = true;
    }
    else
    {
        edward.meleeAttack = true;
    }
}

void edwardPressOneUpdates(Player &edward)
{
    edward.canICastCircle = true;
    edward.isEdwardGetDamage = false;
    edward.cannonState = cannonState::casting;
}

void collision(int dir, Player &edward, String *&solidPtr, float blockSize, int &currentLevel)
{
    for (int i = edward.pos.y / blockSize; i < (edward.pos.y + edward.standRightRect.height) / blockSize; i++)
        for (int j = edward.pos.x / blockSize; j < (edward.pos.x + edward.standRightRect.width - 10) / blockSize; j++)
        {
            if ((solidPtr[i][j] == 'B') || (solidPtr[i][j] == 'P') || (solidPtr[i][j] == 's') || (solidPtr[i][j] == 'S'))
            {
                if ((edward.speedX > 0) && (dir == 0))
                    edward.pos.x = j * blockSize - edward.standRightRect.width;
                if ((edward.speedX < 0) && (dir == 0))
                    edward.pos.x = j * blockSize + blockSize;

                if ((edward.speedY > 0) && (dir == 1))
                {
                    edward.pos.y = i * blockSize - edward.standRightRect.height;
                    edward.speedY = 0;
                    edward.onGround = true;
                }
                if ((edward.speedY < 0) && (dir == 1))
                {
                    edward.pos.y = i * blockSize + blockSize;
                    edward.speedY = 0;
                }
            }

            if (solidPtr[i][j] == 'H')
            {
                solidPtr[i][j] = ' ';
                edwardGetHisHealth(edward);
            }

            if (solidPtr[i][j] == 'K')
            {
                edward.isKey = true;
                solidPtr[i][j] = ' ';
            }

            if (((solidPtr[i][j] == 's') || (solidPtr[i][j] == 'S')) && (edward.isKey) && (edward.speedY == 0))
            {
                if (solidPtr[i][j] == 's')
                {
                    solidPtr[i][j] = 'o';
                    edward.isDoorOpen = true;
                }
                else
                {
                    solidPtr[i][j] = ' ';
                }
            }

            if (((solidPtr[i][j] == 's') || (solidPtr[i][j] == 'S')) && (!edward.isKey) && (edward.speedY == 0))
            {
                edward.showKeyAdvice = true;
                edward.keyAdvice.restart().asSeconds();
            }

            if ((solidPtr[i][j] == 'P'))
            {
                edwardLoseHisHealth(edward);
            }

            if ((solidPtr[i][j] == '2'))
            {
                edward.nextLevel = true;
            }
        }
}

void updatesByAnimation(Player &edward)
{
    if ((edward.cannonLeftRect.left == 700) || (edward.cannonRigthRect.left == 560))
    {
        edward.isShellFlying = true;
        edward.shellPos = edward.pos;
        edward.sightSaved = edward.sight;
    }
}

void spellUpdates(Player &edward, float deltaTime)
{
    /* ПУШКА */
    if ((edward.cannonState == cannonState::casting) && (edward.castCircleLeftRect.left == 810 || edward.castCircleRightRect.left == 720))
    {
        if (edward.sight == lineOfSight::right)
        {
            edward.cannon.setPosition(edward.pos.x - edward.offsetX + 120, edward.pos.y - edward.offsetY);
        }
        else
        {
            edward.cannon.setPosition(edward.pos.x - edward.offsetX - 60, edward.pos.y - edward.offsetY);
        }

        edward.cannonState = cannonState::ready;
    }

    /*СНАРЯД */
    if (edward.isShellFlying)
    {
        if (edward.sightSaved == lineOfSight::right)
        {
            edward.shellPos.x += 1 * deltaTime;
        }
        else
        {
            edward.shellPos.x -= 1 * deltaTime;
        }
        edward.shell.setPosition(edward.shellPos.x - edward.offsetX + 100, edward.shellPos.y - edward.offsetY + 40);
    }
    else
    {
        edward.shell.setPosition(0, 0);
    }
}

void edwardUpdate(float deltaTime, Player &edward, String *solidPtr, float blockSize, int &currentLevel, map<String, float> Xlimits, map<String, float> Ylimits)
{
    updatesByMove(edward);
    updatesByAnimation(edward);
    spellUpdates(edward, deltaTime);
    edwardAnimations(edward);

    edward.isEdwardHitEnemy = false;
    edward.pos.x += edward.speedX * deltaTime;
    collision(0, edward, solidPtr, blockSize, currentLevel);
    if (!edward.onGround)
        edward.speedY = edward.speedY + 0.0005 * deltaTime;

    edward.pos.y += edward.speedY * deltaTime;
    edward.onGround = false;

    collision(1, edward, solidPtr, blockSize, currentLevel);

    //========OFFSET COUNTER
    if ((edward.pos.x > Xlimits["left"]) && (edward.pos.x < Xlimits["right"]))
    {
        edward.offsetX = edward.pos.x - 700;
    }

    if ((edward.pos.y > Ylimits["top"]) && (edward.pos.y < Ylimits["bottom"]))
    {
        edward.offsetY = edward.pos.y - 400;
    }
    //=============================

    edward.keyHelper.setPosition(edward.pos.x - edward.offsetX, edward.pos.y - edward.offsetY - 150);
    edward.sprite.setPosition(edward.pos.x - edward.offsetX, edward.pos.y - edward.offsetY);
    edward.curPos = {edward.pos.x - edward.offsetX, edward.pos.y - edward.offsetY};
    edward.speedX = 0;
}

void drawPlayer(sf::RenderWindow &window, Player edward)
{
    if ((edward.showKeyAdvice) && (edward.keyAdvice.getElapsedTime().asSeconds() < 3))
    {
        window.draw(edward.keyHelper);
    }
    window.draw(edward.sprite);
    window.draw(edward.healthBar);
    if (edward.cannonState == cannonState::ready)
        window.draw(edward.cannon);
    if (edward.isShellFlying)
        window.draw(edward.shell);
    window.draw(edward.livesImage);
}

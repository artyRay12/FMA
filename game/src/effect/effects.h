#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
using namespace sf;
using namespace std;

class Effect
{
public:
  Effect(String effectName, String pathToTexture)
  {
    name = effectName;
    texture.loadFromFile(pathToTexture);
    sprite.setScale(1.5, 1.5);
    if (name == "blood-splatter")
    {
      sprite.setScale(4, 4);
      sprite.setOrigin(25, 25);
    }
  }

  Texture texture;
  Sprite sprite;
  int dustOffsetX = 25;

  String name;

  IntRect dustRightRect{0, 0, 80, 80};
  IntRect dustLeftRect{80, 0, -80, 80};

  IntRect magicRect{0, 0, 80, 80};

  IntRect bloodSplatterRightRect{0, 0, 50, 100};
  IntRect bloodSplatterLeftRect{50, 0, -50, 100};

  Clock effectsClock;
  LineOfSight sight;
  Vector2f pos;
  float time = effectsClock.restart().asSeconds();
  bool startJump = false;
  bool playerSide = false;
  bool isCircleCasting = false;
  bool isBloodSplatter = false;

  void dustAnimationRight()
  {
    sprite.setTexture(texture);
    sprite.setTextureRect(dustRightRect);
    if (effectsClock.getElapsedTime().asSeconds() > 0.07f)
    {
      if (dustRightRect.left == 400)
      {
        dustRightRect.left = 0;
        startJump = false;
      }
      else
        dustRightRect.left += 80;
      effectsClock.restart();
    }
  }

  void dustAnimationLeft()
  {
    sprite.setTexture(texture);
    sprite.setTextureRect(dustLeftRect);
    if (effectsClock.getElapsedTime().asSeconds() > 0.07f)
    {
      if (dustLeftRect.left == 480)
      {
        dustLeftRect.left = 80;
        startJump = false;
      }
      else
        dustLeftRect.left += 80;
      effectsClock.restart();
    }
  }

  void magicCircle()
  {
    sprite.setTexture(texture);
    sprite.setTextureRect(magicRect);
    isCircleCasting = true;
    if (effectsClock.getElapsedTime().asSeconds() > 0.15)
    {
      if (magicRect.left == 560)
      {
        magicRect.left = 0;
        isCircleCasting = false;
      }
      else
        magicRect.left += 80;
      effectsClock.restart();
    }
  }

  void bloodSplatterRight()
  {
    sprite.setTexture(texture);
    sprite.setTextureRect(bloodSplatterRightRect);
    if (effectsClock.getElapsedTime().asSeconds() > 0.05)
    {
      if (bloodSplatterRightRect.left == 450)
      {
        bloodSplatterRightRect.left = 0;
        isBloodSplatter = false;
      }
      else
        bloodSplatterRightRect.left += 50;

      effectsClock.restart();
    }
  }

  void bloodSplatterLeft()
  {
    sprite.setTexture(texture);
    sprite.setTextureRect(bloodSplatterLeftRect);
    if (effectsClock.getElapsedTime().asSeconds() > 0.05)
    {
      if (bloodSplatterLeftRect.left == 500)
      {
        bloodSplatterLeftRect.left = 0;
        isBloodSplatter = false;
      }
      else
        bloodSplatterLeftRect.left += 50;
      effectsClock.restart();
    }
  }
};

void effectsPressUpUpdates(Effect &effectName, Effect &magicCircle, Player edward)
{
  if (edward.onGround)
  {
    effectName.pos = edward.curPos;
    effectName.startJump = true;
    effectName.sight = edward.sight;
  }
}

void effectsPressSpaceUpdates(Effect &effectName, Player edward)
{
  effectName.pos = edward.curPos;
  effectName.sight = edward.sight;
}

void effectsPressOneUpdates(Player edward, Effect &effectName)
{
  effectName.sight = edward.sight;
}

void circleUpdate(Effect &effectName, Player edward, float deltaTime)
{
  if (edward.sight == LineOfSight::right)
  {
    effectName.sprite.setPosition(edward.pos.x - edward.offsetX + 60, edward.pos.y - edward.offsetY - 10);
  }
  else
  {
    effectName.sprite.setPosition(edward.pos.x - edward.offsetX - 60, edward.pos.y - edward.offsetY - 10);
  }
  effectName.magicCircle();
}

void effectUpdate(Effect &dust, Effect &magicCircle, Player &edward, float deltaTime, Effect &bloodSplatter)
{
  /* DUST */
  if (dust.startJump == true)
  {
    if (edward.curPos.x >= 700)
      dust.pos.x -= edward.speedX * deltaTime;
    if (edward.curPos.y >= 400)
      dust.pos.y -= edward.speedY * deltaTime;

    if (dust.sight == LineOfSight::right)
    {
      dust.dustOffsetX = 70;
      dust.sprite.setPosition(dust.pos.x - dust.dustOffsetX, dust.pos.y);
      dust.dustAnimationRight();
    }
    else
    {
      dust.dustOffsetX = -20;
      dust.sprite.setPosition(dust.pos.x - dust.dustOffsetX, dust.pos.y);
      dust.dustAnimationLeft();
    }
  }

  /*  MAGIC CIRCLE */
  if ((edward.canICastCircle) && (edward.speedX == 0) && (edward.speedY == 0))
  {
    circleUpdate(magicCircle, edward, deltaTime);
  }
  else
  {
    magicCircle.magicRect.left = 0;
    magicCircle.isCircleCasting = false;
  }

  /* BLOODSPLATTTER */
  if (edward.isEdwardHitEnemy)
  {
    bloodSplatter.isBloodSplatter = true;
    bloodSplatter.pos = edward.enemyWhoDie;
    edward.isEdwardHitEnemy = false;
  }

  if (bloodSplatter.isBloodSplatter)
  {
    /*     B<< "BLOOD SLATTER TRUE" << endl; */
    if (edward.curPos.x >= 700)
      bloodSplatter.pos.x -= edward.speedX * deltaTime;
    if (edward.curPos.y >= 400)
      bloodSplatter.pos.y -= edward.speedY * deltaTime;

    if (bloodSplatter.sight == LineOfSight::left)
    {
      bloodSplatter.dustOffsetX = -150;
      bloodSplatter.bloodSplatterLeft();
    }
    else
    {
      bloodSplatter.dustOffsetX = 150;
      bloodSplatter.bloodSplatterRight();
    }
    bloodSplatter.sprite.setPosition(bloodSplatter.pos.x + bloodSplatter.dustOffsetX, bloodSplatter.pos.y);
  }
}

void drawEffect(RenderWindow &window, Effect &dust, Effect &magicCircle, Effect &bloodSplatter)
{
  window.draw(dust.sprite);
  if (magicCircle.isCircleCasting)
  {
    window.draw(magicCircle.sprite);
  }

  if (bloodSplatter.isBloodSplatter)
  {
    window.draw(bloodSplatter.sprite);
  }
}

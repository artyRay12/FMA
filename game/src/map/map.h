#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

class Map
{
public:
  String *solidPtr;

  Texture *mapTexture;
  Texture *titleTexture;
  Texture *keyTexture;
  Texture *closedDoor;
  Texture *openDoor;

  Sprite sprite;
  Sprite key;
  Sprite title;
  Sprite doorClose;
  Sprite doorOpen;

  IntRect keyRect{0, 0, 30, 70};

  map<String, float> Xlimits;
  map<String, float> Ylimits;

  Clock mapTimer;
  Clock keyAnimTimer;

  // float darkCoeff = 120;
  float blockSize;

  bool titleDraw = true;
  float titleTransparent = 255;
  float titleScale = 1;

  void keyAnimation()
  {
    key.setTextureRect(keyRect);
    if (keyAnimTimer.getElapsedTime().asSeconds() > 0.10f)
    {
      if (keyRect.left == 240)
      {
        keyRect.left = 0;
      }
      else
      {
        keyRect.left += 30;
      }
      keyAnimTimer.restart();
    }
  }
};
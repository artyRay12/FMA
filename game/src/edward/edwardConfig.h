#include <vector>
#include <map>
#include <string>

using namespace sf;
using namespace std;

const int EDWARD_INITIAL_LIVES = 3;
const int EDWARD_INITIAL_HEALTH = 120;

const Vector2f EDWARD_START_POS{100, 100};
const Vector2f EDWARD_LIVES_COUNTER_POS{75, 75};
const Vector2f EDWARD_HEALTH_BAR_POS{85, 80};

const float EDWARD_SPEED_X = 0.3;
const float EDWARD_SPEED_Y = -0.4;

const int EDWARD_HEALTH_BAR_ROTATE = -90;

const Vector2f CANNON_ORIGIN{60, 0};

/*EDWARD ANIMATION INFO*/
const IntRect EDWARD_STAND_RIGHT_SIZE{0, 0, 50, 90};
const IntRect EDWARD_STAND_LEFT_SIZE{50, 0, -50, 90};

const IntRect EDWARD_RUN_RIGHT_SIZE{0, 0, 90, 90};
const IntRect EDWARD_RUN_LEFT_SIZE{90, 0, -90, 90};

const IntRect EDWARD_JUMP_RIGHT_SIZE{0, 0, 70, 90};
const IntRect EDWARD_JUMP_LEFT_SIZE{70, 0, -70, 90};

const IntRect EDWARD_CIRCLE_HIT_RIGHT_SIZE{0, 0, 130, 120};
const IntRect EDWARD_CIRCLE_HIT_LEFT_SIZE{130, 0, -130, 120};

const IntRect EDWARD_JUMP_ATTACK_RIGHT_SIZE{0, 0, 85, 120};
const IntRect EDWARD_JUMP_ATTACK_LEFT_SIZE{85, 0, -85, 120};

const IntRect EDWARD_MAGIC_CIRCLE_CAST_RIGHT_SIZE{0, 0, 90, 150};
const IntRect EDWARD_MAGIC_CIRCLE_CAST_LEFT_SIZE{90, 0, -90, 150};

const IntRect EDWARD_GET_DAMAGE_RIGHT_SIZE{0, 0, 80, 120};
const IntRect EDWARD_GET_DAMAGE_LEFT_SIZE{80, 0, -80, 120};

const IntRect CANNON_RIGHT_SIZE{0, 0, 120, 120};
const IntRect CANNON_LEFT_SIZE{120, 0, -120, 120};

const IntRect SHELL_SIZE{0, 0, 20, 20};

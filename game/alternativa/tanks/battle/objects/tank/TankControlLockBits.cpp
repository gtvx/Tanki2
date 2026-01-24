#include "TankControlLockBits.h"

const int TankControlLockBits::DEAD = 1;
const int TankControlLockBits::SHAFT = 2;
const int TankControlLockBits::ERROR = 4;
const int TankControlLockBits::PAUSE = 8;
const int TankControlLockBits::DISABLED = 16;
const int TankControlLockBits::CLIENT = 32;
const int TankControlLockBits::STUN = 64;
const int TankControlLockBits::DEBUFF = 128;
const int TankControlLockBits::ALL = 0xffffff;

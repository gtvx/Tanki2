#include "TankConst.h"
#include "MyMath.h"

const double TankConst::RAY_OFFSET = 10;
const int TankConst::NUM_RAYS_PER_TRACK = 5;
const double TankConst::MAX_SLOPE_ANGLE = MyMath::PI / 3;
const double TankConst::MAX_SLOPE_ANGLE_COS = MyMath::cos(MAX_SLOPE_ANGLE);

#include "WeaponConst.h"
#include "game/alternativa/tanks/utils/ConstEncryptedNumber.h"

static const double BASE_IMPACT_FORCE1 = 5000000;
const EncryptedNumber *WeaponConst::BASE_IMPACT_FORCE = new ConstEncryptedNumber(BASE_IMPACT_FORCE1);
const int WeaponConst::DEFAULT_BARREL_INDEX = 0;

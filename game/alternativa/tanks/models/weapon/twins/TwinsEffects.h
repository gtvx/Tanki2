#ifndef TWINSEFFECTS_H
#define TWINSEFFECTS_H

#include <memory>

class BattleService;
class TwinsSFXData;
class Object3D;
class Vector3;

class TwinsEffects
{
	BattleService *battleService;
	TwinsSFXData *sfxData;

public:

	static const int FLASH_SIZE;

	TwinsEffects(BattleService *battleService, TwinsSFXData *sfxData);
	void createShotEffects(std::shared_ptr<Object3D> object, const Vector3 *_arg_2);

private:
	void createGraphicEffect(const Vector3 *_arg_1, std::shared_ptr<Object3D> _arg_2);
};

#endif // TWINSEFFECTS_H

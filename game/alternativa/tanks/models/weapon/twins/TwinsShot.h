#ifndef TWINSSHOT_H
#define TWINSSHOT_H

#include "../../weapons/shell/InelasticShell.h"
#include <memory>

class TwinsCC;
class TwinsSFXData;
class DistanceWeakening;
class TwinsWeaponCallback;
class Splash;
class AllGlobalGunParams;
class AnimatedSprite3D;
class Splash;


class TwinsShot : public InelasticShell
{
	std::shared_ptr<AnimatedSprite3D> sprite;
	TwinsCC *twinsInitParams;
	DistanceWeakening *weakening;
	TwinsWeaponCallback *callback;
	double impactForce;
	Splash *splash;
	TwinsSFXData *sfxData;

public:
	TwinsShot(Pool *pool);

	void init(double impactForce, TwinsCC *twinsCC, TwinsSFXData *sfxData, DistanceWeakening *weakening, TwinsWeaponCallback *callback, Splash *splash);

	ShellStates* createShellStates() override;
	void addToGame(AllGlobalGunParams *_arg_1, const Vector3 *_arg_2, Body *_arg_3, bool _arg_4, int _arg_5) override;
	double getSpeed() override;
	double getMaxDistance() override;
	void processHitImpl(Body *_arg_1, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4, int _arg_5) override;
	void render(int _arg_1, int _arg_2) override;
	void destroy() override;
	double getRadius() override;
	void createExplosionEffect(const Vector3 *_arg_1, double _arg_2);
	void createExplosionLightEffect(const Vector3 *_arg_1);
	void createHitMark(const Vector3 *_arg_1);
};

#endif // TWINSSHOT_H

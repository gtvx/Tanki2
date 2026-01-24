#ifndef TWINSWEAPONCALLBACKADAPT_H
#define TWINSWEAPONCALLBACKADAPT_H

#include "TwinsWeaponCallback.h"
#include <memory>

class IGameObject;

class TwinsWeaponCallbackAdapt : public TwinsWeaponCallback
{
	IGameObject *object;
	std::shared_ptr<TwinsWeaponCallback> impl;

public:
	TwinsWeaponCallbackAdapt(IGameObject *object, std::shared_ptr<TwinsWeaponCallback> impl);

private:
	void onShot(int time, int shotId, int currentBarrel, const Vector3 *shotDirection);
	void onDummyShot(int time, int currentBarrel);
	void onTargetHit(int shotId, Body *targetBody, const Vector3 *hitPoint);
	void onStaticHit(int shotId, const Vector3 *position);
};

#endif // TWINSWEAPONCALLBACKADAPT_H

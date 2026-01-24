#ifndef TWINSWEAPONCALLBACK_H
#define TWINSWEAPONCALLBACK_H

class Vector3;
class Body;

class TwinsWeaponCallback
{
public:
	virtual void onShot(int time, int shotId, int currentBarrel, const Vector3 *shotDirection) = 0;
	virtual void onDummyShot(int time, int currentBarrel) = 0;
	virtual void onTargetHit(int shotId, Body *targetBody, const Vector3 *hitPoint) = 0;
	virtual void onStaticHit(int shotId, const Vector3 *position) = 0;
};

#endif // TWINSWEAPONCALLBACK_H

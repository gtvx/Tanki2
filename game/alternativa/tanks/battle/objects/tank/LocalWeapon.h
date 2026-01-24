#ifndef LOCALWEAPON_H
#define LOCALWEAPON_H

class LocalWeapon
{
public:
	virtual void pullTrigger() = 0;
	virtual void releaseTrigger() = 0;
};

#endif // LOCALWEAPON_H

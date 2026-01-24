#ifndef CUSTOMTURRETSKIN_H
#define CUSTOMTURRETSKIN_H

class Tanks3DSResource;
class TurretSkin;

class CustomTurretSkin
{
public:
	 virtual TurretSkin* createSkin(Tanks3DSResource*) = 0;
};

#endif // CUSTOMTURRETSKIN_H

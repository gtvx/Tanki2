#ifndef SPLASH_H
#define SPLASH_H

class Vector3;
class SplashParams;

class Splash
{
public:
	virtual bool applySplashForce(const Vector3*, double, Body*, SplashParams* = nullptr) = 0;
};

#endif // SPLASH_H

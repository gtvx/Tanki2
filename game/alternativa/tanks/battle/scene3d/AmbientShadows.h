#ifndef AMBIENTSHADOWS_H
#define AMBIENTSHADOWS_H

class Camera3D;
class Shadow;

class AmbientShadows
{
	int color;
	Camera3D *camera;
	double alpha = 0.85;
	double nearDistance;
	double farDistance;

public:
	AmbientShadows(Camera3D*);
	void add(Shadow *shadow);

private:
	void registerConsoleCommands();
};

#endif // AMBIENTSHADOWS_H

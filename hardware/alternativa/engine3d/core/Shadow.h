#ifndef SHADOW_H
#define SHADOW_H

#include <QVector>
#include "flash/geom/Vector3D.h"

class Mesh;

class Shadow
{
	QVector<Mesh*> casters;
	int castersCount;

public:

	Shadow();
	Shadow(int mapSize, int blur, double attenuation, double nearDistance, double farDistance, int color = 0, double alpha = 1);

	Vector3D direction;

	double attenuation;
	double nearDistance;
	double farDistance;
	double alpha;
	double offset = 0;
	double backFadeRange = 0;

	int mapSize;
	int blur;
	int color;
};

#endif // SHADOW_H

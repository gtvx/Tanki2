#include "Shadow.h"

Shadow::Shadow(int mapSize, int blur, double attenuation, double nearDistance, double farDistance, int color, double alpha) :
	direction(0, 0, -1)
{
	this->castersCount = 0;
	this->mapSize = mapSize;
	this->blur = blur;
	this->attenuation = attenuation;
	this->nearDistance = nearDistance;
	this->farDistance = farDistance;
	this->color = color;
	this->alpha = alpha;
}

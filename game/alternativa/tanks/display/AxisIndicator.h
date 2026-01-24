#ifndef AXISINDICATOR_H
#define AXISINDICATOR_H

class Camera3D;
class Canvas;

class AxisIndicator
{
	int _size;

public:
	AxisIndicator(int size);
	void update(Canvas *canvas, Camera3D *camera);
	int get_size();
};

#endif // AXISINDICATOR_H

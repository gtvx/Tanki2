#include "AxisIndicator.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "Canvas.h"
#include "Graphics.h"


AxisIndicator::AxisIndicator(int size)
{
	this->_size = size;
}


void AxisIndicator::update(Canvas *canvas, Camera3D *camera)
{
	camera->composeMatrix();

	double axis1[6];
	axis1[0] = camera->matrix4.ma;
	axis1[1] = camera->matrix4.mb;
	axis1[2] = camera->matrix4.me;
	axis1[3] = camera->matrix4.mf;
	axis1[4] = camera->matrix4.mi;
	axis1[5] = camera->matrix4.mj;
	int size = this->_size / 2;
	int i = 0;
	uint32_t color = 16;
	while (i < 6)
	{
		double a = axis1[i] + 1;
		double b = axis1[i+1] + 1;
		canvas->gfx->moveTo(size, size);
		canvas->gfx->lineTo(size * a, size * b);
		canvas->gfx->end((0xFF << color));
		i = i + 2;
		color = color - 8;
	}
}


int AxisIndicator::get_size()
{
	return this->_size;
}

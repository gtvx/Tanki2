#ifndef DEBUG_H
#define DEBUG_H

#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "Canvas.h"
#include <stdint.h>

class Vertex;
class Wrapper;
class AABB;


void Debug_drawBounds(Canvas*, Bound*, double, double, const Matrix4*, uint32_t color);
void Debug_drawRect(Canvas *canvas, Bound *b, double viewSizeX, double viewSizeY, const Matrix4 *m, uint32_t color, int *z);
void Debug_drawTriangle(Canvas *canvas, Vector3 *v0, Vector3 *v1, Vector3 *v2, double viewSizeX, double viewSizeY, const Matrix4 *m, uint32_t color);
void Debug_draw_point(Canvas*, double viewSizeX, double viewSizeY, const Matrix4*, Wrapper *wrapper, uint32_t color);
void Debug_draw_point(Canvas *canvas, double viewSizeX, double viewSizeY, const Matrix4 *m, const Vector3 *v, uint32_t color);
void Debug_draw_point(Canvas *canvas, double viewSizeX, double viewSizeY, const Matrix4 *m, const Vector3 *a,const Vector3 *b, uint32_t color);

#endif // DEBUG_H

#ifndef WIDGETPRIMITIVE_H
#define WIDGETPRIMITIVE_H

#include <QWidget>
#include "Graphics.h"

class BattlefieldModel;
class GameCamera;
class Tank;
class Canvas;
class CollisionRect;
class CollisionKdNode;
class Vector3;
class Object3D;
class Camera3D;


class WidgetPrimitive : public QWidget
{
	Graphics graphics;
	BattlefieldModel *battlefieldModel;
	GameCamera *camera;
	bool mouse_left_pressed;
	Tank *localTank;

	void draw_rect(Canvas *canvas, CollisionRect *rect, uint32_t color);
	void draw_node(Canvas *canvas, CollisionKdNode *node);
	void drawTank(Canvas *canvas, const Vector3 &camera_position, Tank *tank);
	void draw(Canvas *canvas, Camera3D *camera, Object3D *object, uint32_t color);
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);


public:
	WidgetPrimitive();

	void setBattlefieldModel(BattlefieldModel *battlefieldModel)
	{
		this->battlefieldModel = battlefieldModel;
	}

	void setGameCamera(GameCamera *camera)
	{
		this->camera = camera;
	}

	void setLocalTank(Tank *localTank)
	{
		this->localTank = localTank;
	}
};

#endif // WIDGETPRIMITIVE_H

#include "WidgetPrimitive.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "game/alternativa/physics/collision/CollisionKdNode.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "Debug.h"
#include "getBattleRunner.h"
#include "game/alternativa/tanks/models/battle/battlefield/BattlefieldModel.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"
#include "global_class.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistry.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"
#include <QMouseEvent>
#include <QPainter>


namespace
{
	void setBound(const Vector3 *hs, Bound *bound)
	{
		bound->boundMinX = -hs->getX();
		bound->boundMinY = -hs->getY();
		bound->boundMinZ = -hs->getZ();
		bound->boundMaxX = hs->getX();
		bound->boundMaxY = hs->getY();
		bound->boundMaxZ = hs->getZ();
	}


	void setBound(const AABB *aabb, Bound *bound)
	{
		bound->boundMinX = aabb->minX;
		bound->boundMinY = aabb->minY;
		bound->boundMinZ = aabb->minZ;
		bound->boundMaxX = aabb->maxX;
		bound->boundMaxY = aabb->maxY;
		bound->boundMaxZ = aabb->maxZ;
	}




	bool isMaxLength(const Matrix4 &m, const Vector3 &camera_position, const double &length)
	{
		Vector3 position;
		m.getPosition(&position);
		Vector3 vector;
		vector.diff(&position, &camera_position);
		return vector.length() > length;
	}



	enum BoxType
	{
		BoxNONE = 0,
		BoxBrickWall = 1,
		BoxConcreteWall = 2,
		BoxWater = 3,
		BoxBushes = 4,
		BoxIce = 5,
	};



	uint32_t getBoxColor(BoxType t)
	{
		switch (t)
		{
			case BoxBrickWall:
				return 0xC07000;
			case BoxConcreteWall:
				return 0xFFFFFF;
			case BoxWater:
				return 0x0000AA;
			case BoxBushes:
				return 0x97E800;
			case BoxIce:
				return 0x003300;
			default:
				return 0;
		}
	}
}


WidgetPrimitive::WidgetPrimitive()
{
	mouse_left_pressed = false;
}


void WidgetPrimitive::draw_rect(Canvas *canvas, CollisionRect *rect, uint32_t color)
{
	Matrix4 m = rect->transform;
	Bound bound;
	setBound(&rect->hs, &bound);
	m.append(&camera->matrix4);
	int z;
	Debug_drawRect(canvas, &bound, camera->viewSizeX, camera->viewSizeY, &m, color, &z);
}


void WidgetPrimitive::draw_node(Canvas *canvas, CollisionKdNode *node)
{
	if (node == nullptr)
		return;

	AABB &aabb = node->boundBox;

	int z;

	Bound bound;
	setBound(&aabb, &bound);
	Debug_drawRect(canvas, &bound, camera->viewSizeX, camera->viewSizeY, &camera->matrix4, 0xFF0000, &z);

	draw_node(canvas, node->negativeNode);
	draw_node(canvas, node->positiveNode);

	//if (node->negativeNode != nullptr)
	//draw_node(canvas, node->splitTree->rootNode);
}


void WidgetPrimitive::drawTank(Canvas *canvas, const Vector3 &camera_position, Tank *tank)
{
	(void)camera_position;

	TankSkin *skin = tank->getSkin();
	Mesh *mesh_hull = skin->getHullMesh().get();
	Mesh *mesh_turret = (Mesh*)skin->getTurret3D().get();

	if (mesh_turret->z < -10000) {
		tank->setHealth(0);
	}

	uint32_t color = 0;
	if (tank->getHealth() == 0)
		color = 0x8F3D0B;

	draw(canvas, camera, mesh_hull, color);
	draw(canvas, camera, mesh_turret, color);
}


void WidgetPrimitive::draw(Canvas *canvas, Camera3D *camera, Object3D *object, uint32_t color)
{
	object->matrix4.setMatrix(object->x, object->y, object->z, object->rotationX, object->rotationY, object->rotationZ);
	object->matrix4.append(&camera->matrix4);

	//Debug_drawBounds(canvas, &object->bound, camera->viewSizeX, camera->viewSizeY, &object->matrix4, 0x000000);

	Mesh *mesh = (Mesh*)object;


	Weak<Face> face = mesh->faceList;
	while (face != nullptr)
	{
		Debug_draw_point(canvas, camera->viewSizeX, camera->viewSizeY, &object->matrix4, face->wrapper.get(), color);
		face = face->next;
	}
}


void WidgetPrimitive::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	graphics.setPainter(&painter);

	Canvas canvas(&graphics);

	getBattleRunner()->battleViewUpdate(&canvas);

	MyList<CollisionShape *> &map = *battlefieldModel->mapStaticChildren();

	Vector3 camera_position;
	camera_position.setX(camera->x);
	camera_position.setY(camera->y);
	camera_position.setZ(camera->z);

	int w = this->width();
	int h = this->height();

	camera->render(w, h);
	camera->matrix4.invert();


	{
		double health = localTank->getHealth();
		static QString str;
		static double last_health = -1;
		if (last_health != health) {
			str = QString::number(localTank->getHealth());
		}
		graphics.drawString(str, 5, 130);
		graphics.end(health > 200 ? 0x550000 : 0xFF0000);
	}

	painter.setViewport(camera->viewSizeX, camera->viewSizeY, w, h);

	int _z = -1;
	CollisionRect *_rect = nullptr;

	for (int i = 0; i < map.count(); i++) {
		CollisionShape *shape = map.at(i);

		switch (shape->type)
		{

			case CollisionShape::Type::BOX: { //Коробка
				CollisionBox *box = (CollisionBox*)shape;
				if (box->hide == false)
				{
					Matrix4 m = box->transform;
					//if (isMaxLength(m, camera_position, 4000)) break;
					m.append(&camera->matrix4);
					Bound bound;
					setBound(box->constHS(), &bound);
					Debug_drawBounds(&canvas, &bound, camera->viewSizeX, camera->viewSizeY, &m, getBoxColor((BoxType)box->t)); //blue
				}
				break;
			}

			case CollisionShape::Type::RECT: {
				CollisionRect *rect = (CollisionRect*)shape;
				Matrix4 m = rect->transform;
				//if (isMaxLength(m, camera_position, 4000)) break;
				Bound bound;
				setBound(&rect->hs, &bound);
				m.append(&camera->matrix4);
				int z;
				Debug_drawRect(&canvas, &bound, camera->viewSizeX, camera->viewSizeY, &m, 0x00FF00, &z); //green
				if (z != -1) {
					if (_z == -1) {
						_z = z;
						_rect = rect;
					} else if (z < _z) {
						_z = z;
						_rect = rect;
					}
				}
				break;
			}

				/*
			case CollisionShape::Type::SPHERE: { //Сфера
				CollisionSphere *sphere = (CollisionSphere*)shape;
				AABB *aabb = sphere->calculateAABB();
				Matrix4 m = sphere->transform;
				m.append(&camera->matrix4);
				Debug_drawBounds(&canvas, aabb, camera->viewSizeX, camera->viewSizeY, &m, 0xFF0000); //red
				break;
			}
			*/

			case CollisionShape::Type::TRIANGLE: { //Треугольник
				CollisionTriangle *triangle = (CollisionTriangle*)shape;
				Matrix4 m = triangle->transform;
				if (isMaxLength(m, camera_position, 4000)) break;
				m.append(&camera->matrix4);
				Debug_drawTriangle(&canvas, &triangle->v0,
								   &triangle->v1,
								   &triangle->v2,
								   camera->viewSizeX, camera->viewSizeY, &m, 0xFF0000); //black
				break;
			}



			default:
				break;
		}


	}


	if (_rect != nullptr) {
		draw_rect(&canvas, _rect, 0xFF0000);
	}

	//var _local_2:ITankModel = ITankModel(_arg_1.adapt(ITankModel));
	//return _local_2.getTank();



	MyList<IGameObject*> *users = getTankUsersRegistry()->getUsers();
	//qDebug() << users->count();
	for (int i = 0; i < users->count(); i++) {

		IGameObject *user = users->at(i);

		Tank *tank = user->adapt<ITankModel>()->getTank();

		drawTank(&canvas, camera_position, tank);


		/*
			TankBody *tankBody = tank->getTankBody();
			Body *body = tankBody->body;
			//const int count = tankBody->staticShapes->count();
			const int count = body->collisionShapes->count();
			for (int i = 0; i < count; i++) {
				CollisionBox *box = (CollisionBox*)body->collisionShapes->at(i);
				//if (tankBody->tankCollisionBox != box)
					//continue;
				//CollisionBox *box = (CollisionBox*)tankBody->staticShapes->at(i);
				//CollisionBox *box = tankBody->tankCollisionBox;
				Matrix4 m = box->transform;
				m.append(&camera->matrix4);
				Bound bound;
				setBoud(&box->hs, &bound);
				Debug_drawBounds(&canvas, &bound, camera->viewSizeX, camera->viewSizeY, &m, 0xFF0000); //blue
				//BodyState *b = &body->state;
				//b->velocity.setX(100);
			}
			*/


	}

	{
		//TanksCollisionDetector *c = getBattleRunner()->getCollisionDetector();
		//CollisionKdNode * s = c->tree->rootNode;
		//draw_node(&canvas, s);
	}


}


void WidgetPrimitive::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		mouse_left_pressed = true;
	}
}


void WidgetPrimitive::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		mouse_left_pressed = false;
	}
}


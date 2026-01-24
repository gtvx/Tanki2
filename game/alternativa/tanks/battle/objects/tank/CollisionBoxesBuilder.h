#ifndef COLLISIONBOXESBUILDER_H
#define COLLISIONBOXESBUILDER_H

class Vector3;
class TankBody;

class CollisionBoxesBuilder
{
public:
	static void createStaticCollisionBoxes(Vector3 *param1, double param2, TankBody *param3);
	static void createTankCollisionBox(Vector3 *param1, double param2, TankBody *param3);
};

#endif // COLLISIONBOXESBUILDER_H

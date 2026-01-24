#include "DecalFactory.h"
#include "game/alternativa/physics/collision/CollisionDetector.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/battle/scene3d/RotationState.h"
#include "MyMath.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include "flash/geom/Vector3D.h"
#include "hardware/alternativa/engine3d/containers/KDContainer.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"


static const double ANGLE_LIMIT = ((85 * MyMath::PI) / 180); //1.48352986419518



static double getRotation(const RotationState *state)
{
	if (state == RotationState::WITHOUT_ROTATION)
		return 0;
	else
		return MyMath::random() * 2 * MyMath::PI;
}



DecalFactory::DecalFactory(CollisionDetector *collisionDetector)
{
	this->collisionDetector = collisionDetector;
}


std::shared_ptr<Decal> DecalFactory::createDecal(const Vector3 *_arg_1,
												 const Vector3 *_arg_2,
												 double _arg_3,
												 std::shared_ptr<TextureMaterial> material,
												 KDContainer *_arg_5,
												 const RotationState *state)
{
	Vector3 direction;

	direction.diff(_arg_1, _arg_2);

	double _local_7 = direction.length() + 200;

	direction.normalize();

	Vector3 right, up, normal;

	right.cross2(&direction, Vector3::Z_AXIS);

	right.normalize();

	up.cross2(&right, &direction);

	Vector3 origins[5];


	origins[4].copy(_arg_2);

	origins[0].copy(_arg_2);
	origins[0].addScaled(50, &right);

	origins[1].copy(_arg_2);
	origins[1].addScaled(50, &up);

	origins[2].copy(_arg_2);
	origins[2].addScaled(-50, &right);

	origins[3].copy(_arg_2);
	origins[3].addScaled(-50, &up);

	normal.reset(0, 0, 0);

	for (int i = 0; i < 5; i++)
	{
		const Vector3 *origin = &origins[i];

		RayHit rayHit;

		if (this->collisionDetector->raycastStatic(origin, &direction, CollisionGroup::STATIC, _local_7, nullptr, &rayHit))
		{
			normal.add(&rayHit.normal);
		}
	}

	normal.normalize();

	Vector3D position3D, normal3D;

	BattleUtils::copyToVector3D(_arg_1, &position3D);

	BattleUtils::copyToVector3D(&normal, &normal3D);

	double _local_9 = getRotation(state);

	return _arg_5->createDecal(&position3D, &normal3D, _arg_3, _local_9, ANGLE_LIMIT, 300, material);
}

#ifndef DECALFACTORY_H
#define DECALFACTORY_H

#include <memory>

class CollisionDetector;
class Decal;
class TextureMaterial;
class KDContainer;
class RotationState;
class Vector3;

class DecalFactory
{
	CollisionDetector *collisionDetector;

public:
	DecalFactory(CollisionDetector *collisionDetector);
	std::shared_ptr<Decal> createDecal(const Vector3 *_arg_1,
									   const Vector3 *projectionOrigin,
									   double radius,
									   std::shared_ptr<TextureMaterial> material,
									   KDContainer *container,
									   const RotationState *state);
};

#endif // DECALFACTORY_H

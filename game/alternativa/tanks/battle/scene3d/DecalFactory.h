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
									   const Vector3 *_arg_2,
									   double _arg_3,
									   std::shared_ptr<TextureMaterial> material,
									   KDContainer *_arg_5,
									   const RotationState *_arg_6);
};

#endif // DECALFACTORY_H

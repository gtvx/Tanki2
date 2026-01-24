#ifndef ITWINSMODELBASE_H
#define ITWINSMODELBASE_H

class IGameObject;
class Vector3d;

class ITwinsModelBase
{
public:
	virtual void fire(IGameObject*, int, int, const Vector3d*) = 0;
	virtual void fireDummy(IGameObject*, int) = 0;
};

#endif // ITWINSMODELBASE_H

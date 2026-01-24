#ifndef SCENE3DCONTAINER_H
#define SCENE3DCONTAINER_H

#include <QVector>
#include <memory>

class Object3D;

class Scene3DContainer
{
public:
	virtual void addChild(std::shared_ptr<Object3D>) = 0;
	virtual void addChildAt(std::shared_ptr<Object3D>, int) = 0;
	virtual void addChildren(QVector<std::shared_ptr<Object3D>>&) = 0;
	virtual void removeChild(std::shared_ptr<Object3D>) = 0;
};

#endif // SCENE3DCONTAINER_H

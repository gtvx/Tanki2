#ifndef OBJECT3DCONTAINERPROXY_H
#define OBJECT3DCONTAINERPROXY_H

#include "Scene3DContainer.h"
#include <QMap>

class Object3DContainer;
class Object3D;

class Object3DContainerProxy : public Scene3DContainer
{
	std::shared_ptr<Object3DContainer> container;
	QMap<std::shared_ptr<Object3D>, bool> objects;

public:
	Object3DContainerProxy(std::shared_ptr<Object3DContainer> = nullptr);
	void setContainer(std::shared_ptr<Object3DContainer>);
	void addChild(std::shared_ptr<Object3D>) override;
	void addChildren(QVector<std::shared_ptr<Object3D>>&) override;
	QVector<std::shared_ptr<Object3D>> removeAllChildren();
	void removeChild(std::shared_ptr<Object3D>) override;
	void addChildAt(std::shared_ptr<Object3D>, int) override;
};

#endif // OBJECT3DCONTAINERPROXY_H

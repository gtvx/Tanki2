#ifndef OBJECT3DCONTAINER_H
#define OBJECT3DCONTAINER_H

#include "Object3D.h"
#include <QVector>

class Object3DContainer : public Object3D
{
	int numVisibleChildren;
	QVector<Object3D*> visibleChildren;
	bool mouseChildren;

protected:
	std::shared_ptr<Object3D> childrenList;

public:

	Object3DContainer();
	std::shared_ptr<Object3D> addChild(std::shared_ptr<Object3D>);
	void addToList(std::shared_ptr<Object3D>, std::shared_ptr<Object3D> = nullptr);
	void removeChild(std::shared_ptr<Object3D> );
	void collectPlanes(const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, const Vector3D*, Faces*, void* = nullptr) override;
	std::shared_ptr<Object3D> clone() const;
	std::shared_ptr<Object3D> addChildAt(std::shared_ptr<Object3D> _arg_1, int _arg_2);
	void updateBounds(Object3D *_arg_1, Object3D *_arg_2 = nullptr) override;
	void draw(Camera3D *camera) override;
	Shared<VG> getVG(Camera3D *camera) override;

protected:
	void clonePropertiesFrom(const Object3D*) override;

private:
	void drawVisibleChildren(Camera3D *camera);
};

std::shared_ptr<Object3DContainer> new_Object3DContainer();

#endif // OBJECT3DCONTAINER_H

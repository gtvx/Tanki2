#ifndef OBJECT3DPOSITIONPROVIDER_H
#define OBJECT3DPOSITIONPROVIDER_H

class Object3D;
class GameCamera;

class Object3DPositionProvider
{
public:
	virtual void initPosition(Object3D*) = 0;
	virtual void updateObjectPosition(Object3D*, GameCamera*, int) = 0;
	virtual void destroy() = 0;
};

#endif // OBJECT3DPOSITIONPROVIDER_H

#ifndef CAMERATARGET_H
#define CAMERATARGET_H

class Vector3;

class CameraTarget
{
public:
	virtual void getCameraParams(Vector3*, Vector3*) = 0;
};

#endif // CAMERATARGET_H

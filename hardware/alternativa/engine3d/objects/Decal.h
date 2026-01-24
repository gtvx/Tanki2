#ifndef DECAL_H
#define DECAL_H

#include "Mesh.h"

class Decal : public Mesh
{
public:

	double attenuation;

	Decal();

	std::shared_ptr<Object3D> clone() const override;
	void clonePropertiesFrom(Object3D *src);
	void draw(Camera3D *camera) override;
	Shared<VG> getVG(Camera3D *camera) override;
	void prepareResources() override;
};

std::shared_ptr<Decal> new_Decal();

#endif // DECAL_H

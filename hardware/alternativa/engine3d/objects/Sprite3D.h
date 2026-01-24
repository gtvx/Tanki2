#ifndef SPRITE3D_H
#define SPRITE3D_H

#include "../core/Object3D.h"
#include <memory>


class Material;
class Face;

class Sprite3D : public Object3D
{
public:

	std::shared_ptr<Material> material;

	double width;
	double height;

	double rotation;

	double originX;
	double originY;

	double topLeftU;
	double topLeftV;

	double bottomRightU;
	double bottomRightV;

	float lightConst[4];

	int clipping;
	int sorting;
	bool autoSize;
	bool lighted;
	bool perspectiveScale;
	bool depthTest;


	Sprite3D(double width, double height, std::shared_ptr<Material> material = nullptr);

	virtual void clonePropertiesFrom(const Object3D *src);

	virtual std::shared_ptr<Object3D> clone() const;

	void updateBounds(Object3D *_arg_1, Object3D *_arg_2 = nullptr) override;

	void draw(Camera3D *camera) override;

	Shared<VG> getVG(Camera3D *camera) override;

private:
	void calculateLight(Camera3D *camera);
	Shared<Face> calculateFace(Camera3D *camera);
};

std::shared_ptr<Sprite3D> new_Sprite3D(double width, double height, std::shared_ptr<Material> material = nullptr);

#endif // SPRITE3D_H

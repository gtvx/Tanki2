#ifndef FILLMATERIAL_H
#define FILLMATERIAL_H

#include "TextureMaterial.h"
#include "shared.h"

class Camera3D;
class Canvas;
class Face;

class FillMaterial : public TextureMaterial
{
	int color;
	int lineColor;
	double alpha;
	double lineThickness;

public:
	FillMaterial(int color = 8355711, double lineThickness = 1, double alpha = -1, int lineColor = 16777215);
	virtual void draw(Camera3D *param1, Canvas *param2, Shared<Face> param3, double param4);
	void init(DrawInitParams*) override;
};

std::shared_ptr<FillMaterial> new_FillMaterial(int color = 8355711, double lineThickness = 1, double alpha = -1, int lineColor = 1677721);

#endif // FILLMATERIAL_H

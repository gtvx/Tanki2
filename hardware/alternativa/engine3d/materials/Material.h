#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>
#include "shared.h"
#include "../core/Face.h"

class Camera3D;
class Canvas;
class Face;
class QString;
class DrawParams;


class Material : public std::enable_shared_from_this<Material>
{
	bool isTransparent;

public:

	Material();

	std::string name;
	bool useVerticesNormals;
	double alphaTestThreshold;

	virtual void draw(Camera3D*, Canvas*, Shared<Face>, double);

	QString toString() const;

	virtual bool transparent();

	void setTransparent(bool s);

	virtual void drawOpaqueVulkan(DrawParams *p) = 0;
	virtual void drawTransparentVulkan(DrawParams *p) = 0;
};

#endif // MATERIAL_H

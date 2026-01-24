#ifndef FACE_H
#define FACE_H

#include <memory>
#include "shared.h"

class Material;
class Wrapper;
class VG;

class Face : public SharedObject
{
	Face();
public:

	Shared<Face> next;
	std::shared_ptr<Material> material;
	Shared<Wrapper> wrapper;
	Shared<Face> processNext;
	Shared<Face> processNegative;
	Shared<Face> processPositive;
	Shared<VG> geometry;

	double normalX;
	double normalY;
	double normalZ;
	double offset;
	double distance;

	uint32_t smoothingGroups;

	int id;

	static Shared<Face> create(int id);
	void remove();
	void calculateBestSequenceAndNormal();
	void copy(const Face *src);
};

#endif // FACE_H

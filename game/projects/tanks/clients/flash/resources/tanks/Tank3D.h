#ifndef TANK3D_H
#define TANK3D_H

#include <string>
#include <memory>

class Mesh;
class QString;

class Tank3D
{
public:
	static const QString *TANK_PART;
	static std::shared_ptr<Mesh> cloneMesh(std::shared_ptr<Mesh> mesh);
	static bool EXCLUDED_test(const QString*);
	static bool EXCLUDED_test(const std::string &name);
};

#endif // TANK3D_H

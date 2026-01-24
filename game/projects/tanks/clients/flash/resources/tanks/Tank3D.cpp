#include "Tank3D.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include <QString>



std::shared_ptr<Mesh> Tank3D::cloneMesh(std::shared_ptr<Mesh> mesh)
{
	std::shared_ptr<Mesh> new_mesh = std::dynamic_pointer_cast<Mesh>(mesh->clone());
	new_mesh->name = "tankPart";
	//new_mesh->colorTransform = new ColorTransform();
	//new_mesh->shadowMapAlphaThreshold = 0.1;
	//new_mesh->calculateVerticesNormalsBySmoothingGroups(0.01); //e
	return new_mesh;
}


bool Tank3D::EXCLUDED_test(const QString *name)
{
	QString l = name->toLower();
	bool s = l.contains("box") ||
			l.contains("fmnt") ||
			l.contains("muzzle") ||
			l.contains("laser") ||
			l.contains("rocket");

	return s;
}


static bool string_contains(const std::string &str, const std::string &substring)
{
	return str.find(substring) != std::string::npos;
}


bool Tank3D::EXCLUDED_test(const std::string &name)
{
	std::string temp = name;

	std::transform(temp.begin(), temp.end(), temp.begin(),
					  [](unsigned char c) { return std::tolower(c); });

	bool s = string_contains(temp, "box") ||
			string_contains(temp, "fmnt") ||
			string_contains(temp, "muzzle") ||
			string_contains(temp, "laser") ||
			string_contains(temp, "rocket");

	return s;
}

#include "Material.h"
#include <QString>


Material::Material()
{
	alphaTestThreshold = 0;
	isTransparent = false;
	useVerticesNormals = true;
}


void Material::draw(Camera3D *, Canvas *, Shared<Face>, double)
{

}


QString Material::toString() const
{
	return QString::fromStdString(this->name);
}


bool Material::transparent()
{
	return isTransparent;
}


void Material::setTransparent(bool s)
{
	isTransparent = s;
}

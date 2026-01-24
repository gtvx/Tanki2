#include "ResourceInfo.h"
#include <QString>

ResourceInfo::ResourceInfo(int type, const Long &id, const Long &version, bool isLazy)
{
	this->type = type;
	this->id = id;
	this->version = version;
	this->isLazy = isLazy;
}

QString ResourceInfo::toString() const
{
	return "[ResourceInfo type=" + QString::number(this->type) +
			", id=" + this->id.toString() + ", version=" +
			this->version.toString() + ", isLazy=" + QString::number(this->isLazy) + "]";
}

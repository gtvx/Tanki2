#include "PropGroup.h"
#include "PropData.h"


PropGroup::PropGroup(const QString &name)
{
	this->name = name;
	this->props = nullptr;
	this->groups = nullptr;
}


PropData* PropGroup::getPropByName(const QString &propName)
{
	if (this->props == nullptr)
	{
		return nullptr;
	}


	for (PropData *prop : *this->props)
	{
		if (prop->getName() == propName)
		{
			return prop;
		}
	}

	return nullptr;
}


PropGroup* PropGroup::getGroupByName(const QString &groupName)
{
	if (this->groups == nullptr)
	{
		return nullptr;
	}


	for (PropGroup *group : *this->groups)
	{
		if (group->name == groupName)
		{
			return group;
		}
	}

	return nullptr;
}


void PropGroup::addProp(PropData *prop)
{
	if (this->props == nullptr)
	{
		this->props = new QVector<PropData*>();
	}

	this->props->append(prop);
}


void PropGroup::addGroup(PropGroup *group)
{
	if (this->groups == nullptr)
	{
		this->groups = new QVector<PropGroup*>();
	}

	this->groups->append(group);
}

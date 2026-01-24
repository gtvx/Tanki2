#include "PropState.h"
#include "Error.h"
#include "PropLOD.h"

const QString PropState::DEFAULT_NAME = "default";


PropState::PropState()
{

}


void PropState::addLOD(PropObject *prop, double distance)
{
	this->_lods.append(new PropLOD(prop, distance));
}


int PropState::getNumLODs()
{
	return this->_lods.length();
}


PropLOD* PropState::lodByIndex(int index)
{
	return this->_lods[index];
}


PropObject* PropState::getDefaultObject()
{
	if (this->_lods.length() == 0)
	{
		throw Error("No LODs found");
	}

	return this->_lods[0]->prop;
}

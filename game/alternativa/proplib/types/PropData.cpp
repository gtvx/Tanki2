#include "PropData.h"
#include "PropState.h"


PropData::PropData(const QString &name)
{
	this->_name = name;
}


QString PropData::getName()
{
	return this->_name;
}


void PropData::addState(const QString &stateName, PropState *state)
{
	this->states[stateName] = state;
}


PropState* PropData::getStateByName(const QString &stateName)
{
	return this->states[stateName];
}


PropState* PropData::getDefaultState()
{
	return this->states[PropState::DEFAULT_NAME];
}

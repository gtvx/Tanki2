#include "TargetingSector.h"


void TargetingSector::set(double left, double right, double distance, Tank *tank)
{
	this->left = left;
	this->right = right;
	this->distance = distance;
	this->tank = tank;
}


double TargetingSector::getLeft()
{
	return this->left;
}


double TargetingSector::getRight()
{
	return this->right;
}


double TargetingSector::getDistance()
{
	return this->distance;
}


Tank *TargetingSector::getTank()
{
	return this->tank;
}

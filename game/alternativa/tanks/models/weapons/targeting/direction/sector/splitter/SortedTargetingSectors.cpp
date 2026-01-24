#include "SortedTargetingSectors.h"
#include "game/alternativa/tanks/models/weapons/targeting/direction/sector/TargetingSector.h"


void SortedTargetingSectors::clear()
{
	qDeleteAll(this->orderByRight);
	qDeleteAll(this->orderByDistance);
	this->orderByRight.clear();
	this->orderByDistance.clear();
}


QVector<TargetingSector*>* SortedTargetingSectors::getOrderedByDistance()
{
	return &this->orderByDistance;
}


double SortedTargetingSectors::getRight()
{
	return this->orderByRight.at(0)->getRight();
}


bool SortedTargetingSectors::isEmpty()
{
	return this->orderByRight.count() == 0;
}


void SortedTargetingSectors::add(TargetingSector *targetingSector)
{
	const double right = targetingSector->getRight();
	const int count_orderByRight = this->orderByRight.count();

	int index = 0;

	for (index = 0; index < count_orderByRight; index++) {
		if (right < this->orderByRight[index]->getRight())
			break;
	}

	this->orderByRight.insert(index, targetingSector);



	const double distance = targetingSector->getDistance();
	const int count_orderByDistance = this->orderByDistance.count();

	for (index = 0; index < count_orderByDistance; index++) {
		if (distance < this->orderByDistance[index]->getDistance())
			break;
	}

	this->orderByDistance.insert(index, targetingSector);
}


void SortedTargetingSectors::removeSectorsWhichRightCoordIsLessOrEqualThan(double right)
{
	while (this->orderByRight.count() > 0)
	{
		TargetingSector *targetingSector = this->orderByRight.at(0);
		if (targetingSector->getRight() > right)
			break;
		this->removeSector(this->orderByRight.at(0));
	}
}


void SortedTargetingSectors::removeSector(TargetingSector *targetingSector)
{
	this->removeElement(&this->orderByRight, targetingSector);
	this->removeElement(&this->orderByDistance, targetingSector);
}


void SortedTargetingSectors::removeElement(QVector<TargetingSector*> *vector, TargetingSector *targetingSector)
{
	vector->removeAt(vector->indexOf(targetingSector));
}

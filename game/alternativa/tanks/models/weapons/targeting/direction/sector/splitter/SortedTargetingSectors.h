#ifndef SORTEDTARGETINGSECTORS_H
#define SORTEDTARGETINGSECTORS_H

#include <QVector>

class TargetingSector;

class SortedTargetingSectors
{
	QVector<TargetingSector*> orderByRight;
	QVector<TargetingSector*> orderByDistance;

public:
	void clear();
	QVector<TargetingSector *> *getOrderedByDistance();
	double getRight();
	bool isEmpty();
	void add(TargetingSector *targetingSector);
	void removeSectorsWhichRightCoordIsLessOrEqualThan(double);

private:
	void removeSector(TargetingSector *targetingSector);
	void removeElement(QVector<TargetingSector*>*, TargetingSector*);
};

#endif // SORTEDTARGETINGSECTORS_H

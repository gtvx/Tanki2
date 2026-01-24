#ifndef PROPSTATE_H
#define PROPSTATE_H

#include <QVector>

class PropLOD;
class PropObject;

class PropState
{
	QVector<PropLOD*> _lods;

public:

	static const QString DEFAULT_NAME;

	PropState();

	void addLOD(PropObject *prop, double distance);

	int getNumLODs();

	PropLOD* lodByIndex(int index);

	PropObject* getDefaultObject();
};

#endif // PROPSTATE_H

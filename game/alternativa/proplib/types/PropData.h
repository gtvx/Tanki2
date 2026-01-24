#ifndef PROPDATA_H
#define PROPDATA_H

#include <QMap>

class PropState;

class PropData
{
	QString _name;
	QMap<QString, PropState*> states;

public:
	PropData(const QString &name);
	QString getName();

	void addState(const QString &stateName, PropState *state);

	PropState* getStateByName(const QString &stateName);

	PropState* getDefaultState();
};

#endif // PROPDATA_H

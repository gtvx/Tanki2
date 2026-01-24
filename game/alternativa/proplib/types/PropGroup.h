#ifndef PROPGROUP_H
#define PROPGROUP_H

#include <QVector>

class PropData;

class PropGroup
{
public:
	QString name;
	QVector<PropData*> *props;
	QVector<PropGroup*> *groups;

public:
	PropGroup(const QString &name);

	PropData* getPropByName(const QString &propName);
	PropGroup* getGroupByName(const QString &groupName);
	void addProp(PropData *prop);
	void addGroup(PropGroup *group);
};

#endif // PROPGROUP_H

#ifndef PROPLIBREGISTRY_H
#define PROPLIBREGISTRY_H

#include <QMap>

class PropLibrary;

class PropLibRegistry
{
	QMap<QString, PropLibrary*> libs;

public:
	PropLibRegistry();

	void addLibrary(PropLibrary *lib);

	PropLibrary* getLibrary(const QString &libName);
};

#endif // PROPLIBREGISTRY_H

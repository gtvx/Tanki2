#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#include "entrance/alternativa/types/Long.h"

class ResourceInfo
{
public:
	ResourceInfo(int type, const Long &id, const Long &version, bool isLazy);
	int type;
	Long id;
	Long version;
	bool isLazy;
	QString toString() const;
};

#endif // RESOURCEINFO_H

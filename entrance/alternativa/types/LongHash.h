#ifndef LONGHASH_H
#define LONGHASH_H

#include "Long.h"
#include <QHash>

static unsigned int qHash(const Long &l)
{
	return qHash(l.low() ^ l.high());
}

#endif // LONGHASH_H

#ifndef SHARED_QHASH_H
#define SHARED_QHASH_H

#include <QHash>
#include "hardware/alternativa/engine3d/core/Vertex.h"

static unsigned int qHash(const Shared<Vertex> &s)
{
	return qHash(s.get());
}

#endif // SHARED_QHASH_H

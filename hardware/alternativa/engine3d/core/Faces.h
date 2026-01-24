#ifndef FACES_H
#define FACES_H

#include <QVector>
#include "Face.h"

struct Faces
{
	QVector<Shared<Face>> *faces;
};

#endif // FACES_H

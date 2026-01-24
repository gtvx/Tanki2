#ifndef WRAPPER_H
#define WRAPPER_H

#include "shared.h"

class Vertex;

class Wrapper : public SharedObject
{
	Wrapper();
public:

	Shared<Wrapper> next;
	Shared<Vertex> vertex;

	static Shared<Wrapper> create();
	void remove();
};

#endif // WRAPPER_H

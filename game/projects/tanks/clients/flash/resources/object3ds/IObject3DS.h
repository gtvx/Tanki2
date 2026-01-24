#ifndef IOBJECT3DS_H
#define IOBJECT3DS_H

class Tanks3DSResource;

class IObject3DS
{
public:
	virtual Tanks3DSResource* getResource3DS() = 0;
};

#endif // IOBJECT3DS_H

#ifndef IRAYCOLLISIONFILTER_H
#define IRAYCOLLISIONFILTER_H

class Body;

class IRayCollisionFilter
{
public:
	virtual bool considerBody(Body*) = 0;
};

#endif // IRAYCOLLISIONFILTER_H

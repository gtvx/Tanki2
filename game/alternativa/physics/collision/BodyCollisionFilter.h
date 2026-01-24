#ifndef BODYCOLLISIONFILTER_H
#define BODYCOLLISIONFILTER_H

class Body;

class BodyCollisionFilter
{
public:
	virtual bool considerBodies(Body *param1, Body *param2) = 0;
};

#endif // BODYCOLLISIONFILTER_H

#ifndef ITANKMODEL_H
#define ITANKMODEL_H

class Tank;

class ITankModel
{
public:
	virtual Tank* getTank() = 0;
	virtual bool isLocal() = 0;
};

#endif // ITANKMODEL_H

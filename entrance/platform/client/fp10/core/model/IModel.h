#ifndef IMODEL_H
#define IMODEL_H

class ProtocolBuffer;
class Long;
class Class;

class IModel
{
public:
	virtual void invoke(Long*, ProtocolBuffer*) = 0;
	virtual Long* getId() = 0;
	//virtual void putInitParams(void*) = 0;
	virtual void clearInitParams() = 0;
	//virtual void* getData(Class*) = 0;
	//virtual void putData(Class*, void*) = 0;
	//virtual void* clearData(Class*) = 0;
};

#endif // IMODEL_H

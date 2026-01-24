#ifndef DEFERREDACTION_H
#define DEFERREDACTION_H

class DeferredAction
{
public:
	virtual ~DeferredAction() {}
	virtual void execute() = 0;
};

#endif // DEFERREDACTION_H

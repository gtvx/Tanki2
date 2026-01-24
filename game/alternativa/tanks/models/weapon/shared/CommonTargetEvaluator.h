#ifndef COMMONTARGETEVALUATOR_H
#define COMMONTARGETEVALUATOR_H

class Body;

class CommonTargetEvaluator
{
public:
	virtual double getTargetPriority(Body*, double, double, double, double) = 0;
};

#endif // COMMONTARGETEVALUATOR_H

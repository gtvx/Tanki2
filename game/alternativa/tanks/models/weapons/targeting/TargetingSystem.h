#ifndef TARGETINGSYSTEM_H
#define TARGETINGSYSTEM_H

class TargetingDirectionCalculator;
class TargetingPriorityCalculator;
class TargetingDirectionProcessor;
class TargetingResult;
class AllGlobalGunParams;

class TargetingSystem
{
	TargetingDirectionCalculator *directionCalculator;
	TargetingPriorityCalculator *priorityCalculator;
	TargetingDirectionProcessor *directionProcessor;
	TargetingResult *result;

public:
	void init(TargetingDirectionCalculator *directionCalculator, TargetingDirectionProcessor *directionProcessor, TargetingPriorityCalculator *priorityCalculator);
	TargetingDirectionProcessor *getProcessor();
	TargetingResult *target(AllGlobalGunParams*);
};

#endif // TARGETINGSYSTEM_H

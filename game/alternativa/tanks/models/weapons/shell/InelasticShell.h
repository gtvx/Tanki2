#ifndef INELASTICSHELL_H
#define INELASTICSHELL_H

#include "Shell.h"

class Body;

class InelasticShell : public Shell
{
public:
	InelasticShell(Pool *pool);

protected:
	void update(double _arg_1);
	void processHitImpl(Body *_arg_1, const Vector3 *_arg_2, const Vector3 *_arg_3, double _arg_4, int _arg_5) override;
	void updateTotalDistance(double _arg_1);
	void handleFlightFinish();
	bool isFlightFinished();
	virtual double getMaxDistance();
	virtual double getSpeed();
	void updatePosition(double _arg_1);
};

#endif // INELASTICSHELL_H

#ifndef SUSPENSIONPARAMS_H
#define SUSPENSIONPARAMS_H

struct SuspensionParams
{
	SuspensionParams();
	double maxRayLength;
	double nominalRayLength;
	double springCoeff;
	double dampingCoeff;
};

#endif // SUSPENSIONPARAMS_H

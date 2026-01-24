#ifndef ROTATIONSTATE_H
#define ROTATIONSTATE_H

class RotationState
{
	const char *_state;

public:
	static const RotationState *WITHOUT_ROTATION;
	static const RotationState *USE_RANDOM_ROTATION;

	RotationState(const char *state);
};

#endif // ROTATIONSTATE_H

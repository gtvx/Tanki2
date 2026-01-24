#include "RotationState.h"

const RotationState *RotationState::WITHOUT_ROTATION = new RotationState("without_rotation");
const RotationState *RotationState::USE_RANDOM_ROTATION = new RotationState("use_random_rotation");

RotationState::RotationState(const char *state)
{
	this->_state = state;
}

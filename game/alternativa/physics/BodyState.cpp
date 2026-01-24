#include "BodyState.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Quaternion.h"


BodyState::BodyState()
{
	velocity.reset();
	angularVelocity.reset();
	position.reset();
	orientation.x = 0;
	orientation.y = 0;
	orientation.z = 0;
	orientation.w = 1;
}


void BodyState::copy(const BodyState *state)
{
	this->position.copy(&state->position);
	this->orientation.copy(&state->orientation);
	this->velocity.copy(&state->velocity);
	this->angularVelocity.copy(&state->angularVelocity);
}


bool BodyState::isValid() const
{
	return this->velocity.isFiniteVector() &&
			this->angularVelocity.isFiniteVector() &&
			this->position.isFiniteVector() &&
			this->orientation.isFiniteQuaternion();
}

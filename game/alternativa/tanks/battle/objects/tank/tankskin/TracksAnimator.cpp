#include "TracksAnimator.h"
#include "../tankchassis/Track.h"
#include "../tankchassis/TrackedChassis.h"
#include "TankSkin.h"
#include "game/alternativa/physics/Body.h"
#include "../tankchassis/SuspensionRay.h"
#include "MyMath.h"
#include "game/alternativa/tanks/utils/MathUtils.h"
#include "../ValueSmoother.h"


static const double MIN_TRACK_SPEED = 100;



TracksAnimator::TracksAnimator(TrackedChassis *chassis,
							   TankSkin *skin,
							   ValueSmoother *maxSpeedSmoother)
{
	this->chassis = chassis;
	this->skin = skin;
	this->maxSpeedSmoother = maxSpeedSmoother;
}


void TracksAnimator::animate(double value)
{
	this->calculateTracksAnimationSpeed(value);
	this->skin->updateTracks((value * this->chassis->leftTrack->animationSpeed), (value * this->chassis->rightTrack->animationSpeed));
}


void TracksAnimator::calculateTracksAnimationSpeed(double value)
{
	this->calculateTrackAnimationSpeed(this->chassis->leftTrack, value);
	this->calculateTrackAnimationSpeed(this->chassis->rightTrack, value);
}


void TracksAnimator::calculateTrackAnimationSpeed(Track *track, double value)
{
	if (this->hasCorrectContacts(track))
	{
		this->animateTrackWithContacts(track, value);
	}
	else
	{
		this->animateTrackWithoutContacts(track, value);
	}
}


bool TracksAnimator::hasCorrectContacts(Track *track)
{
	return (track->body->baseMatrix.m22 > 0) && (track->numContacts > 0);
}


void TracksAnimator::animateTrackWithContacts(Track *track, double value)
{
	double _local_3 = this->getTrackSpeed(track);
	if (this->requiresSynchronizedAnimation(track, _local_3))
	{
		track->animationSpeed = _local_3;
	}
	else
	{
		double _local_4 = (this->getDesiredSpeedCoeff(track) * MIN_TRACK_SPEED);
		track->setAnimationSpeed(_local_4, (this->chassis->getAcceleration() * value));
	}
}


double TracksAnimator::getTrackSpeed(Track *track)
{
	const Vector3 &_local_2 = track->averageSurfaceVelocity;
	SuspensionRay *_local_3 = track->rays->at(track->numRays >> 1);
	Vector3 _bodyPointVelocity;
	this->getBodyPointVelocity(track->body, _local_3->getGlobalOrigin(), &_bodyPointVelocity);
	double _local_4 = (_bodyPointVelocity.x - _local_2.x);
	double _local_5 = (_bodyPointVelocity.y - _local_2.y);
	double _local_6 = (_bodyPointVelocity.z - _local_2.z);
	const Matrix3 &_local_7 = track->body->baseMatrix;
	return (((_local_4 * _local_7.m01) + (_local_5 * _local_7.m11)) + (_local_6 * _local_7.m21));
}


void TracksAnimator::getBodyPointVelocity(Body *body, const Vector3 *_arg_2, Vector3 *_arg_3)
{
	const Vector3 &_local_4 = body->state.position;
	double _local_5 = (_arg_2->x - _local_4.x);
	double _local_6 = (_arg_2->y - _local_4.y);
	double _local_7 = (_arg_2->z - _local_4.z);
	const Vector3 &_local_8 = body->state.angularVelocity;
	_arg_3->x = ((_local_8.y * _local_7) - (_local_8.z * _local_6));
	_arg_3->y = ((_local_8.z * _local_5) - (_local_8.x * _local_7));
	_arg_3->z = ((_local_8.x * _local_6) - (_local_8.y * _local_5));
	const Vector3 &_local_9 = body->state.velocity;
	_arg_3->x = (_arg_3->x + _local_9.x);
	_arg_3->y = (_arg_3->y + _local_9.y);
	_arg_3->z = (_arg_3->z + _local_9.z);
}


bool TracksAnimator::requiresSynchronizedAnimation(Track *track, double value)
{
	double _local_3 = this->getDesiredSpeedCoeff(track);
	return (((MyMath::abs(value) > (0.8 * MIN_TRACK_SPEED)) || (_local_3 == 0)) || ((MathUtils::numberSign(value, 1) * MathUtils::sign(_local_3)) == -1));
}


double TracksAnimator::getDesiredSpeedCoeff(Track *track)
{
	int _local_2 = this->chassis->getActualMovementDirection();
	int _local_3 = this->chassis->getActualTurnDirection();
	double _local_4 = 0;
	if (_local_2 == 0)
	{
		_local_4 = ((track->side * _local_3) * 0.5);
	}
	else
	{
		if (_local_3 == 0)
		{
			_local_4 = _local_2;
		}
		else
		{
			_local_4 = ((_local_2 * (3 + (track->side * _local_3))) / 4);
		}
	}
	return _local_4;
}


void TracksAnimator::animateTrackWithoutContacts(Track *track, double value)
{
	double v = this->getDesiredSpeedCoeff(track);
	track->setAnimationSpeed(v * this->maxSpeedSmoother->getTargetValue(), this->chassis->getAcceleration() * value);
}


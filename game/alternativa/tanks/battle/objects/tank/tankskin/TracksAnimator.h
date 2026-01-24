#ifndef TRACKSANIMATOR_H
#define TRACKSANIMATOR_H

class TrackedChassis;
class TankSkin;
class ValueSmoother;
class Track;
class Body;
class Vector3;


class TracksAnimator
{
	TrackedChassis *chassis;
	TankSkin *skin;
	ValueSmoother *maxSpeedSmoother;

public:
	TracksAnimator(TrackedChassis *chassis,
				   TankSkin *skin,
				   ValueSmoother *maxSpeedSmoother);

	void animate(double value);

private:
	void calculateTracksAnimationSpeed(double value);
	void calculateTrackAnimationSpeed(Track *track, double value);
	bool hasCorrectContacts(Track *track);
	void animateTrackWithContacts(Track *track, double value);
	double getTrackSpeed(Track *track);
	void getBodyPointVelocity(Body *_arg_1, const Vector3 *_arg_2, Vector3 *_arg_3);
	bool requiresSynchronizedAnimation(Track *track, double value);
	double getDesiredSpeedCoeff(Track *track);
	void animateTrackWithoutContacts(Track *track, double value);
};

#endif // TRACKSANIMATOR_H

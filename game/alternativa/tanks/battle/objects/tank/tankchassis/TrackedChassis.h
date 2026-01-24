#ifndef TRACKEDCHASSIS_H
#define TRACKEDCHASSIS_H

class Track;
class Body;
class SuspensionParams;
class ValueSmoother;
class Vector3;
class EncryptedNumber;
class SuspensionRay;

class TrackedChassis
{
	Body *body;
	SuspensionParams *suspensionParams;
	ValueSmoother *maxSpeedSmoother;
	EncryptedNumber *_acceleration;
	EncryptedNumber *_reverseAcceleration;
	EncryptedNumber *_sideAcceleration;
	EncryptedNumber *_turnAcceleration;
	EncryptedNumber *_reverseTurnAcceleration;
	EncryptedNumber *_stabilizationAcceleration;

public:
	static const int TURN_SPEED_COUNT;

	Track *leftTrack;
	Track *rightTrack;
	int movementDirection;
	int turnDirection;
	bool inverseBackTurnMovement;
	int turnSpeedNumber;


	TrackedChassis(Body *body, SuspensionParams *suspensionParams, ValueSmoother *maxSpeedSmoother, Vector3 *vector3);

	void applyForces(double param1, double param2, double param3);
	void setAcceleration(double value);
	void setReverseAcceleration(double value);
	void setSideAcceleration(double value);
	void setTurnAcceleration(double value);
	void setReverseTurnAcceleration(double value);
	void setStabilizationAcceleration(double value);
	void calculateSurfaceVelocities(Vector3 *param1, Vector3 *param2);
	void setTracksCollisionGroup(int param1);
	double getAcceleration();
	int getActualMovementDirection();
	int getActualTurnDirection();

private:
	void createTracks(int param1, Vector3 *param2);
	void applySlopeHack();
	void applyMovementForces(double param1, double param2, double param3);
	void doApplyMovementForces(double param1, double param2, double param3);
	void calculateSuspensionContacts(double value);
	void adjustSuspensionSpringCoeff();
	void applyForceFromRay(SuspensionRay *param1, Vector3 *param2, double param3, double param4, double param5);
	bool isReversedTurn(int param1, double param2, int param3, bool param4);
	void addVelocitiesFromRay(SuspensionRay *param1, Vector3 *param2, Vector3 *param3, Vector3 *param4);
};

#endif // TRACKEDCHASSIS_H

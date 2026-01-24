#ifndef TANK_H
#define TANK_H

#include <QVector>
#include "game/alternativa/physics/BodyState.h"
#include "game/alternativa/math/Matrix4.h"
#include "game/alternativa/math/Quaternion.h"
#include "game/alternativa/math/Vector3.h"
#include "WeaponPlatform.h"
#include "game/alternativa/tanks/battle/PhysicsController.h"
#include "game/alternativa/tanks/battle/PhysicsInterpolator.h"
#include "ClientTankState.h"
#include "game/alternativa/tanks/camera/CameraTarget.h"
#include "game/alternativa/tanks/battle/PostPhysicsController.h"
#include "MyList.h"
#include <memory>


class Track;
class TrackedChassis;
class WeaponMount;
class CollisionBox;
class ValueSmoother;
class SuspensionParams;
struct IGameObject;
struct TankSoundEffects;
struct TankSkin;
struct Weapon;
struct UserTitle;
struct BattleEventDispatcher;
class TankBody;
class Vector3d;
class BattleService;
class HullTransformUpdater;
class BattleTeam;
class Body;
class ChassisController;
class AllGlobalGunParams;
class LocalTurretController;
class TracksAnimator;


class Tank : public std::enable_shared_from_this<Tank>,
		public virtual WeaponPlatform,
		public virtual PhysicsController,
		public virtual PhysicsInterpolator,
		public virtual CameraTarget,
		public virtual PostPhysicsController
{
	BodyState savedBodyState;
	TrackedChassis *chassis;
	ValueSmoother *maxSpeedSmoother;
	SuspensionParams *suspensionParams;
	TankBody *tankBody;
	ValueSmoother *maxTurnSpeedSmoother;
	TankSkin *skin;
	QVector<Vector3*> *visibilityPoints;
	WeaponMount *_weaponMount;
	Weapon *weapon;
	BattleService *battleService;
	HullTransformUpdater *hullTransformUpdater;
	ChassisController *chassisController;
	TracksAnimator *tracksAnimator;

	double health;
	double halfLength;
	double boundSphereRadius;

	int collisionCount;

public:
	Tank(IGameObject *user,
		 double mass,
		 double damping,
		 TankSoundEffects *sounds,
		 TankSkin *skin,
		 WeaponMount *weaponMount,
		 Weapon *weapon,
		 UserTitle *title,
		 BattleEventDispatcher *battleEventDispatcher,
		 int maxHealth);

	bool hasTracksContactsWithStatic();
	Track *getLeftTrack();
	Track *getRightTrack();
	void setMaxSpeed(double speed, bool b);
	void setMaxTurnSpeed(double speed, bool b);
	void setAcceleration(double acceleration);
	WeaponMount *getWeaponMount();
	void setReverseAcceleration(double reverseAcceleration);
	void setSideAcceleration(double sideAcceleration);
	void setTurnAcceleration(double turnAcceleration);
	void setReverseTurnAcceleration(double reverseTurnAcceleration);
	void setStabilizationAcceleration(double turnStabilizationAcceleration);

	QVector<CollisionBox*> turretCollisions;
	Vector3 interpolatedPosition;
	ClientTankState state;
	void setPhysicsState(const Vector3d *position, const Vector3d *orientation, const Vector3d *velocity, const Vector3d *angularVelocity);
	void addToBattle(BattleService *battleService);
	void runBeforePhysicsUpdate(double);
	void interpolatePhysicsState(double, int );
	void validateBodyState();
	void setHullTransformUpdater(HullTransformUpdater *);
	TankSkin *getSkin();

	Matrix4 interpolatedTransform;
	Quaternion interpolatedOrientation;
	Vector3 skinCenterOffset;

	IGameObject *user;

	void updatePhysicsState();
	void setActivatedState();
	void setBodyCollisionGroup(int);
	void setTracksCollisionGroup(int);
	int getBodyCollisionGroup();
	void spawn(BattleTeam const* team, int incarnation);
	TankBody *getTankBody();
	Body *getBody();


	CollisionBox *getMainCollisionBox();
	void setMovementParams(int, int, int, bool);

	void setChassisController(ChassisController *chassisController);
	ChassisController* getChassisController();

	void getAllGunParams(AllGlobalGunParams*, int = 0);
	void getCameraParams(Vector3*, Vector3*);
	double getInterpolatedTurretWorldDirection();
	void render(int, int);
	virtual void runAfterPhysicsUpdate(double);


	void setLocalTurretController(LocalTurretController *localTurretController);
	LocalTurretController *getLocalTurretController();

	virtual Vector3 *getLocalMuzzlePosition(int=0);
	virtual Vector3* getLaserLocalPosition();
	virtual double getBarrelLength(int=0);
	virtual std::shared_ptr<Object3D> getTurret3D();
	virtual void stopMovement();
	virtual void lockMovement(bool);
	virtual int getNumberOfBarrels();
	virtual void enableTurretSound(bool);
	virtual void showTitle();
	virtual void addDust(int=7);
	virtual BattleTeam* get_teamType();
	virtual void getBasicGunParams(BasicGlobalGunParams*, int=0);


	void applyWeaponHit(const Vector3*, const Vector3*, double);
	bool isImpactEnabled();
	void setLastHitPoint(const Vector3*);

	Vector3 lastHitPoint;
	bool isLastHitPointSet;
	double getBoundSphereRadius();

	void setHealth(double);
	double getHealth();

	Weapon *getWeapon();

	void destroy();
	void removeFromBattle();



private:
	void createBody(double mass, Vector3*);
	void calculateSkinCenterOffset(Vector3*);
	void createCollisionPrimitives(Vector3*);
	void setBoundSphereRadius(Vector3*, double);
	void createVisibilityPoints(Vector3*);
	void updateTurretPhysics(double);
	void initTurretGeometry(TankSkin*);
	CollisionBox* createTurretCollision(const Vector3 *);
	void rotateTurret(double);
	void ensureBodyStateIsValid();
	LocalTurretController *localTurretController;
};

#endif // TANK_H

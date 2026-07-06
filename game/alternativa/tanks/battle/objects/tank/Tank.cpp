#include "Tank.h"
#include "game/alternativa/tanks/battle/objects/tank/tankchassis/TrackedChassis.h"
#include "game/alternativa/tanks/battle/objects/tank/tankchassis/Track.h"
#include "EncryptedValueSmoother.h"
#include "game/alternativa/tanks/battle/objects/tank/tankchassis/SuspensionParams.h"
#include "ClientTankState.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "game/alternativa/physics/PhysicsUtils.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankHullSkinCacheItem.h"
#include "TankConst.h"
#include "CollisionBoxesBuilder.h"
#include "MyMath.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretGeometryItem.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "Weapon.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/physics/TankBodyIdProvider.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "HullTransformUpdater.h"
#include "WeaponMount.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/tanks/models/weapon/WeaponUtils.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/turret/TurretSkinCacheItem.h"
#include "entrance/projects/tanks/client/battleservice/model/battle/team/BattleTeam.h"
#include "game/alternativa/tanks/camera/FollowCameraController.h"
#include "Error.h"
#include "tankskin/TracksAnimator.h"


static Vector3* calculateSizeForMesh(Mesh *mesh)
{
	return new Vector3(mesh->bound.boundMaxX - mesh->bound.boundMinX,
					   mesh->bound.boundMaxY - mesh->bound.boundMinY,
					   mesh->bound.boundMaxZ - mesh->bound.boundMinZ);
}


Tank::Tank(IGameObject *user,
		   double mass,
		   double damping,
		   TankSoundEffects*,
		   TankSkin *skin,
		   WeaponMount *weaponMount,
		   Weapon *weapon,
		   UserTitle*,
		   BattleEventDispatcher*,
		   int health)
{
	this->user = user;
	this->battleService = nullptr;
	this->hullTransformUpdater = nullptr;
	this->collisionCount = 0;
	this->isLastHitPointSet = false;
	//this->local = local;
	this->health = health;

	this->maxSpeedSmoother = new EncryptedValueSmoother(100,1000,0,0);
	this->maxTurnSpeedSmoother = new EncryptedValueSmoother(0.3,10,0,0);
	this->suspensionParams = new SuspensionParams();

	//this->lastHitPoint = new Vector3();
	//this->user = user;
	this->skin = skin;
	//this->sounds = sounds;
	//this->battleEventDispatcher = battleEventDispatcher;
	//this->maxHealth = maxHealth;
	this->suspensionParams->dampingCoeff = damping;
	//param4.setTank(this);
	Vector3 *_loc11_ = calculateSizeForMesh(skin->getHullDescriptor()->mesh.get());
	Vector3 *_loc12_ = new Vector3(_loc11_->getX() / 2, _loc11_->getY() / 2,_loc11_->getZ() / 2);
	this->halfLength = _loc12_->getY();
	this->calculateSkinCenterOffset(_loc11_);
	this->createBody(mass,_loc12_);
	this->createCollisionPrimitives(_loc12_);
	this->createVisibilityPoints(_loc12_);
	this->initTurretGeometry(skin);
	this->chassis = new TrackedChassis(this->tankBody->body, this->suspensionParams, this->maxSpeedSmoother, _loc11_);
	this->tracksAnimator = new TracksAnimator(this->chassis, skin, this->maxSpeedSmoother);
	this->state = ClientTankState::ACTIVE;
	//this->bodyStateValidator = new BodyPhysicsStateValidator(this->tankBody.body,param9);
	this->_weaponMount = weaponMount;
	this->weapon = weapon;
	this->weapon->init(this);
	//this->title = title;
	//logService = LogService(OSGi.getInstance().getService(LogService));
	this->tankBody->onTankInited();
}


bool Tank::hasTracksContactsWithStatic()
{
	return this->getLeftTrack()->hasContactsWithStatic() || this->getRightTrack()->hasContactsWithStatic();
}


Track* Tank::getLeftTrack()
{
	return this->chassis->leftTrack;
}


Track* Tank::getRightTrack()
{
	return this->chassis->rightTrack;
}


void Tank::setMaxSpeed(double speed, bool b)
{
	if (b) {
		this->maxSpeedSmoother->reset(speed);
	} else {
		this->maxSpeedSmoother->setTargetValue(speed);
	}
}


void Tank::setMaxTurnSpeed(double speed, bool b)
{
	if (b) {
		this->maxTurnSpeedSmoother->reset(speed);
	} else {
		this->maxTurnSpeedSmoother->setTargetValue(speed);
	}
}


void Tank::setAcceleration(double acceleration)
{
	this->chassis->setAcceleration(acceleration);
}


WeaponMount* Tank::getWeaponMount()
{
	return this->_weaponMount;
}


Weapon* Tank::getWeapon()
{
	return this->weapon;
}


void Tank::setReverseAcceleration(double reverseAcceleration)
{
	this->chassis->setReverseAcceleration(reverseAcceleration);
}


void Tank::setSideAcceleration(double sideAcceleration)
{
	this->chassis->setSideAcceleration(sideAcceleration);
}


void Tank::setTurnAcceleration(double turnAcceleration)
{
	this->chassis->setTurnAcceleration(turnAcceleration);
}


void Tank::setReverseTurnAcceleration(double reverseTurnAcceleration)
{
	this->chassis->setReverseTurnAcceleration(reverseTurnAcceleration);
}


void Tank::setStabilizationAcceleration(double turnStabilizationAcceleration)
{
	this->chassis->setStabilizationAcceleration(turnStabilizationAcceleration);
}


void Tank::createBody(double mass, Vector3 *vector)
{
	Body *body = new Body(mass, &Matrix3::IDENTITY, 0);
	PhysicsUtils::setBoxInvInertia(mass, vector, &body->invInertia);
	body->tank = this;
	this->tankBody = new TankBody(body);
}


void Tank::calculateSkinCenterOffset(Vector3 *param1)
{
	std::shared_ptr<Mesh> mesh = this->skin->getHullMesh();
	mesh->calculateBounds();
	this->skinCenterOffset.setX(-0.5 * (mesh->bound.boundMinX + mesh->bound.boundMaxX));
	this->skinCenterOffset.setY(-0.5 * (mesh->bound.boundMinY + mesh->bound.boundMaxY));
	this->skinCenterOffset.setZ(-0.5 * param1->getZ() - this->suspensionParams->nominalRayLength + TankConst::RAY_OFFSET);
}


void Tank::createCollisionPrimitives(Vector3 *param1)
{
	double _loc2_ = (2 * param1->getZ()) - (this->suspensionParams->nominalRayLength - TankConst::RAY_OFFSET);
	CollisionBoxesBuilder::createTankCollisionBox(param1, _loc2_, this->tankBody);
	CollisionBoxesBuilder::createStaticCollisionBoxes(param1, _loc2_, this->tankBody);
	this->setBoundSphereRadius(param1, _loc2_);
}


void Tank::setBoundSphereRadius(Vector3 *param1, double param2)
{
	Vector3 *_loc3_ = new Vector3(param1->getX(), param1->getY(), param2 / 2);
	Matrix4 *_loc4_ = this->tankBody->tankCollisionBox->localTransform;
	this->boundSphereRadius = _loc3_->length() + MyMath::abs(_loc4_->m23);
}


void Tank::createVisibilityPoints(Vector3 *param1)
{
	double _loc2_ = param1->getX();
	double _loc3_ = param1->getY();
	this->visibilityPoints = new QVector<Vector3*>;
	this->visibilityPoints->append(new Vector3(-_loc2_,_loc3_,0));
	this->visibilityPoints->append(new Vector3(_loc2_,_loc3_,0));
	this->visibilityPoints->append(new Vector3(-_loc2_,0,0));
	this->visibilityPoints->append(new Vector3(_loc2_,0,0));
	this->visibilityPoints->append(new Vector3(-_loc2_,-_loc3_,0));
	this->visibilityPoints->append(new Vector3(_loc2_,-_loc3_,0));
}


void Tank::initTurretGeometry(TankSkin *param1)
{
	QVector<TurretGeometryItem*> *vector = param1->getTurretGeometry();
	for (int i = 0; i < vector->count(); i++)
	{
		TurretGeometryItem *turretgeometryitem = vector->at(i);
		CollisionBox *box = this->createTurretCollision(turretgeometryitem->getHalfSize());
		this->turretCollisions.append(box);

		this->tankBody->body->addCollisionShape(box, new Matrix4());
	}
	this->updateTurretPhysics(0);
}


void Tank::updateTurretPhysics(double param1)
{
	TankHullSkinCacheItem *_loc2_ = this->skin->getHullDescriptor();
	QVector<TurretGeometryItem*> *_loc3_ = this->skin->getTurretGeometry();
	for (int i = 0; i < this->turretCollisions.count(); i++)
	{
		CollisionBox *_loc5_ = this->turretCollisions[i];
		Matrix4 *_loc6_ = _loc5_->localTransform;

		_loc6_->setMatrix(_loc2_->getTurretMountPointX() + this->skinCenterOffset.getX(),
						  _loc2_->getTurretMountPointY() + this->skinCenterOffset.getY(),
						  _loc2_->getTurretMountPointZ() + this->skinCenterOffset.getZ(),
						  0, 0, param1);

		TurretGeometryItem *_loc7_ = _loc3_->at(i);
		_loc6_->prepend(_loc7_->getTransform());
	}
}


CollisionBox* Tank::createTurretCollision(const Vector3 *param1)
{
	return new CollisionBox(param1, CollisionGroup::EMPTY, PhysicsMaterial::DEFAULT_MATERIAL);
}


void Tank::setPhysicsState(const Vector3d *position, const Vector3d *orientation, const Vector3d *velocity, const Vector3d *angularVelocity)
{
	Body *body = this->tankBody->body;
	BodyState *bodyState = &body->state;
	BattleUtils::copyToVector3(position, &bodyState->position);

	bodyState->orientation.setFromEulerAnglesXYZ(orientation->getX(), orientation->getY(), orientation->getZ());

	BattleUtils::copyToVector3(velocity, &bodyState->velocity);
	BattleUtils::copyToVector3(angularVelocity, &bodyState->angularVelocity);
	body->saveState();
	body->calcDerivedData();
	//this->bodyStateValidator->refresh();


	this->interpolatedTransform.null();
	this->tankBody->body->interpolate(1, &this->interpolatedPosition, &this->interpolatedOrientation);
	this->interpolatedOrientation.normalize();
	this->interpolatedOrientation.toMatrix4(&this->interpolatedTransform);
	this->interpolatedTransform.setPosition(&this->interpolatedPosition);

	this->hullTransformUpdater->update(0);
	this->skin->updateTurretTransform(this->_weaponMount->getTurretInterpolatedDirection(), this->_weaponMount->getBarrelInterpolatedElevation());
}


void Tank::addToBattle(BattleService *battleService)
{
	if (this->battleService == nullptr)
	{
		this->battleService = battleService;
		this->tankBody->id = TankBodyIdProvider::claimId();
		BattleRunner *_loc2_ = battleService->getBattleRunner();
		_loc2_->addBodyWrapper(this->tankBody);
		_loc2_->addPhysicsController(this);
		_loc2_->addPostPhysicsController(this);
		_loc2_->addPhysicsInterpolator(this);
		this->skin->addToScene();
		//this->title->addToContainer();
		BattleScene3D *battleScene3D = battleService->getBattleScene3D();
		battleScene3D->addTank(this);
		//_loc2_->getSoundManager()->addEffect(this->sounds);
		this->hullTransformUpdater->reset();
		//battleScene3D->getDustEngine()->addTank(this);
	}
}


void Tank::removeFromBattle()
{
	//this->battleService->getBattleScene3D().getDustEngine().removeTank(this);
	BattleRunner *battleRunner = this->battleService->getBattleRunner();
	BattleScene3D *battleScene3D = this->battleService->getBattleScene3D();
	battleRunner->removeBodyWrapper(this->tankBody);
	battleRunner->removePhysicsController(this);
	battleRunner->removePostPhysicsController(this);
	battleRunner->removePhysicsInterpolator(this);
	TankBodyIdProvider::releaseId(this->tankBody->id);
	this->skin->removeFromScene();
	//this->title.removeFromContainer();
	//if (this->floatingTextEffect != nullptr)
	//this->floatingTextEffect.kill();

	battleScene3D->removeTank(this);
	//battleRunner->getSoundManager().removeEffect(this->sounds);
	//this->temperature = 0;
	this->tankBody->body->clearAccumulators();
	this->_weaponMount->reset();
	this->weapon->deactivate();
	this->weapon->reset();
}


void Tank::runBeforePhysicsUpdate(double param1)
{
	this->collisionCount = 0;
	double maxSpeed = this->maxSpeedSmoother->update(param1);
	double maxTurnSpeed = this->maxTurnSpeedSmoother->update(param1);
	this->tankBody->body->setMaxSpeedXY(maxSpeed);
	this->chassis->applyForces(maxSpeed, maxTurnSpeed, param1);
	this->tankBody->body->slipperyMode = !this->hasTracksContactsWithStatic() && this->tankBody->isSoaring();
	this->rotateTurret(param1);
	this->validateBodyState();
}


void Tank::interpolatePhysicsState(double param1, int param2)
{
	this->interpolatedTransform.null();
	this->tankBody->body->interpolate(param1, &this->interpolatedPosition, &this->interpolatedOrientation);
	this->interpolatedOrientation.normalize();
	this->interpolatedOrientation.toMatrix4(&this->interpolatedTransform);
	this->interpolatedTransform.setPosition(&this->interpolatedPosition);
	this->_weaponMount->interpolate(param1, param2);
}


void Tank::validateBodyState()
{
	this->savedBodyState.copy(&this->tankBody->body->state);
	//this->bodyStateValidator->validate();
}


void Tank::rotateTurret(double param1)
{
	this->_weaponMount->rotate(param1, &this->tankBody->body->baseMatrix);
	this->updateTurretPhysics(this->_weaponMount->getTurretPhysicsDirection());
	//this->sounds.playTurretSound(this->_weaponMount.isRotating());
}


void Tank::setHullTransformUpdater(HullTransformUpdater *param1)
{
	this->hullTransformUpdater = param1;
}


TankSkin* Tank::getSkin()
{
	return this->skin;
}


void Tank::updatePhysicsState()
{
	this->interpolatePhysicsState(1, 16);
	this->hullTransformUpdater->update(0);
	this->skin->updateTurretTransform(this->_weaponMount->getTurretInterpolatedDirection(), this->_weaponMount->getBarrelInterpolatedElevation());
}


void Tank::setActivatedState()
{
	this->state = ClientTankState::ACTIVE;
	this->setBodyCollisionGroup(CollisionGroup::TANK | CollisionGroup::ACTIVE_TRACK | CollisionGroup::WEAPON);
	this->setTracksCollisionGroup(CollisionGroup::ACTIVE_TRACK);
	//this->skin->setAlpha(1);
	this->tankBody->body->postCollisionFilter = nullptr;
}


void Tank::setBodyCollisionGroup(int param1)
{
	this->tankBody->tankCollisionBox->collisionGroup = param1;

	for (int i = 0; i < turretCollisions.count(); i++)
	{
		 CollisionBox *collisionBox = turretCollisions[i];
		 collisionBox->collisionGroup = param1 | CollisionGroup::WEAPON;
	}
}


void Tank::setTracksCollisionGroup(int param1)
{
	this->chassis->setTracksCollisionGroup(param1);
}


int Tank::getBodyCollisionGroup()
{
	return this->tankBody->tankCollisionBox->collisionGroup;
}


void Tank::spawn(BattleTeam const*, int)
{
	//this->_teamType = team;
	//this->_incarnation = param2;
	//this->title.setTeamType(team);
	//this->sounds.setIdleMode();
	//this->sounds.turretSoundEnabled = true;
	//this->skin->resetColorTransform();
	//this->skin->setNormalState();
	this->maxSpeedSmoother->reset(this->maxSpeedSmoother->getTargetValue());
	this->maxTurnSpeedSmoother->reset(this->maxTurnSpeedSmoother->getTargetValue());
}


TankBody *Tank::getTankBody()
{
	return this->tankBody;
}


Body* Tank::getBody()
{
	return this->tankBody->body;
}


CollisionBox* Tank::getMainCollisionBox()
{
	return this->tankBody->tankCollisionBox;
}


void Tank::setMovementParams(int param1, int param2, int param3, bool param4)
{
	this->chassis->movementDirection = param1;
	this->chassis->turnDirection = param2;
	this->chassis->turnSpeedNumber = param3;
	this->chassis->inverseBackTurnMovement = param4;

	if (param1 != 0)
	{
		//this->sounds.setAccelerationMode();
	}
	else if (param2 != 0)
	{
		//this->sounds.setTurningMode();
	}
	else
	{
		//this->sounds.setIdleMode();
	}
}


void Tank::setChassisController(ChassisController *chassisController)
{
	this->chassisController = chassisController;
}


ChassisController *Tank::getChassisController()
{
	return chassisController;
}


void Tank::getAllGunParams(AllGlobalGunParams *allGunParams, int param2)
{
	WeaponUtils::calculateMainGunParams(this->skin->getBarrel3D().get(),
										this->skin->getTurretDescriptor()->muzzles->at(param2),
										allGunParams);
}


void Tank::getCameraParams(Vector3 *position, Vector3 *rotation)
{
	Matrix3 _m;
	Matrix3 _rm;
	Vector3 _v;

	std::shared_ptr<Object3D> turret3D = this->skin->getTurret3D();

	position->reset(turret3D->x, turret3D->y, turret3D->z + 10);
	if (FollowCameraController::getFollowCameraMode() == FollowCameraController::CAMERA_FOLLOWS_TURRET)
	{
		_m.setRotationMatrix(turret3D->rotationX, turret3D->rotationY, turret3D->rotationZ);
		rotation->reset(_m.m01, _m.m11, _m.m21);
		if (_m.m22 < -0.999)
		{
			rotation->scale(-1);
		}
		else
		{
			double _loc4_ = MyMath::PI / 6;
			double _loc5_ = MyMath::sin(_loc4_);
			if (_m.m22 < _loc5_)
			{
				_v.reset(_m.m02, _m.m12, _m.m22);
				_v.cross(Vector3::Z_AXIS);
				_v.normalize();
				_rm.fromAxisAngle(&_v, _loc4_ - MyMath::asin(_m.m22));
				_m.append(&_rm);
				rotation->reset(_m.m01, _m.m11, _m.m21);
			}
		}
	}
	else
	{
		BattleUtils::fillDirectionVector(rotation, FollowCameraController::getFollowCameraDirection());
	}
}


double Tank::getInterpolatedTurretWorldDirection()
{
	Vector3 _v;
	BattleUtils::fillDirectionVector(&_v, this->_weaponMount->getTurretInterpolatedDirection());
	_v.deltaTransform4(&this->interpolatedTransform);
	return BattleUtils::getDirectionAngle(&_v);
}


void Tank::render(int, int _arg_2)
{
	double _local_3 = (_arg_2 * 0.001);
	this->hullTransformUpdater->update(_local_3);
	this->skin->updateTurretTransform(this->_weaponMount->getTurretInterpolatedDirection(), this->_weaponMount->getBarrelInterpolatedElevation());
	//this->skin.setColorTransformByTemperature(this->temperature);
	this->tracksAnimator->animate(_local_3);

	/*
	Object3D *_local_4 = this->skin.getTurret3D();
	_v.x = _local_4.x;
	_v.y = _local_4.y;
	_v.z = _local_4.z;

	//this->title.setWeaponStatus((100 * this->weapon.getStatus()));
	//this->title.update(_v);
	*/
	/*
	TankHullSkinCacheItem *_local_5 = this->skin.getHullDescriptor();
	if (_local_5.hasIncorrectData())
	{
		this->battleEventDispatcher.dispatchEventOnce(new DataValidationErrorEvent(_local_5.getType()));
	}
	TurretSkinCacheItem *_local_6 = this->skin.getTurretDescriptor();
	if (_local_6.hasIncorrectData())
	{
		this->battleEventDispatcher.dispatchEventOnce(new DataValidationErrorEvent(_local_6.getType()));
	}
	*/
}


void Tank::ensureBodyStateIsValid()
{
	Body *body = this->tankBody->body;
	BodyState *state = &body->state;
	if ((!(state->isValid())))
	{
		state->copy(&this->savedBodyState);
		body->saveState();
	}
}


void Tank::runAfterPhysicsUpdate(double)
{
	this->ensureBodyStateIsValid();
	//this->bodyStateValidator.refresh();
	this->_weaponMount->updatePhysics(this->getBody());
}


void Tank::setLocalTurretController(LocalTurretController *localTurretController)
{
	this->localTurretController = localTurretController;
}


LocalTurretController* Tank::getLocalTurretController()
{
	return this->localTurretController;
}


Vector3 *Tank::getLocalMuzzlePosition(int _arg_1)
{
	return this->skin->getTurretDescriptor()->muzzles->at(_arg_1);
}


Vector3 *Tank::getLaserLocalPosition()
{
	throw Error(139);
}


double Tank::getBarrelLength(int id)
{
	return this->skin->getTurretDescriptor()->muzzles->at(id)->y;
}


std::shared_ptr<Object3D> Tank::getTurret3D()
{
	return this->skin->getTurret3D();
}


void Tank::stopMovement()
{

}


void Tank::lockMovement(bool)
{

}


int Tank::getNumberOfBarrels()
{
	return this->skin->getTurretDescriptor()->muzzles->length();
}


void Tank::enableTurretSound(bool)
{

}


void Tank::showTitle()
{

}


void Tank::addDust(int)
{

}


BattleTeam *Tank::get_teamType()
{
	return nullptr;
}


void Tank::getBasicGunParams(BasicGlobalGunParams *, int)
{
	throw Error(141);
}


void Tank::applyWeaponHit(const Vector3 *a, const Vector3 *b, double c)
{
	if (this->health > 0)
	{
		if (this->isImpactEnabled())
		{
			this->tankBody->body->addWorldForceScaled(a, b, c);
			this->tankBody->additionForcesSum.addScaled(c, b);
		}
		this->setLastHitPoint(a);
	}
}


bool Tank::isImpactEnabled()
{
	/*
	IGameObject *iGameObject = ITankModel(this->user.adapt(ITankModel)).getTankSet().hull;
	if (iGameObject.hasModel(ImpactEnable))
	{
		return (ImpactEnable(iGameObject.adapt(ImpactEnable)).isImpactEnabled());
	}
	*/
	return true;
}


void Tank::setLastHitPoint(const Vector3 *v)
{
	this->lastHitPoint.copy(v);
	BattleUtils::globalToLocal(this->getBody(), &this->lastHitPoint);
	this->isLastHitPointSet = true;
}


double Tank::getBoundSphereRadius()
{
	return this->boundSphereRadius;
}


void Tank::setHealth(double health)
{
	this->health = health;
	if (health == 0) {
		this->state = ClientTankState::DEAD;
	} else {
		this->state = ClientTankState::ACTIVE;
	}
}


double Tank::getHealth()
{
	return this->health;
}


void Tank::destroy()
{
	this->setHealth(0);
	this->user = nullptr;
	this->state = ClientTankState::DEAD;
	//this->skin->dispose();
	this->skin = nullptr;
	//this->weapon->destroy();
	this->tankBody->destroy();
	delete this->tankBody;
}


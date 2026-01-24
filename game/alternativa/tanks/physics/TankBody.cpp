#include "TankBody.h"
#include "game/alternativa/math/Vector3.h"
#include "SoaringChecker.h"


TankBody::TankBody(Body *body)
{
	this->previousUpVelocity = 0;
	this->wasContactWithStatic = false;
	this->body = body;
	this->soaringChecker = nullptr;
	this->id = -1;
}


void TankBody::updateSoaring()
{
	if (soaringChecker != nullptr)
		this->soaringChecker->updateSoaring();
}


bool TankBody::hasContactsWithOtherBodies()
{
	return this->penetratedBodies.count() > 0;
}


bool TankBody::isSoaring()
{
	if (this->soaringChecker == nullptr)
		return false;
	return this->soaringChecker->isSoaring();
}


void TankBody::onTankInited()
{
	this->soaringChecker = new SoaringChecker(this);
}


void TankBody::destroy()
{
	if (this->soaringChecker != nullptr) {
		delete this->soaringChecker;
		this->soaringChecker = nullptr;
	}
	this->body = nullptr;
}

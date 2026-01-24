#ifndef SOARINGCHECKER_H
#define SOARINGCHECKER_H

class Tank;
class CollisionBox;
class TankBody;

class SoaringChecker
{
	Tank *tank;
	CollisionBox *testSoaringCollisionBox;
	bool soaring;
	bool wasSoaring;
	bool staticIsNear;

public:
	SoaringChecker(TankBody *tankBod);
	void updateSoaring();
	bool isSoaring();
	bool isJumpBegin();
	bool isJumpEnd();
	bool isElasticStaticCollisionWhenSoaring();

private:
	void updateSoaringCollisionBox();
};

#endif // SOARINGCHECKER_H

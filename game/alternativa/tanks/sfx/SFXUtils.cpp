#include "SFXUtils.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/math/Matrix3.h"
#include "MyMath.h"

//private static var axis1:Vector3 = new Vector3();
//private static var axis2:Vector3 = new Vector3();
//private static var eulerAngles:Vector3 = new Vector3();
//private static var targetAxisZ:Vector3 = new Vector3();
//private static var objectAxis:Vector3 = new Vector3();
//private static var matrix1:Matrix3 = new Matrix3();
//private static var matrix2:Matrix3 = new Matrix3();




void SFXUtils::alignObjectPlaneToView(Object3D *_arg_1,
									  const Vector3 *_arg_2,
									  const Vector3 *_arg_3,
									  const Vector3 *_arg_4)
{
	double _local_5;

	Vector3 axis1;

	if (((_arg_3->y < -0.9999999) || (_arg_3->y > 0.9999999)))
	{
		axis1.x = 0;
		axis1.y = 0;
		axis1.z = 1;
		_local_5 = ((_arg_3->y < 0) ? MyMath::PI : 0);
	}
	else
	{
		axis1.x = _arg_3->z;
		axis1.y = 0;
		axis1.z = -_arg_3->x;
		axis1.normalize();
		_local_5 = MyMath::acos(_arg_3->y);
	}

	Matrix3 matrix1;


	matrix1.fromAxisAngle(&axis1, _local_5);

	Vector3 targetAxisZ;

	targetAxisZ.x = (_arg_4->x - _arg_2->x);
	targetAxisZ.y = (_arg_4->y - _arg_2->y);
	targetAxisZ.z = (_arg_4->z - _arg_2->z);

	double _local_6 = (((targetAxisZ.x * _arg_3->x) + (targetAxisZ.y * _arg_3->y)) + (targetAxisZ.z * _arg_3->z));
	targetAxisZ.x = (targetAxisZ.x - (_local_6 * _arg_3->x));
	targetAxisZ.y = (targetAxisZ.y - (_local_6 * _arg_3->y));
	targetAxisZ.z = (targetAxisZ.z - (_local_6 * _arg_3->z));

	targetAxisZ.normalize();

	Vector3 objectAxis;

	matrix1.transformVector(Vector3::Z_AXIS, &objectAxis);

	_local_6 = (((objectAxis.x * targetAxisZ.x) + (objectAxis.y * targetAxisZ.y)) + (objectAxis.z * targetAxisZ.z));

	Vector3 axis2;
	axis2.x = ((objectAxis.y * targetAxisZ.z) - (objectAxis.z * targetAxisZ.y));
	axis2.y = ((objectAxis.z * targetAxisZ.x) - (objectAxis.x * targetAxisZ.z));
	axis2.z = ((objectAxis.x * targetAxisZ.y) - (objectAxis.y * targetAxisZ.x));

	axis2.normalize();

	_local_5 = MyMath::acos(_local_6);

	Matrix3 matrix2;
	matrix2.fromAxisAngle(&axis2, _local_5);

	matrix1.append(&matrix2);

	Vector3 eulerAngles;

	matrix1.getEulerAngles(&eulerAngles);

	_arg_1->rotationX = eulerAngles.x;
	_arg_1->rotationY = eulerAngles.y;
	_arg_1->rotationZ = eulerAngles.z;

	_arg_1->x = _arg_2->x;
	_arg_1->y = _arg_2->y;
	_arg_1->z = _arg_2->z;
}

#if 0
public static function setEffectParams(_arg_1:Object3D):void
{
	_arg_1.softAttenuation = 80;
	_arg_1.useLight = false;
	_arg_1.shadowMapAlphaThreshold = 2;
	_arg_1.depthMapAlphaThreshold = 2;
	_arg_1.useShadowMap = false;
}

public static function copyColorTransform(_arg_1:ColorTransform, _arg_2:ColorTransform):void
{
	_arg_2.redMultiplier = _arg_1.redMultiplier;
	_arg_2.greenMultiplier = _arg_1.greenMultiplier;
	_arg_2.blueMultiplier = _arg_1.blueMultiplier;
	_arg_2.alphaMultiplier = _arg_1.alphaMultiplier;
	_arg_2.redOffset = _arg_1.redOffset;
	_arg_2.greenOffset = _arg_1.greenOffset;
	_arg_2.blueOffset = _arg_1.blueOffset;
	_arg_2.alphaOffset = _arg_1.alphaOffset;
}

public static function calculateAlphaForObject(_arg_1:Object3D, _arg_2:Vector3, _arg_3:Vector3, _arg_4:Boolean, _arg_5:Number, _arg_6:Number):void
{
	var _local_7:Number = (_arg_2.x - _arg_1.x);
	var _local_8:Number = (_arg_2.y - _arg_1.y);
	var _local_9:Number = (_arg_2.z - _arg_1.z);
	var _local_10:Number = Math.sqrt((((_local_7 * _local_7) + (_local_8 * _local_8)) + (_local_9 * _local_9)));
	_local_7 = (_local_7 / _local_10);
	_local_8 = (_local_8 / _local_10);
	_local_9 = (_local_9 / _local_10);
	var _local_11:Number = Math.abs((((_local_7 * _arg_3.x) + (_local_8 * _arg_3.y)) + (_local_9 * _arg_3.z)));
	if (_arg_4)
	{
		_local_11 = (1 - _local_11);
	};
	_local_11 = Math.pow(_local_11, _arg_5);
	_arg_1.alpha = (1 - (_local_11 / _arg_6));
	if (_arg_1.alpha < 0)
	{
		_arg_1.alpha = 0;
	};
}
#endif

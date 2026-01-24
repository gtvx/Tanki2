#include "GraphicsUtils.h"
#include "game/alternativa/tanks/engine3d/TextureAnimation.h"
#include "game/alternativa/utils/TextureMaterialRegistry.h"
#include "entrance/platform/client/fp10/core/resource/types/MultiframeTextureResource.h"
#include "game/alternativa/tanks/engine3d/UVFrame.h"
#include "MyMath.h"
#include "flash/display/BitmapData.h"


//private static var filteredImages:Dictionary = new Dictionary(true);


#if 0
public static function setObjectTransform(_arg_1:Object3D, _arg_2:Vector3, _arg_3:Vector3):void
{
	_arg_1.x = _arg_2.x;
	_arg_1.y = _arg_2.y;
	_arg_1.z = _arg_2.z;
	_arg_1.rotationX = _arg_3.x;
	_arg_1.rotationY = _arg_3.y;
	_arg_1.rotationZ = _arg_3.z;
}
#endif


std::shared_ptr<TextureAnimation> GraphicsUtils::getTextureAnimationFromResource(TextureMaterialRegistry *registry, MultiframeTextureResource *resource)
{
	std::shared_ptr<TextureAnimation> textureAnimation = getTextureAnimation(registry, resource->getData(), resource->getFrameWidth(), resource->getFrameHeight(), resource->getNumFrames());
	textureAnimation->fps = resource->getFps();
	return textureAnimation;
}

#if 0
public static function createFilteredImage(_arg_1:BitmapData, _arg_2:BitmapFilter):BitmapData
{
	var _local_3:*;
	var _local_4:BitmapData;
	var _local_5:Object;
	for (_local_3 in filteredImages)
	{
		_local_5 = filteredImages[_local_3];
		if (((_local_5.s == _arg_1) && (_local_5.f == _arg_2)))
		{
			return (_local_3);
		};
	};
	_local_4 = _arg_1.clone();
	_local_4.applyFilter(_arg_1, _arg_1.rect, new Point(), _arg_2);
	filteredImages[_local_4] = {
		"s":_arg_1,
		"f":_arg_2
	};
	return (_local_4);
}
#endif

std::shared_ptr<TextureAnimation> GraphicsUtils::getTextureAnimation(TextureMaterialRegistry *_arg_1, BitmapData *_arg_2, int _arg_3, int _arg_4, int _arg_5, bool _arg_6)
{
	(void)_arg_6;

	std::shared_ptr<TextureMaterial> _local_7 = _arg_1->getMaterial(_arg_2);
	QVector<UVFrame*> _local_8 = getUVFramesFromTexture(_arg_2, _arg_3, _arg_4, _arg_5);
	return std::make_shared<TextureAnimation>(_local_7, _local_8);
}


QVector<UVFrame*> GraphicsUtils::getUVFramesFromTexture(BitmapData *_arg_1, int _arg_2, int _arg_3, int _arg_4)
{

	int _local_5 = _arg_1->width();
	int _local_6 = MyMath::min(_arg_2, _local_5);
	int _local_7 = int((_local_5 / _local_6));
	int _local_8 = _arg_1->height();
	int _local_9 = MyMath::min(_arg_3, _local_8);
	int _local_10 = int((_local_8 / _local_9));
	int _local_11 = (_local_7 * _local_10);

	if (((_arg_4 > 0) && (_local_11 > _arg_4)))
	{
		_local_11 = _arg_4;
	}

	QVector<UVFrame*> _local_12;
	_local_12.resize(_local_11);


	int _local_13 = 0;
	int _local_14 = 0;

	while (_local_14 < _local_10)
	{
		int _local_15 = (_local_14 * _local_9);
		int _local_16 = (_local_15 + _local_9);
		int _local_17 = 0;
		while (_local_17 < _local_7)
		{
			int _local_18 = (_local_17 * _local_6);
			int _local_19 = (_local_18 + _local_6);
			int _local_20 = _local_13++;

			_local_12[_local_20] = new UVFrame(double(_local_18) / double(_local_5),
											   double(_local_15) / double(_local_8),
											   double(_local_19) / double(_local_5),
											   double(_local_16) / double(_local_8));

			if (_local_13 == _local_11)
			{
				return _local_12;
			}
			_local_17++;
		}
		_local_14++;
	}

	return _local_12;
}


#if 0
public static function getUVFramesFromTextureWithMirror(_arg_1:BitmapData, _arg_2:int, _arg_3:int, _arg_4:int=0):Vector.<UVFrame>
{
	var _local_15:int;
	var _local_16:int;
	var _local_17:int;
	var _local_18:int;
	var _local_19:int;
	var _local_20:Number;
	var _local_21:Number;
	var _local_5:int = _arg_1.width;
	var _local_6:int = Math.min(_arg_2, _local_5);
	var _local_7:int = int((_local_5 / _local_6));
	var _local_8:int = _arg_1.height;
	var _local_9:int = Math.min(_arg_3, _local_8);
	var _local_10:int = int((_local_8 / _local_9));
	var _local_11:int = (_local_7 * _local_10);
	if (((_arg_4 > 0) && (_local_11 > _arg_4)))
	{
		_local_11 = _arg_4;
	}
	var _local_12:Vector.<UVFrame> = new Vector.<UVFrame>(_local_11);
	var _local_13:int;
	var _local_14:int;
	while (_local_14 < _local_10)
	{
		_local_15 = (_local_14 * _local_9);
		_local_16 = (_local_15 + _local_9);
		_local_17 = 0;
		while (_local_17 < _local_7)
		{
			_local_18 = (_local_17 * _local_6);
			_local_19 = (_local_18 + _local_6);
			_local_20 = (_local_18 / _local_5);
			_local_21 = (_local_19 / _local_5);
			var _local_22:* = _local_13++;
			_local_12[_local_22] = new UVFrame(_local_20, (_local_15 / _local_8), _local_21, (_local_16 / _local_8));
			if (_local_13 == _local_11)
			{
				return (_local_12);
			}
			var _local_23:* = _local_13++;
			_local_12[_local_23] = new UVFrame(_local_21, (_local_15 / _local_8), _local_20, (_local_16 / _local_8));
			if (_local_13 == _local_11)
			{
				return (_local_12);
			}
			_local_17++;
		}
		_local_14++;
	}
	return (_local_12);
}
#endif

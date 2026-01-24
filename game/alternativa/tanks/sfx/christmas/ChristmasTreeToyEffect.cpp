#include "ChristmasTreeToyEffect.h"
#include <QDebug>

/*
private static const SIZE:Number = 0x0100;
private static const RESOLUTION:Number = 2;
private static const RedTexture:Class = ChristmasTreeToyEffect_RedTexture;
private static const redMaterial:TextureMaterial = new TextureMaterial(new RedTexture().bitmapData, false, true, MipMapping.PER_PIXEL, RESOLUTION);
private static const GreenTexture:Class = ChristmasTreeToyEffect_GreenTexture;
private static const greenMaterial:TextureMaterial = new TextureMaterial(new GreenTexture().bitmapData, false, true, MipMapping.PER_PIXEL, RESOLUTION);
private static const BlueTexture:Class = ChristmasTreeToyEffect_BlueTexture;
private static const blueMaterial:TextureMaterial = new TextureMaterial(new BlueTexture().bitmapData, false, true, MipMapping.PER_PIXEL, RESOLUTION);
private static const OrangeTexture:Class = ChristmasTreeToyEffect_OrangeTexture;
private static const orangeMaterial:TextureMaterial = new TextureMaterial(new OrangeTexture().bitmapData, false, true, MipMapping.PER_PIXEL, RESOLUTION);
private static const NEAR_1:Number = 100;
private static const NEAR_2:Number = 300;
private static const FAR_1:Number = 500;
private static const FAR_2:Number = 1000;
private static const MIN_SCALE:Number = 0.3;
private static const MAX_SCALE:Number = 1.3;
private static const ROTATION_SPEED:Number = (Math.PI / 2);//1.5707963267949
private static const INTERVAL:Number = 3;
*/


ChristmasTreeToyEffect::ChristmasTreeToyEffect(Pool *pool) :
	PooledObject(pool)
{

}


void ChristmasTreeToyEffect::addedToScene(Scene3DContainer *_arg_1)
{
	qDebug("ChristmasTreeToyEffect:addedToScene");

	(void)_arg_1;

	/*
	this.container = _arg_1;
	_arg_1.addChild(this.sprite);
	if (((GPUCapabilities.gpuEnabled) && (!(GPUCapabilities.constrained))))
	{
		_arg_1.addChild(this.omni);
	}
	*/
}


void ChristmasTreeToyEffect::init(std::shared_ptr<Sprite3D> _arg_1, Object3DPositionProvider *_arg_2, Vector3 *_arg_3)
{
	qDebug("used ChristmasTreeToyEffect:init");

	(void)_arg_1;
	(void)_arg_2;
	(void)_arg_3;

#if 0
	this.sprite = _arg_1;
	this.positionProvider = _arg_2;
	_arg_1.width = SIZE;
	_arg_1.height = SIZE;
	_arg_1.originX = 0.5;
	_arg_1.originY = 0.5;
	_arg_1.softAttenuation = 50;
	_arg_1.useLight = false;
	_arg_1.useShadowMap = false;
	_arg_1.shadowMapAlphaThreshold = 2;
	_arg_1.depthMapAlphaThreshold = 2;
	_arg_1.blendMode = BlendMode.ADD;
	if (_arg_1.name.indexOf("1") >= 0)
	{
		_arg_1.material = redMaterial;
		this.omni.color = 16727074;
	}
	else
	{
		if (_arg_1.name.indexOf("2") >= 0)
		{
			_arg_1.material = greenMaterial;
			this.omni.color = 0x60FF00;
		}
		else
		{
			if (_arg_1.name.indexOf("3") >= 0)
			{
				_arg_1.material = blueMaterial;
				this.omni.color = 34047;
			}
			else
			{
				if (_arg_1.name.indexOf("4") >= 0)
				{
					_arg_1.material = orangeMaterial;
					this.omni.color = 0xFFEA00;
				}
			}
		}
	}
	this.omni.intensity = 0.7;
	this.omni.attenuationBegin = NEAR_2;
	this.omni.attenuationEnd = FAR_2;
	this.omni.calculateBounds();
	var _local_4:Number = 200;
	var _local_5:Number = (_arg_1.x - _arg_3.x);
	var _local_6:Number = (_arg_1.y - _arg_3.y);
	var _local_7:Number = Math.sqrt(((_local_5 * _local_5) + (_local_6 * _local_6)));
	this.omni.x = (_arg_1.x + ((_local_5 * _local_4) / _local_7));
	this.omni.y = (_arg_1.y + ((_local_6 * _local_4) / _local_7));
	this.omni.z = (_arg_1.z + (_local_4 / 2));
	this.time = (Math.random() * INTERVAL);
	this.alive = true;
#endif
}


bool ChristmasTreeToyEffect::play(int _arg_1, GameCamera *_arg_2)
{
	qDebug("used ChristmasTreeToyEffect:play");

	(void)_arg_1;
	(void)_arg_2;

#if 0
	if ((!(this.alive)))
	{
		return (false);
	}
	this.positionProvider.updateObjectPosition(this.sprite, _arg_2, _arg_1);
	var _local_3:Number = (_arg_1 / 1000);
	this.time = (this.time + _local_3);
	this.time = (this.time % INTERVAL);
	var _local_4:Number = ((Math.sin((((Math.PI * 2) * this.time) / INTERVAL)) * 0.5) + 0.5);
	var _local_5:Number = (MIN_SCALE + ((MAX_SCALE - MIN_SCALE) * _local_4));
	this.sprite.scaleX = _local_5;
	this.sprite.scaleY = _local_5;
	this.sprite.scaleZ = _local_5;
	this.sprite.rotation = (this.sprite.rotation + ((ROTATION_SPEED * _local_3) * _local_4));
	this.omni.attenuationBegin = (NEAR_1 + ((NEAR_2 - NEAR_1) * _local_4));
	this.omni.attenuationEnd = (FAR_1 + ((FAR_2 - FAR_1) * _local_4));
	return (true);
#endif
	return false;
}


void ChristmasTreeToyEffect::destroy()
{
	qDebug("used ChristmasTreeToyEffect:destroy");

#if 0
	this.container.removeChild(this.sprite);
	if (((GPUCapabilities.gpuEnabled) && (!(GPUCapabilities.constrained))))
	{
		this.container.removeChild(this.omni);
	}
	this.sprite = null;
	this.container = null;
	this.positionProvider.destroy();
	this.positionProvider = null;
	recycle();
#endif
}


void ChristmasTreeToyEffect::kill()
{
	//this->alive = false;
}

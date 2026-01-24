#include "BitmapTextureResource.h"
#include "flash/display/BitmapData.h"
#include "MyMath.h"
#include "Error.h"
#include <QDebug>


BitmapTextureResource::BitmapTextureResource(BitmapData *bitmapData, bool mipMapping, bool stretchNotPowerOf2Textures, bool calculateMipMapsUsingGPU)
{

	this->referencesCount = 1;
	this->_correctionU = 1;
	this->_correctionV = 1;

	this->_bitmapData = bitmapData;
	this->_mipMapping = mipMapping;


	if (this->_bitmapData->width() > 10000)
	{
		throw Error("BitmapTextureResource error image size");
	}

	this->_stretchNotPowerOf2Textures = stretchNotPowerOf2Textures;
	this->_calculateMipMapsUsingGPU = calculateMipMapsUsingGPU;
	this->correctedWidth = MyMath::pow(2, MyMath::ceil((MyMath::log(this->_bitmapData->width()) / MyMath::LN2)));
	this->correctedHeight = MyMath::pow(2, MyMath::ceil((MyMath::log(this->_bitmapData->height()) / MyMath::LN2)));

	if (this->correctedWidth > 0x0800)
	{
		this->correctedWidth = 0x0800;
	}

	if (this->correctedHeight > 0x0800)
	{
		this->correctedHeight = 0x0800;
	}

	if ((((((!(this->_bitmapData->width() == this->correctedWidth)) || (!(this->_bitmapData->height() == this->correctedHeight))) && (!(this->_stretchNotPowerOf2Textures))) && (this->_bitmapData->width() <= 0x0800)) && (this->_bitmapData->height() <= 0x0800)))
	{
		this->_correctionU = (this->_bitmapData->width() / this->correctedWidth);
		this->_correctionV = (this->_bitmapData->height() / this->correctedHeight);
	}
}


void BitmapTextureResource::dispose()
{
	if (this->referencesCount == 0)
	{
		return;
	}

	this->referencesCount--;
	if (this->referencesCount == 0)
	{
		//TextureResourcesRegistry.release(this._bitmapData);
		//this._bitmapData = null;
		//super.dispose();
	}
}


void BitmapTextureResource::forceDispose()
{
	this->referencesCount = 1;
	this->dispose();
	this->_bitmapData = nullptr;
}


void BitmapTextureResource::increaseReferencesCount()
{
	this->referencesCount++;
}


bool BitmapTextureResource::getMipMapping()
{
	return this->_mipMapping;
}

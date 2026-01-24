#ifndef BITMAPTEXTURERESOURCE_H
#define BITMAPTEXTURERESOURCE_H

#include "GfxTextureResource.h"

class BitmapData;

class BitmapTextureResource : public GfxTextureResource
{
	BitmapData *_bitmapData;

	int referencesCount;

	int correctedWidth;
	int correctedHeight;

	double _correctionU;
	double _correctionV;

	bool _mipMapping;
	bool _stretchNotPowerOf2Textures;
	bool _calculateMipMapsUsingGPU;


public:
	BitmapTextureResource(BitmapData *bitmapData, bool mipMapping, bool stretchNotPowerOf2Textures = false, bool calculateMipMapsUsingGPU = false);


	BitmapData* bitmapData()
	{
		return this->_bitmapData;
	}


	void dispose();

	void forceDispose();

	void increaseReferencesCount();
	bool getMipMapping();
};

#endif // BITMAPTEXTURERESOURCE_H

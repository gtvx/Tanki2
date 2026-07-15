#ifndef BITMAPTEXTURERESOURCE_H
#define BITMAPTEXTURERESOURCE_H

#include "GfxTextureResource.h"
#include <memory>

class BitmapData;

class BitmapTextureResource : public GfxTextureResource
{
	std::shared_ptr<BitmapData> _bitmapData;

	int referencesCount;

	int correctedWidth;
	int correctedHeight;

	double _correctionU;
	double _correctionV;

	bool _mipMapping;
	bool _stretchNotPowerOf2Textures;
	bool _calculateMipMapsUsingGPU;


public:
	BitmapTextureResource(std::shared_ptr<BitmapData> bitmapData, bool mipMapping, bool stretchNotPowerOf2Textures = false, bool calculateMipMapsUsingGPU = false);


	std::shared_ptr<BitmapData> bitmapData()
	{
		return this->_bitmapData;
	}


	void dispose();

	void forceDispose();

	void increaseReferencesCount();
	bool getMipMapping();
};

#endif // BITMAPTEXTURERESOURCE_H

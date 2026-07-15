#include "TextureMaterialRegistryBase.h"
#include "CachedEntityStat.h"
#include "MaterialEntry.h"
#include "Error.h"
#include "TextureMaterialFactory.h"
#include "hardware/alternativa/engine3d/core/MipMapping.h"
#include "hardware/alternativa/tanks/materials/PaintMaterial.h"
#include "hardware/alternativa/tanks/materials/AnimatedPaintMaterial.h"
#include "entrance/platform/client/fp10/core/resource/types/TextureResource.h"
#include "entrance/platform/client/fp10/core/resource/types/MultiframeTextureResource.h"
#include "flash/display/BitmapData.h"
#include "qhash_shared_ptr.h"
#include <QDebug>


TextureMaterialRegistryBase::TextureMaterialRegistryBase(TextureMaterialFactory *materialFactory)
{
	this->materialFactory = materialFactory;
	this->materialStat = new CachedEntityStat();
	this->mipMappingEnabled = false;
}


std::shared_ptr<AnimatedPaintMaterial> TextureMaterialRegistryBase::getAnimatedPaint(MultiframeTextureResource *_arg_1,
																					 std::shared_ptr<BitmapData> lightMapBitmap,
																					 std::shared_ptr<BitmapData> texture,
																					 const Long &_arg_4)
{
	this->materialStat->requestCount++;

	QString _local_5 = _arg_1->getId().toString() + " " + _arg_4.toString();

	if (this->string_entryForTexture.contains(_local_5))
	{
		MaterialEntry *_local_9 = this->string_entryForTexture[_local_5];
		_local_9->referenceCount++;
		return std::dynamic_pointer_cast<AnimatedPaintMaterial>(_local_9->material);
	}


	std::shared_ptr<BitmapData> data = _arg_1->getData();

	int _local_6 = int(data->width() / _arg_1->getFrameWidth());
	int _local_7 = int(data->height() / _arg_1->getFrameHeight());

	std::shared_ptr<AnimatedPaintMaterial> _local_8 = new_AnimatedPaintMaterial(data,
																				lightMapBitmap,
																				texture,
																				_local_6,
																				_local_7,
																				_arg_1->getFps(),
																				_arg_1->getNumFrames(),
																				this->mipMappingEnabled ? (int)MipMapping::PER_PIXEL : 0);


	MaterialEntry *_local_9 = this->createPaintMaterialEntry(_local_5, _local_8);
	_local_9->referenceCount++;
	this->materials.append(_local_8);
	this->materialStat->createCount++;
	return _local_8;
}


std::shared_ptr<PaintMaterial> TextureMaterialRegistryBase::getPaint(TextureResource *_arg_1,
																	 std::shared_ptr<BitmapData> _arg_2,
																	 std::shared_ptr<BitmapData> _arg_3,
																	 const Long &_arg_4)
{
	qDebug("TextureMaterialRegistryBase::getPaint");

	this->materialStat->requestCount++;
	QString _local_5 = _arg_1->getId().toString() + " " + _arg_4.toString();


	if (this->string_entryForTexture.contains(_local_5))
	{
		MaterialEntry *_local_7 = string_entryForTexture[_local_5];
		_local_7->referenceCount++;
		return std::dynamic_pointer_cast<PaintMaterial>(_local_7->material);
	}

	std::shared_ptr<PaintMaterial> _local_6 = new_PaintMaterial(_arg_1->data, _arg_2, _arg_3, ((this->mipMappingEnabled) ? (int)MipMapping::PER_PIXEL : 0));
	MaterialEntry *_local_7 = this->createPaintMaterialEntry(_local_5, _local_6);
	_local_7->referenceCount++;
	this->materials.append(_local_6);
	this->materialStat->createCount++;
	return _local_6;
}


std::shared_ptr<TextureMaterial> TextureMaterialRegistryBase::getMaterial(std::shared_ptr<BitmapData> bitmapData, bool v)
{
	if (bitmapData == nullptr)
	{
		throw ArgumentError("Texture is null");
	}

	this->materialStat->requestCount++;
	MaterialEntry *materialEntry = this->getOrCreateEntry(bitmapData, v);
	materialEntry->referenceCount++;
	return materialEntry->material;
}


void TextureMaterialRegistryBase::addMaterial(std::shared_ptr<TextureMaterial> material)
{
	MaterialEntry *materialEntry = this->createMaterialEntry(nullptr, material);
	materialEntry->referenceCount++;
	this->entryForMaterial[material] = materialEntry;
	this->materials.append(material);
}


void TextureMaterialRegistryBase::releaseMaterial(std::shared_ptr<TextureMaterial>)
{
	qDebug("used TextureMaterialRegistryBase::releaseMaterial");
}


void TextureMaterialRegistryBase::setMipMapping(bool)
{
	qDebug("used TextureMaterialRegistryBase::setMipMapping");
}


void TextureMaterialRegistryBase::clear()
{
	qDebug("used TextureMaterialRegistryBase::clear");
}


MaterialEntry* TextureMaterialRegistryBase::getOrCreateEntry(std::shared_ptr<BitmapData> bitmapData, bool _arg_2)
{

	MaterialEntry* materialEntry = this->bitmapData_entryForTexture[bitmapData];

	if (materialEntry == nullptr)
	{
		std::shared_ptr<TextureMaterial> textureMaterial = this->createMaterial(bitmapData, _arg_2);
		materialEntry = this->createMaterialEntry(bitmapData, textureMaterial);
	}

	return materialEntry;
}


std::shared_ptr<TextureMaterial> TextureMaterialRegistryBase::createMaterial(std::shared_ptr<BitmapData> bitmapData, bool _arg_2)
{
	//qDebug() << "TextureMaterialRegistryBase::createMaterial" << bitmapData;

	std::shared_ptr<BitmapData> _local_3 = this->getTexture(bitmapData, _arg_2);
	std::shared_ptr<TextureMaterial> _local_4 = this->materialFactory->createTextureMaterial(_local_3, this->mipMappingEnabled);
	this->materials.append(_local_4);
	this->materialStat->createCount++;
	return _local_4;
}


MaterialEntry* TextureMaterialRegistryBase::createMaterialEntry(std::shared_ptr<BitmapData> _arg_1, std::shared_ptr<TextureMaterial> _arg_2)
{
	MaterialEntry *_local_3 = new MaterialEntry(_arg_1, _arg_2);
	this->bitmapData_entryForTexture[_arg_1] = _local_3;
	this->entryForMaterial[_arg_2] = _local_3;
	return _local_3;
}


std::shared_ptr<BitmapData> TextureMaterialRegistryBase::getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2)
{
	(void)bitmapData;
	(void)_arg_2;
	throw Error("Not implemented");
}


MaterialEntry* TextureMaterialRegistryBase::createPaintMaterialEntry(const QString &name, std::shared_ptr<TextureMaterial> material)
{
	MaterialEntry *materialEntry = new MaterialEntry(name, material);
	this->string_entryForTexture[name] = materialEntry;
	this->entryForMaterial[material] = materialEntry;
	return materialEntry;
}

#include "MaterialEntry.h"

MaterialEntry::MaterialEntry(BitmapData *keyData, std::shared_ptr<TextureMaterial> material)
{
	this->keyData_bitmapData = keyData;
	this->texture = nullptr;
	this->material = material;
	this->referenceCount = 0;
}

MaterialEntry::MaterialEntry(const QString &keyData, std::shared_ptr<TextureMaterial> material)
{
	this->keyData_string = keyData;
	this->texture = nullptr;
	this->material = material;
	this->referenceCount = 0;
}

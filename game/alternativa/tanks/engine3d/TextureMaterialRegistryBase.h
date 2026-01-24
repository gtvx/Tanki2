#ifndef TEXTUREMATERIALREGISTRYBASE_H
#define TEXTUREMATERIALREGISTRYBASE_H

#include "game/alternativa/utils/TextureMaterialRegistry.h"
#include <QHash>

class MaterialEntry;
class CachedEntityStat;
class TextureMaterialFactory;

class TextureMaterialRegistryBase : public TextureMaterialRegistry
{
	QHash<QString, MaterialEntry*> string_entryForTexture;
	QHash<BitmapData*, MaterialEntry*> bitmapData_entryForTexture;

	QHash<std::shared_ptr<TextureMaterial>, MaterialEntry*> entryForMaterial;


	QVector<std::shared_ptr<TextureMaterial>> materials;

	CachedEntityStat *materialStat;

	TextureMaterialFactory *materialFactory;

	bool mipMappingEnabled;


public:
	TextureMaterialRegistryBase(TextureMaterialFactory *materialFactory);


	std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, BitmapData*, BitmapData*, const Long&) override;
	std::shared_ptr<PaintMaterial> getPaint(TextureResource*, BitmapData*, BitmapData*, const Long&) override;
	std::shared_ptr<TextureMaterial> getMaterial(BitmapData*, bool = true) override;
	void addMaterial(std::shared_ptr<TextureMaterial>) override;
	void releaseMaterial(std::shared_ptr<TextureMaterial>)override;
	void setMipMapping(bool)override;
	void clear() override;

protected:
	virtual BitmapData *getTexture(BitmapData *bitmapData, bool _arg_2);

private:
	MaterialEntry* getOrCreateEntry(BitmapData *bitmapData, bool _arg_2);
	std::shared_ptr<TextureMaterial> createMaterial(BitmapData *bitmapData, bool _arg_2);
	MaterialEntry* createMaterialEntry(BitmapData *_arg_1, std::shared_ptr<TextureMaterial> _arg_2);
	MaterialEntry* createPaintMaterialEntry(const QString &_arg_1, std::shared_ptr<TextureMaterial> _arg_2);
};

#endif // TEXTUREMATERIALREGISTRYBASE_H

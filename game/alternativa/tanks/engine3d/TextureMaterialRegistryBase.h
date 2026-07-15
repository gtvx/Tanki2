#ifndef TEXTUREMATERIALREGISTRYBASE_H
#define TEXTUREMATERIALREGISTRYBASE_H

#include "game/alternativa/utils/TextureMaterialRegistry.h"
#include <QHash>
#include <memory>

class MaterialEntry;
class CachedEntityStat;
class TextureMaterialFactory;

class TextureMaterialRegistryBase : public TextureMaterialRegistry
{
	QHash<QString, MaterialEntry*> string_entryForTexture;
	QHash<std::shared_ptr<BitmapData>, MaterialEntry*> bitmapData_entryForTexture;

	QHash<std::shared_ptr<TextureMaterial>, MaterialEntry*> entryForMaterial;


	QVector<std::shared_ptr<TextureMaterial>> materials;

	CachedEntityStat *materialStat;

	TextureMaterialFactory *materialFactory;

	bool mipMappingEnabled;


public:
	TextureMaterialRegistryBase(TextureMaterialFactory *materialFactory);


	std::shared_ptr<AnimatedPaintMaterial> getAnimatedPaint(MultiframeTextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) override;
	std::shared_ptr<PaintMaterial> getPaint(TextureResource*, std::shared_ptr<BitmapData>, std::shared_ptr<BitmapData>, const Long&) override;
	std::shared_ptr<TextureMaterial> getMaterial(std::shared_ptr<BitmapData>, bool = true) override;
	void addMaterial(std::shared_ptr<TextureMaterial>) override;
	void releaseMaterial(std::shared_ptr<TextureMaterial>)override;
	void setMipMapping(bool)override;
	void clear() override;

protected:
	virtual std::shared_ptr<BitmapData> getTexture(std::shared_ptr<BitmapData> bitmapData, bool _arg_2);

private:
	MaterialEntry* getOrCreateEntry(std::shared_ptr<BitmapData> bitmapData, bool _arg_2);
	std::shared_ptr<TextureMaterial> createMaterial(std::shared_ptr<BitmapData> bitmapData, bool _arg_2);
	MaterialEntry* createMaterialEntry(std::shared_ptr<BitmapData> _arg_1, std::shared_ptr<TextureMaterial> _arg_2);
	MaterialEntry* createPaintMaterialEntry(const QString &_arg_1, std::shared_ptr<TextureMaterial> _arg_2);
};

#endif // TEXTUREMATERIALREGISTRYBASE_H

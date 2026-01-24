#include "DefaultTankSkinMaterialsFactory.h"
#include "entrance/projects/tanks/clients/flash/commons/models/coloring/IColoring.h"
#include "../TankHullSkinCacheItem.h"
#include "../turret/TurretSkinCacheItem.h"
#include "../TankSkin.h"
#include "game/alternativa/utils/TextureMaterialRegistry.h"
#include "../SkinMaterials.h"
#include "entrance/alternativa/types/Long.h"
#include "hardware/alternativa/tanks/materials/AnimatedPaintMaterial.h"
#include "hardware/alternativa/tanks/materials/TrackMaterial.h"
#include "game/alternativa/tanks/engine3d/TextureMaterialRegistryBase.h"


TextureMaterialRegistry *DefaultTankSkinMaterialsFactory::textureMaterialRegistry;



DefaultTankSkinMaterialsFactory::DefaultTankSkinMaterialsFactory()
{

}


SkinMaterials* DefaultTankSkinMaterialsFactory::createSkinMaterials(TankSkin *tankSkin, IColoring *coloring)
{
	TankHullSkinCacheItem *hull_cache = tankSkin->getHullDescriptor();
	TurretSkinCacheItem *turret_cache = tankSkin->getTurretDescriptor();

	Long hull_id = hull_cache->partId;
	BitmapData *hull_bitmap_lightmap = hull_cache->lightmap;
	BitmapData *hull_bitmap_details = hull_cache->details;

	Long turret_id = turret_cache->partId;
	BitmapData *turret_bitmap_lightmap = turret_cache->lightmap;
	BitmapData *turret_bitmap_details = turret_cache->details;

	std::shared_ptr<TextureMaterial> hull_texture_material;
	std::shared_ptr<TextureMaterial> turret_texture_material;

	if (coloring->isAnimated())
	{
		MultiframeTextureResource *resource = coloring->getAnimatedColoring();
		hull_texture_material = textureMaterialRegistry->getAnimatedPaint(resource, hull_bitmap_lightmap, hull_bitmap_details, hull_id);
		turret_texture_material = textureMaterialRegistry->getAnimatedPaint(resource, turret_bitmap_lightmap, turret_bitmap_details, turret_id);
	}
	else
	{
		TextureResource *resource = coloring->getColoring();
		hull_texture_material = textureMaterialRegistry->getPaint(resource, hull_bitmap_lightmap, hull_bitmap_details, hull_id);
		turret_texture_material = textureMaterialRegistry->getPaint(resource, turret_bitmap_lightmap, turret_bitmap_details, turret_id);
	}

	std::shared_ptr<TrackMaterial> track_material_1 = new_TrackMaterial(hull_bitmap_details);
	std::shared_ptr<TrackMaterial> track_material_2 = new_TrackMaterial(hull_bitmap_details);
	textureMaterialRegistry->addMaterial(track_material_1);
	textureMaterialRegistry->addMaterial(track_material_2);

	return new SkinMaterials(hull_texture_material, turret_texture_material, track_material_1, track_material_2);
}


SkinMaterials *DefaultTankSkinMaterialsFactory::createDeadSkinMaterials(TankSkin *tankSkin, TextureResource *resource)
{
	TankHullSkinCacheItem *hull_cache = tankSkin->getHullDescriptor();
	TurretSkinCacheItem *turret_cache = tankSkin->getTurretDescriptor();

	Long hull_id = hull_cache->partId;
	BitmapData *hull_bitmap_lightmap = hull_cache->lightmap;
	BitmapData *hull_bitmap_details = hull_cache->details;

	Long turret_id = turret_cache->partId;
	BitmapData *turret_bitmap_lightmap = turret_cache->lightmap;
	BitmapData *turret_bitmap_details = turret_cache->details;

	std::shared_ptr<TextureMaterial> _local_11 = textureMaterialRegistry->getPaint(resource, hull_bitmap_lightmap, hull_bitmap_details, hull_id);
	std::shared_ptr<TextureMaterial> _local_12 = textureMaterialRegistry->getPaint(resource, turret_bitmap_lightmap, turret_bitmap_details, turret_id);

	return new SkinMaterials(_local_11, _local_12);
}

#include "DefaultTextureMaterialFactory.h"
#include "hardware/alternativa/engine3d/core/MipMapping.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"

std::shared_ptr<TextureMaterial> DefaultTextureMaterialFactory::createTextureMaterial(BitmapData *bitmapData, bool b)
{
	return new_TextureMaterial(1, bitmapData, false, true, (b ? (int)MipMapping::PER_PIXEL : 0));
}

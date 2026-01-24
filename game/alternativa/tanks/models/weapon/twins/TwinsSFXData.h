#ifndef TWINSSFXDATA_H
#define TWINSSFXDATA_H

#include <memory>

class TextureAnimation;
class TextureMaterial;
class LightAnimation;
class Sound;

class TwinsSFXData
{
public:
	std::shared_ptr<TextureAnimation> shotAnimation;
	std::shared_ptr<TextureAnimation> explosionAnimation;
	std::shared_ptr<TextureMaterial> muzzleFlashMaterial;
	//Sound *shotSound;
	std::shared_ptr<TextureMaterial> hitMarkMaterial;
	//LightAnimation *shotLightingAnimation;
	//LightAnimation *shellLightingAnimation;
	//LightAnimation *hitLightingAnimation;
};

#endif // TWINSSFXDATA_H

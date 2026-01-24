#ifndef TWINSSHOOTSFXCC_H
#define TWINSSHOOTSFXCC_H

class MultiframeTextureResource;
class TextureResource;
class LightingSFXEntity;
class SoundResource;

class TwinsShootSFXCC
{
public:
	MultiframeTextureResource *explosionTexture;
	TextureResource *hitMarkTexture;
	LightingSFXEntity *lightingSFXEntity;
	TextureResource *muzzleFlashTexture;
	SoundResource *shotSound;
	MultiframeTextureResource *shotTexture;
};

#endif // TWINSSHOOTSFXCC_H

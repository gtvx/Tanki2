#ifndef TEXTURECONSTRUCTOR_H
#define TEXTURECONSTRUCTOR_H

#include <memory>

class BitmapData;
class ITextureConstructorListener;
class Loader;
class TextureByteData;

class TextureConstructor
{
	std::shared_ptr<BitmapData> _texture;
	ITextureConstructorListener *listener;
	Loader *loader;
	TextureByteData *textureData;
	bool cancelled;

public:
	TextureConstructor();

	std::shared_ptr<BitmapData> texture()
	{
		return _texture;
	}

	void createTexture(TextureByteData *textureData, ITextureConstructorListener *listener);

private:
	void complete();
	void cancel();
};

#endif // TEXTURECONSTRUCTOR_H

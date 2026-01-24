#ifndef TEXTURECONSTRUCTOR_H
#define TEXTURECONSTRUCTOR_H

class BitmapData;
class ITextureConstructorListener;
class Loader;
class TextureByteData;

class TextureConstructor
{
	BitmapData *_texture;
	ITextureConstructorListener *listener;
	Loader *loader;
	TextureByteData *textureData;
	bool cancelled;

public:
	TextureConstructor();

	BitmapData* texture()
	{
		return _texture;
	}

	void createTexture(TextureByteData *textureData, ITextureConstructorListener *listener);

private:
	void complete();
	void cancel();
};

#endif // TEXTURECONSTRUCTOR_H

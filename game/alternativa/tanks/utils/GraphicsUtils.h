#ifndef GRAPHICSUTILS_H
#define GRAPHICSUTILS_H

#include <QVector>
#include <memory>

class UVFrame;
class BitmapData;
class TextureAnimation;
class TextureMaterialRegistry;
class MultiframeTextureResource;

class GraphicsUtils
{
public:

	static std::shared_ptr<TextureAnimation> getTextureAnimationFromResource(TextureMaterialRegistry *registry, MultiframeTextureResource *resource);

	static std::shared_ptr<TextureAnimation> getTextureAnimation(TextureMaterialRegistry *_arg_1, BitmapData *_arg_2, int _arg_3, int _arg_4, int _arg_5 = 0, bool _arg_6 = 0);

	static QVector<UVFrame*> getUVFramesFromTexture(BitmapData *_arg_1, int _arg_2, int _arg_3, int _arg_4 = 0);
};

#endif // GRAPHICSUTILS_H

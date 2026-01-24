#ifndef ICOLORING_H
#define ICOLORING_H

class TextureResource;
class MultiframeTextureResource;

class IColoring
{
public:
	virtual TextureResource* getColoring() = 0;
	virtual MultiframeTextureResource* getAnimatedColoring() = 0;
	virtual bool isAnimated() = 0;
};

#endif // ICOLORING_H

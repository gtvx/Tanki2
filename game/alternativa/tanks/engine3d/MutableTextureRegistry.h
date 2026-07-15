#ifndef MUTABLETEXTUREREGISTRY_H
#define MUTABLETEXTUREREGISTRY_H

class BitmapData;
class Function;

#include <memory>

class MutableTextureRegistry
{
public:
	virtual std::shared_ptr<BitmapData> getTexture(std::shared_ptr<BitmapData>, bool = true) = 0;
	virtual void addTextureChangeHandler(Function*) = 0;
	virtual void clear() = 0;
};

#endif // MUTABLETEXTUREREGISTRY_H

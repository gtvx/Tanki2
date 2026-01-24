#ifndef MUTABLETEXTUREREGISTRY_H
#define MUTABLETEXTUREREGISTRY_H

class BitmapData;
class Function;

class MutableTextureRegistry
{
public:
	virtual BitmapData* getTexture(BitmapData*, bool = true) = 0;
	virtual void addTextureChangeHandler(Function*) = 0;
	virtual void clear() = 0;
};

#endif // MUTABLETEXTUREREGISTRY_H

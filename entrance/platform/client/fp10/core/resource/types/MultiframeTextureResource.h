#ifndef MULTIFRAMETEXTURERESOURCE_H
#define MULTIFRAMETEXTURERESOURCE_H

#include "../Resource.h"
#include <QByteArray>


class ResourceImageFrameParams;
//class FramesConstructor;
class SafeURLLoader;
class BitmapData;

class MultiframeTextureResource : public Resource
{

	ResourceImageFrameParams *multiframeResourceInfo;
	//FramesConstructor *framesConstructor;
	SafeURLLoader *loader;
	QByteArray taraData;
	BitmapData *_data;

public:
	MultiframeTextureResource(ResourceInfo *resourceInfo);
	void buildFrames(const QByteArray &data);
	int getFrameWidth();
	int getFrameHeight();
	int getNumFrames();
	double getFps();
	BitmapData* getData();
};

#endif // MULTIFRAMETEXTURERESOURCE_H

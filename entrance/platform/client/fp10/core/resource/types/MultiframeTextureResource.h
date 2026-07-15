#ifndef MULTIFRAMETEXTURERESOURCE_H
#define MULTIFRAMETEXTURERESOURCE_H

#include "../Resource.h"
#include <memory>
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
	std::shared_ptr<BitmapData> _data;

public:
	MultiframeTextureResource(ResourceInfo *resourceInfo);
	void buildFrames(const QByteArray &data);
	int getFrameWidth();
	int getFrameHeight();
	int getNumFrames();
	double getFps();
	std::shared_ptr<BitmapData> getData();
};

#endif // MULTIFRAMETEXTURERESOURCE_H

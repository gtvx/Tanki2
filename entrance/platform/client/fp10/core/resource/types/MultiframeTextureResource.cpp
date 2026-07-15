#include "MultiframeTextureResource.h"
#include "../tara/TARAParser.h"
#include "stream/stream_read.h"
#include "Error.h"
#include "entrance/platform/core/general/resource/types/imageframe/ResourceImageFrameParams.h"
#include "flash/display/BitmapData.h"


std::shared_ptr<BitmapData> getDetails(const QByteArray &details, const QByteArray &alpha);


static const QString TARA_FILE = "image.tara";
static const QString PARAMS_FILE = "p";
static const QString DIFFUSE_FILE = "i";
static const QString ALPHA_FILE = "a";


MultiframeTextureResource::MultiframeTextureResource(ResourceInfo *resourceInfo) :
	Resource(resourceInfo)
{

}


int MultiframeTextureResource::getFrameWidth()
{
	return this->multiframeResourceInfo->frameWidth();
}

int MultiframeTextureResource::getFrameHeight()
{
	return this->multiframeResourceInfo->frameHeight();
}

int MultiframeTextureResource::getNumFrames()
{
	return this->multiframeResourceInfo->numFrames();
}

double MultiframeTextureResource::getFps()
{
	return this->multiframeResourceInfo->fps();
}

std::shared_ptr<BitmapData> MultiframeTextureResource::getData()
{
	return this->_data;
}



static ResourceImageFrameParams* decode_params(const QByteArray &data)
{
	stream_read stream;
	stream_read_init(&stream, (const uint8_t*)data.constData(), data.size());

	if (stream_read_available(&stream) != 25)
	{
		return nullptr;
	}

	uint16_t size = stream_read_uint16_be(&stream);

	if (size != 23)
	{
		return nullptr;
	}

	uint8_t v = stream_read_uint8(&stream);

	if (v != 0)
	{
		return nullptr;
	}

	double fps = stream_read_float_be(&stream);
	int frameHeight = stream_read_uint32_be(&stream);
	int frameWidth = stream_read_uint32_be(&stream);
	int imageHeight = stream_read_uint32_be(&stream);
	int imageWidth = stream_read_uint32_be(&stream);
	int numFrames = stream_read_uint16_be(&stream);


	return new ResourceImageFrameParams(fps,
										frameHeight,
										frameWidth,
										imageHeight,
										imageWidth,
										numFrames);
}


void  MultiframeTextureResource::buildFrames(const QByteArray &data)
{
	TARAParser tara(data);

	multiframeResourceInfo = decode_params(tara.getFileData(PARAMS_FILE));

	if (multiframeResourceInfo == nullptr)
	{
		throw Error("Error MultiframeTextureResource::buildFrames PARAMS_FILE");
	}


	QByteArray diffuse_data = tara.getFileData(DIFFUSE_FILE);

	if (diffuse_data.isEmpty())
	{
		throw Error("Error MultiframeTextureResource::buildFrames diffuse_data");
	}


	if (tara.contains(ALPHA_FILE))
	{
		QByteArray alpga_data = tara.getFileData(ALPHA_FILE);

		if (alpga_data.isEmpty())
		{
			throw Error("Error MultiframeTextureResource::buildFrames alpga_data");
		}

		this->_data = getDetails(diffuse_data, alpga_data);
	}
	else
	{
		this->_data = std::make_shared<BitmapData>();
		this->_data->loadFromData(diffuse_data);
	}

}

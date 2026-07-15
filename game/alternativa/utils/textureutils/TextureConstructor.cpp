#include "TextureConstructor.h"
#include "TextureByteData.h"
#include "Error.h"
#include "ITextureConstructorListener.h"
#include "flash/display/BitmapData.h"
#include <QImage>


static std::shared_ptr<BitmapData> mergeBitmapAlpha(const QByteArray &diffuseData, const QByteArray &alpha)
{
	QImage image_details;
	image_details.loadFromData(diffuseData);

	QImage image_details_alpha;
	image_details_alpha.loadFromData(alpha);

	std::shared_ptr<BitmapData> result_details = std::make_shared<BitmapData>(image_details.width(), image_details.height(), true);

	QImage *result = result_details->qimage();

	uint8_t *byte_result = (uint8_t*)result->bits();
	uint8_t *byte_details = (uint8_t*)image_details.bits();
	uint8_t *byte_alpha = (uint8_t*)image_details_alpha.bits();

	int depth_result = result->depth() / 8;
	int depth_details = image_details.depth() / 8;
	int depth_alpha = image_details_alpha.depth() / 8;

	int count = result->sizeInBytes() / (result->depth() / 8);

	for (int i = 0; i < count; i++)
	{
		uint8_t alpha = byte_alpha[0];

		byte_result[0] = (byte_details[0] * alpha) / 255;
		byte_result[1] = (byte_details[1] * alpha) / 255;
		byte_result[2] = (byte_details[2] * alpha) / 255;

		byte_result[3] = alpha;

		byte_result += depth_result;
		byte_details += depth_details;
		byte_alpha += depth_alpha;
	}

	return result_details;

}


TextureConstructor::TextureConstructor()
{
	_texture = nullptr;
	listener = nullptr;
	loader = nullptr;
	textureData = nullptr;
	cancelled = false;
}


void TextureConstructor::createTexture(TextureByteData *textureData, ITextureConstructorListener *listener)
{
	if (this->loader != nullptr)
	{
		throw Error("Construction in progress");
	}

	if (textureData == nullptr)
	{
		throw ArgumentError("Parameter textureData is null");
	}

	if (textureData->diffuseData == nullptr)
	{
		throw ArgumentError("Diffuse data is null");
	}

	this->cancelled = false;
	this->textureData = textureData;
	this->listener = listener;

	if (textureData->opacityData.isEmpty())
	{
		this->_texture = std::make_shared<BitmapData>();
		if (!this->_texture->loadFromData(textureData->diffuseData))
		{
			throw Error("error TextureConstructor::createTexture loadFromData diffuseData");
		}
	}
	else
	{
		this->_texture = mergeBitmapAlpha(textureData->diffuseData, textureData->opacityData);

		if (this->_texture == nullptr)
		{
			throw Error("error TextureConstructor::createTexture loadFromData mergeBitmapAlpha");
		}
	}

	complete();
}



void TextureConstructor::cancel()
{
	this->cancelled = true;
}


/*
void TextureConstructor::loadBytes(const QByteArray &data, Function *callback)
{
	//data.position = 0;
	this->loader = new Loader();
	this->loader->contentLoaderInfo->addEventListener(Event.COMPLETE, callback);
	this->loader->loadBytes(data);
}

void TextureConstructor::onDiffuseTextureLoadingComplete(Event *e)
{
	if (this.cancelled)
	{
		Bitmap(this->loader->content)->bitmapData->dispose();
		this->loader->unload();
		this->loader = nullptr;
	}
	else
	{
		this->_texture = Bitmap(this->loader->content)->bitmapData;
		this->loader->contentLoaderInfo->removeEventListener(Event.COMPLETE, this->onDiffuseTextureLoadingComplete);
		this->loader->unload();
		if (this->textureData->opacityData != nullptr)
		{
			this->loadBytes(this->textureData->opacityData, this->onAlphaTextureLoadingComplete);
		}
		else
		{
			this->complete();
		}
	}
}

void TextureConstructor::onAlphaTextureLoadingComplete(Event *e)
{
	if (this->cancelled)
	{
		Bitmap(this->loader->content)->bitmapData->dispose();
		this->loader->unload();
		this->loader = nullptr;
	}
	else
	{
		BitmapData *alpha = Bitmap(this->loader->content)->bitmapData;
		this->loader->contentLoaderInfo->removeEventListener(Event.COMPLETE, this->onAlphaTextureLoadingComplete);
		this->loader->unload();
		this->_texture = BitmapUtils::mergeBitmapAlpha(this->_texture, alpha, true);
		this->complete();
	}
}
*/

void TextureConstructor::complete()
{
	this->loader = nullptr;
	this->textureData = nullptr;
	ITextureConstructorListener *listener = this->listener;
	this->listener = nullptr;
	listener->onTextureReady(this);
}

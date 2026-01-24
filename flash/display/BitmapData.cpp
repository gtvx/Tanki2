#include "BitmapData.h"
#include "flash/geom/GeomRectangle.h"
#include "flash/geom/ColorTransform.h"
#include <QImage>
#include <QPainter>
#include <QBuffer>
#include <QDebug>



BitmapData::BitmapData()
{
	this->_width = 0;
	this->_height = 0;
	this->_transparent = false;
	this->image = new QImage();
}

BitmapData::BitmapData(uint32_t width, uint32_t height, bool transparent)
{
	this->_width = width;
	this->_height = height;
	this->_transparent = transparent;

	QImage::Format format = transparent ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32;
	this->image = new QImage(width, height, format);
}

BitmapData::BitmapData(uint32_t width, uint32_t height, bool transparent, uint32_t fillColor)
{
	this->_width = width;
	this->_height = height;
	this->_transparent = transparent;

	QImage::Format format = transparent ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32;
	this->image = new QImage(width, height, format);
	this->image->fill(fillColor);
}

BitmapData::~BitmapData()
{
	delete this->image;
}

void BitmapData::fill(uint32_t color)
{
	this->image->fill(color);
}

void BitmapData::beginBitmapFill(const BitmapData *src)
{
	QPainter p(this->image);

	for (uint32_t x = 0; x < this->_width;)
	{
		for (uint32_t y = 0; y < this->_height;)
		{
			p.drawImage(x, y, *src->image);
			y += src->_height;
		}

		x += src->_width;
	}
	p.end();
}

bool BitmapData::loadFromData(const QByteArray &data)
{
	bool result = this->image->loadFromData(data);

	if (result == false)
	{
		this->_width = false;
		this->_height = false;
		this->_transparent = false;
		return false;
	}

	this->_width =  this->image->width();
	this->_height =  this->image->height();

	QImage::Format format = this->image->format();

	if (format == QImage::Format_ARGB32)
	{
		this->image->convertTo(QImage::Format_ARGB32_Premultiplied);
		this->_transparent = true;
		return true;
	}

	if (format == QImage::Format_RGB32)
	{
		this->_transparent = false;
		return true;
	}

	if (format == QImage::Format_Indexed8)
	{
		this->image->convertTo(QImage::Format_ARGB32_Premultiplied);
		this->_transparent = true;
		return true;
	}

	qDebug() << "error BitmapData::loadFromData" << format;

	return false;
}

QByteArray BitmapData::toData()
{
	QByteArray data;
	QBuffer buffer(&data);
	buffer.open(QIODevice::WriteOnly);
	this->image->save(&buffer, "png");
	return data;
}

uint32_t* BitmapData::data()
{
	return (uint32_t*)this->image->bits();
}

uint32_t BitmapData::size()
{
	return 0;
}

void BitmapData::copy(const BitmapData *src)
{
	*this->image = *src->image;
	this->_width =  src->image->width();
	this->_height =  src->image->height();
	this->_transparent = src->_transparent;
}

void BitmapData::scaled(int w, int h)
{
	*this->image = this->image->scaled(QSize(w, h), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->_width =  this->image->width();
	this->_height =  this->image->height();
}


int BitmapData::width() const
{
	return _width;
}

int BitmapData::height() const
{
	return _height;
}

bool BitmapData::transparent()
{
	return _transparent;
}

void BitmapData::dispose()
{

}

void BitmapData::getRect(GeomRectangle *rectangle)
{
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->width = _width;
	rectangle->height = _height;
}

void BitmapData::copyPixels(const BitmapData *sourceBitmapData,
							const GeomRectangle *sourceRect,
							const Point *destPoint,
							BitmapData *alphaBitmapData,
							Point *alphaPoint,
							bool mergeAlpha)
{
	(void)alphaBitmapData;
	(void)alphaPoint;
	(void)mergeAlpha;

	QPainter p(this->image);
	p.drawImage(destPoint->x, destPoint->y, *sourceBitmapData->image, sourceRect->x, sourceRect->y, sourceRect->width, sourceRect->height);
	p.end();
}

void BitmapData::setPixel(int x, int y, uint32_t color)
{
	this->image->setPixel(x, y, color);
}

uint32_t BitmapData::getPixel(int x, int y)
{
	return this->image->pixel(x, y);
}

void BitmapData::colorTransform(const ColorTransform *colorTransform)
{
	(void)colorTransform;
}

#include "Graphics.h"
#include <QPainter>
#include <QPainterPath>

static QFont font("Times", 13, QFont::Bold);

Graphics::Graphics()
{
	this->painter_path = new QPainterPath;
}

void Graphics::setPainter(QPainter *painter)
{
	this->painter = painter;
}

void Graphics::beginFill(uint32_t color, double)
{
	painter->setPen(QPen(color));
	//painter_path
}

void Graphics::moveTo(double x, double y)
{
	painter_path->moveTo(x, y);
}

void Graphics::lineTo(double x, double y)
{
	painter_path->lineTo(x, y);
}

void Graphics::drawString(const QString &text, int x, int y)
{
	painter_path->addText(x, y, font, text);
}

void Graphics::end(uint32_t color)
{
	painter->setPen(QPen(color));
	painter->drawPath(*painter_path);

	this->painter_path->~QPainterPath();
	new(this->painter_path) QPainterPath;

	//delete this->painter_path;
	//this->painter_path = new QPainterPath;
}

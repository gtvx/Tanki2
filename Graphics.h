#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

class QPainter;
class QPainterPath;
class QString;

class Graphics
{
	QPainter *painter;
	QPainterPath *painter_path;
public:
	Graphics();
	void setPainter(QPainter*);
	void beginFill(uint32_t color, double alpha = 1.0);
	void moveTo(double x, double y);
	void lineTo(double x, double y);
	void drawString(const QString& text, int x, int y);
	//void lineStyle(double thickness = 0, uint color = 0);
	/*
	void lineStyle(double thickness = NaN,
				   uint color = 0,
				   double alpha: = 1.0,
				   bool pixelHinting = false,
				   QString scaleMode = "normal",
				   QString caps = null,
				   QString joints = null,
				   double miterLimit = 3);
				   */
	void end(uint32_t color);
};

#endif // GRAPHICS_H

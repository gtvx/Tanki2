#ifndef CANVAS_H
#define CANVAS_H

class Graphics;

class Canvas
{
public:
	Canvas(Graphics *gfx);
	Graphics *gfx;
};

#endif // CANVAS_H

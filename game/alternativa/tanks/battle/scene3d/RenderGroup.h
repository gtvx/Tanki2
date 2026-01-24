#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <QVector>

class Renderer;
class DeferredAction;

class RenderGroup
{
	int size;
	bool rendering;

	QVector<Renderer*> renderers;
	QVector<DeferredAction*> deferredActions;


public:
	RenderGroup();

	void addRenderer(Renderer *renderer);
	void removeRenderer(Renderer *renderer);
	void render(int _arg_1, int _arg_2);

private:
	void executeDeferredActions();
};

#endif // RENDERGROUP_H

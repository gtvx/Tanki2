#ifndef DEFERREDRENDERERADDITION_H
#define DEFERREDRENDERERADDITION_H

#include "../DeferredAction.h"

class RenderGroup;
class Renderer;

class DeferredRendererAddition : public DeferredAction
{
	RenderGroup *renderGroup;
	Renderer *renderer;

public:
	DeferredRendererAddition(RenderGroup *renderGroup, Renderer *renderer);
	void execute() override;
};

#endif // DEFERREDRENDERERADDITION_H

#ifndef DEFERREDRENDERERDELETION_H
#define DEFERREDRENDERERDELETION_H

#include "../DeferredAction.h"

class RenderGroup;
class Renderer;

class DeferredRendererDeletion : public DeferredAction
{
	RenderGroup *renderGroup;
	Renderer *renderer;

public:
	DeferredRendererDeletion(RenderGroup *renderGroup, Renderer *renderer);

	void execute() override;
};

#endif // DEFERREDRENDERERDELETION_H

#include "DeferredRendererAddition.h"
#include "RenderGroup.h"

DeferredRendererAddition::DeferredRendererAddition(RenderGroup *renderGroup, Renderer *renderer)
{
	this->renderGroup = renderGroup;
	this->renderer = renderer;
}

void DeferredRendererAddition::execute()
{
	this->renderGroup->addRenderer(this->renderer);
}

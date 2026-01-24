#include "DeferredRendererDeletion.h"
#include "RenderGroup.h"

DeferredRendererDeletion::DeferredRendererDeletion(RenderGroup *renderGroup, Renderer *renderer)
{
	this->renderGroup = renderGroup;
	this->renderer = renderer;
}

void DeferredRendererDeletion::execute()
{
	this->renderGroup->removeRenderer(this->renderer);
}

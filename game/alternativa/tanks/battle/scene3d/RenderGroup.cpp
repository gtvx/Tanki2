#include "RenderGroup.h"
#include "../DeferredAction.h"
#include "DeferredRendererAddition.h"
#include "DeferredRendererDeletion.h"
#include "Renderer.h"


RenderGroup::RenderGroup()
{
	size = 0;
	rendering = false;
}


void RenderGroup::addRenderer(Renderer *renderer)
{
	if (this->rendering)
	{
		this->deferredActions.append(new DeferredRendererAddition(this, renderer));
	}
	else
	{
		if (this->renderers.indexOf(renderer) < 0)
		{
			int i = this->size++;
			if (this->renderers.length() <= i)
				this->renderers.resize(i + 1);
			this->renderers[i] = renderer;
		}
	}
}


void RenderGroup::removeRenderer(Renderer *renderer)
{
	if (this->rendering)
	{
		this->deferredActions.append(new DeferredRendererDeletion(this, renderer));
	}
	else
	{
		int i = this->renderers.indexOf(renderer);
		if (i >= 0)
		{
			this->renderers[i] = this->renderers[--this->size];
			this->renderers[this->size] = nullptr;
		}
	}
}


void RenderGroup::render(int _arg_1, int _arg_2)
{
	this->rendering = true;
	int i = 0;
	while (i < this->size)
	{
		Renderer *renderer = this->renderers[i];
		renderer->render(_arg_1, _arg_2);
		i++;
	}
	this->rendering = false;
	this->executeDeferredActions();
}


void RenderGroup::executeDeferredActions()
{
	for (DeferredAction *item : this->deferredActions)
		item->execute();

	this->deferredActions.clear();
}

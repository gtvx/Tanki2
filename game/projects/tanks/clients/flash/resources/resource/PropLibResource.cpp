#include "PropLibResource.h"
#include "game/alternativa/proplib/PropLibrary.h"
#include "game/alternativa/proplib/utils/ByteArrayMap.h"
#include "game/alternativa/proplib/types/PropGroup.h"
#include "game/alternativa/proplib/types/PropData.h"
#include "game/alternativa/proplib/types/PropState.h"
#include "game/alternativa/proplib/objects/PropObject.h"
#include "Object3D_isType.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "game/projects/tanks/clients/flash/resources/resource/ResourceParams.h"
#include "entrance/platform/client/fp10/core/resource/tara/TARAParser.h"
#include <QDebug>


static void prepareMeshes(PropGroup *_arg_1)
{
	if (_arg_1->groups != nullptr)
	{
		for (PropGroup *item : *_arg_1->groups)
		{
			prepareMeshes(item);
		}
	}

	if (_arg_1->props != nullptr)
	{
		for (PropData *item : *_arg_1->props)
		{
			PropObject *_local_4 = item->getDefaultState()->getDefaultObject();

			std::shared_ptr<Object3D> object = _local_4->object;

			if (Object3D_isType<Mesh>(object.get()))
			{
				std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(object);

				mesh->calculateVerticesNormalsByAngle(ResourceParams::ANGLE_THRESHOLD, 0.01);
			}
		}
	}
}


PropLibResource::PropLibResource(ResourceInfo *resourceInfo) :
	Resource(resourceInfo)
{
	this->lib = nullptr;
}


void PropLibResource::createLibrary(const QByteArray &data)
{
	TARAParser *tara = new TARAParser(data);
	ByteArrayMap *files = new ByteArrayMap(tara->data());
	this->lib = new PropLibrary(files);
	prepareMeshes(this->lib->getRootGroup());
}

#include "Billboards.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"


static const std::string BILLBOARD_MATERIAL_NAME = "display";


Billboards::Billboards()
{
	billboardImage = nullptr;
}


void Billboards::add(std::shared_ptr<Mesh> mesh)
{
	this->billboards.append(mesh);

	for (Shared<Face> face : mesh->getFaces())
	{
		if (face->material->name == BILLBOARD_MATERIAL_NAME)
		{
			this->faces.append(face);
			if (this->material == nullptr)
			{
				this->material = std::dynamic_pointer_cast<TextureMaterial>(face->material);
				this->updateMaterial();
			}
		}
	}
}


void Billboards::setImage(std::shared_ptr<BitmapData> bitmapData)
{
	this->billboardImage = bitmapData;
	this->updateMaterial();
}


void Billboards::updateMaterial()
{
	if (((!(this->material == nullptr)) && (!(this->billboardImage == nullptr))))
	{
		this->material->setTexture(this->billboardImage);
		this->setMaterialResolution();
	}
}


void Billboards::setMaterialResolution()
{
	if (this->billboards.length() > 0)
	{
		this->material->resolution = 1;
	}
}

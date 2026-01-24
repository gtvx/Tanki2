#ifndef SKINMATERIALS_H
#define SKINMATERIALS_H

#include <memory>

class TextureMaterial;

class SkinMaterials
{
public:

	std::shared_ptr<TextureMaterial> hullMaterial;
	std::shared_ptr<TextureMaterial> turretMaterial;
	std::shared_ptr<TextureMaterial> leftTrackMaterial;
	std::shared_ptr<TextureMaterial> rightTrackMaterial;

	SkinMaterials(std::shared_ptr<TextureMaterial> hullMaterial,
				  std::shared_ptr<TextureMaterial> turretMaterial,
				  std::shared_ptr<TextureMaterial> leftTrackMaterial = nullptr,
				  std::shared_ptr<TextureMaterial> rightTrackMaterial = nullptr)
	{
		this->hullMaterial = hullMaterial;
		this->turretMaterial = turretMaterial;
		this->leftTrackMaterial = leftTrackMaterial;
		this->rightTrackMaterial = rightTrackMaterial;
	}
};

#endif // SKINMATERIALS_H

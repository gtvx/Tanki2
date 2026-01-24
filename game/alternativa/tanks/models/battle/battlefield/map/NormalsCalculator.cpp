#include "NormalsCalculator.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Face.h"


namespace
{
	std::unordered_map<std::string, bool> map =
	{
		{"hang_1", true},
		{"hang_2", true},
		{"hang_3", true},
		{"SmHouse008", true},
		{"wall_broke_1", true},
		{"wall_broke_2", true},
		{"Bk_roof1", true},
		{"Bk_roof2", true},
		{"Tree01", true},
		{"Tree02", true},
		{"tube_1", true},
		{"tube_2", true},
		{"tube_3", true},
		{"tube_cor", true},
		{"cliff_3", true},
		{"cliff_1", true},
		{"cliff_2", true},
		{"cliff_4", true},
		{"cliff_inco", true},
		{"cliff_cor", true},
		{"cliff_r2", true},
		{"cliff_ri", true},
		{"Big_Rock04", true},
		{"Big_Rock", true},
		{"Change01", true},
		{"Change02", true},
		{"Corn1", true},
		{"Corn2", true},
		{"Corn3", true},
		{"Corn4", true},
		{"Corn_B", true},
		{"crater", true},
		{"Land02", true},
		{"Land03", true},
		{"Land04", true},
		{"Land05", true},
		{"Land06", true},
		{"Land07", true},
		{"Land08", true},
		{"Land09", true},
		{"Land22", true},
		{"Land33", true},
		{"Line", true},
		{"Med_Rock", true},
		{"rise_g1", true},
		{"rise_g2", true},
		{"rise_g3", true},
		{"rise_g4", true},
		{"rise_g5", true},
		{"rise_g6", true},
		{"Rise_gr1", true},
		{"Rise_gr2", true},
		{"rise_r1", true},
		{"sm_rock1", true},
		{"sm_rock2", true},
		{"up_Brock", true},
		{"Up_Rock", true},
		{"Up_Rock1", true},
		{"Up_Rock2", true},
		{"Up_Rock14", true}
	};

}

NormalsCalculator::NormalsCalculator()
{

}

void NormalsCalculator::calculateNormals(QVector<std::shared_ptr<Object3D>> &list)
{
	this->clearSmoothingGroups(list);
	Mesh::calculateVerticesNormalsBySmoothingGroupsForMeshList(list, 0.01);
}

void NormalsCalculator::clearSmoothingGroups(QVector<std::shared_ptr<Object3D>> &list)
{
	for (std::shared_ptr<Object3D> object3d : list)
	{
		if (map[object3d->name] == false)
		{
			std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(object3d);
			for (Shared<Face> &_local_4 : mesh->getFaces())
			{
				_local_4->smoothingGroups = 0;
			}
		}
	}
}


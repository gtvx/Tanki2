#ifndef HELPERMESH_H
#define HELPERMESH_H

#include "hardware/alternativa/engine3d/objects/BSP.h"

class HelperMesh : public BSP
{
public:
	HelperMesh();
};

std::shared_ptr<HelperMesh> new_HelperMesh();

#endif // HELPERMESH_H

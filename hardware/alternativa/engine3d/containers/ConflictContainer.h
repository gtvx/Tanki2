#ifndef CONFLICTCONTAINER_H
#define CONFLICTCONTAINER_H

#include "hardware/alternativa/engine3d/core/Object3DContainer.h"
#include "shared.h"

class ConflictContainer : public Object3DContainer
{
public:
	ConflictContainer();
	double threshold;

	bool resolveByAABB;
	bool resolveByOOBB;

	void draw(Camera3D *camera) override;

	std::shared_ptr<Object3D> clone() const;

private:
	Shared<VG> sortGeometry(Shared<VG> _arg_1, bool _arg_2, bool _arg_3);

protected:
	void drawOOBBGeometry(Camera3D *_arg_1, Shared<VG> _arg_2);
	void drawConflictGeometry(Camera3D *_arg_1, Shared<VG> _arg_2);
	void drawAABBGeometry(Camera3D *_arg_1,
						  Shared<VG> _arg_2,
						  bool _arg_3 = true,
						  bool _arg_4 = false,
						  bool _arg_5 = true,
						  int _arg_6 = -1);
};

#endif // CONFLICTCONTAINER_H

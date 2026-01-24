#ifndef VERTICALANGLESADAPT_H
#define VERTICALANGLESADAPT_H

#include "VerticalAngles.h"
#include <memory>

class IGameObject;

class VerticalAnglesAdapt : public VerticalAngles
{
	IGameObject *object;
	std::shared_ptr<VerticalAngles> impl;

public:
	VerticalAnglesAdapt(IGameObject *object, std::shared_ptr<VerticalAngles> impl);

private:
	double getAngleUp() override;
	double getAngleDown() override;
};

#endif // VERTICALANGLESADAPT_H

#include "FillMaterial.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "Canvas.h"
#include "Graphics.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"


std::shared_ptr<FillMaterial> new_FillMaterial(int color, double lineThickness, double alpha, int lineColor)
{
	return std::make_shared<FillMaterial>(color, lineThickness, alpha, lineColor);
}


FillMaterial::FillMaterial(int color, double alpha, double lineThickness, int lineColor) :
	TextureMaterial(5)
{
	this->color = color;
	this->alpha = alpha;
	this->lineThickness = lineThickness;
	this->lineColor = lineColor;
}


void FillMaterial::init(DrawInitParams*)
{

}


void FillMaterial::draw(Camera3D *param1, Canvas *param2, Shared<Face> param3, double)
{
	qDebug("FillMaterial::draw");

	double _loc5_ = param1->viewSizeX;
	double _loc6_ = param1->viewSizeY;

	//if (this->lineThickness >= 0)
		//param2->gfx->lineStyle(this->lineThickness, this->lineColor);

	Shared<Face> _loc8_ = param3;
	while (_loc8_ != nullptr)
	{
		Shared<Face> _loc7_ = _loc8_->processNext;
		_loc8_->processNext = nullptr;
		Shared<Wrapper> _loc9_ = _loc8_->wrapper;
		Vertex *_loc10_ = _loc9_->vertex.get();
		param2->gfx->beginFill(this->color, this->alpha);
		param2->gfx->moveTo(_loc10_->cameraX * _loc5_ / _loc10_->cameraZ, _loc10_->cameraY * _loc6_ / _loc10_->cameraZ);
		int _loc11_ = -1;
		_loc9_ = _loc9_->next;

		while (_loc9_ != nullptr) {
			_loc10_ = _loc9_->vertex.get();
			param2->gfx->lineTo(_loc10_->cameraX * _loc5_ / _loc10_->cameraZ, _loc10_->cameraY * _loc6_ / _loc10_->cameraZ);
			_loc11_++;
			_loc9_ = _loc9_->next;
		}

		_loc10_ = _loc8_->wrapper->vertex.get();
		param2->gfx->lineTo(_loc10_->cameraX * _loc5_ / _loc10_->cameraZ, _loc10_->cameraY * _loc6_ / _loc10_->cameraZ);
		param1->numTriangles = param1->numTriangles + _loc11_;
		param1->numPolygons++;
		_loc8_ = _loc7_;
	}

	if (this->lineThickness >= 0)
		param2->gfx->end(this->lineColor);

	//if (this->lineThickness >= 0)
		//param2->gfx->lineStyle();

	param1->numDraws++;
}

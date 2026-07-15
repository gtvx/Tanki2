#include "AnimatedPaintMaterial.h"
#include "flash/display/BitmapData.h"
#include "MyMath.h"
#include "utils/float_array.h"
#include <QDateTime>
#include "hardware/alternativa/engine3d/materials/DrawParams.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "getTimer.h"


std::shared_ptr<AnimatedPaintMaterial> new_AnimatedPaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
																 std::shared_ptr<BitmapData> lightMapBitmap,
																 std::shared_ptr<BitmapData> texture,
																 int numFramesX,
																 int numFramesY,
																 int fps,
																 int numFrames,
																 int mipMapping)
{
	return std::make_shared<AnimatedPaintMaterial>(spriteSheetBitmap,
												   lightMapBitmap,
												   texture,
												   numFramesX,
												   numFramesY,
												   fps,
												   numFrames,
												   mipMapping);
}



AnimatedPaintMaterial::AnimatedPaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
											 std::shared_ptr<BitmapData> lightMapBitmap,
											 std::shared_ptr<BitmapData> texture,
											 int numFramesX,
											 int numFramesY,
											 int fps,
											 int numFrames,
											 int mipMapping) :
	PaintMaterial(spriteSheetBitmap, lightMapBitmap, texture, mipMapping)
{
	this->lastFrame = -1;
	this->numFramesX = numFramesX;
	this->numFramesY = numFramesY;
	this->fps = fps;
	this->numFrames = numFrames;
	this->currentFrame = 0;


	this->scaleX = (double(texture->width()) / double(spriteSheetBitmap->width()));
	this->scaleY = (double(texture->height()) / double(spriteSheetBitmap->height()));


	this->frameWidth = (1. / numFramesX);
	this->frameHeight = (1. / numFramesY);
	this->time = getTimer();


	float_array_set_8(fragConst, 0, 0.5, 1, 2, this->frameWidth, this->frameHeight, 0, 0);
}


void AnimatedPaintMaterial::init(DrawInitParams *p)
{
	PaintMaterial::init(p);
}


void AnimatedPaintMaterial::update()
{
	int currentTime = getTimer();
	int tick = currentTime - this->time;
	this->time = currentTime;
	this->currentFrame = this->currentFrame + ((tick / 1000.) * this->fps);

	this->currentFrame = MyMath::fmod(this->currentFrame, this->numFrames); //this->currentFrame % this->numFrames;


	if (this->lastFrame == this->currentFrame)
	{
		return;
	}

	int width = MyMath::fmod(this->currentFrame, this->numFramesX); //this->currentFrame % this->numFramesX;
	int height = int((double(this->currentFrame) / this->numFramesY));


	this->lastFrame = this->currentFrame;

	uvTransformConst[0] = this->scaleX;
	uvTransformConst[1] = 0;
	uvTransformConst[2] = width * this->frameWidth;
	uvTransformConst[3] = 0;

	uvTransformConst[4] = 0;
	uvTransformConst[5] = this->scaleY;
	uvTransformConst[6] = height * this->frameHeight;
	uvTransformConst[7] = 0;


	fragConst[6] = uvTransformConst[2];
	fragConst[7] = uvTransformConst[6];
}


void AnimatedPaintMaterial::drawOpaqueVulkan(DrawParams *p)
{
	this->update();
	PaintMaterial::drawOpaqueVulkan(p);
}


void AnimatedPaintMaterial::drawTransparentVulkan(DrawParams *p)
{
	this->update();

	float_array_set_12(p->object->transformConst, 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0);

	PaintMaterial::drawTransparentVulkan(p);
}

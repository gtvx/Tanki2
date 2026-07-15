#ifndef ANIMATEDPAINTMATERIAL_H
#define ANIMATEDPAINTMATERIAL_H

#include "PaintMaterial.h"

class AnimatedPaintMaterial : public PaintMaterial
{
	double currentFrame;
	double frameWidth;
	double frameHeight;

	int lastFrame;
	int fps;
	int numFrames;
	int numFramesX;
	int numFramesY;
	int time;

public:

	double scaleX;
	double scaleY;


	AnimatedPaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
						  std::shared_ptr<BitmapData> lightMapBitmap,
						  std::shared_ptr<BitmapData> texture,
						  int numFramesX,
						  int numFramesY,
						  int fps,
						  int numFrames,
						  int mipMapping = 0);

	void init(DrawInitParams*) override;

private:
	void update();
	void drawOpaqueVulkan(DrawParams *p) override;
	void drawTransparentVulkan(DrawParams *p) override;
};


std::shared_ptr<AnimatedPaintMaterial> new_AnimatedPaintMaterial(std::shared_ptr<BitmapData> spriteSheetBitmap,
																 std::shared_ptr<BitmapData> lightMapBitmap,
																 std::shared_ptr<BitmapData> texture,
																 int numFramesX,
																 int numFramesY,
																 int fps,
																 int numFrames,
																 int mipMapping = 0);

#endif // ANIMATEDPAINTMATERIAL_H

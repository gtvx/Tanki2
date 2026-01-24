#ifndef RESOURCEIMAGEFRAMEPARAMS_H
#define RESOURCEIMAGEFRAMEPARAMS_H

class ResourceImageFrameParams
{
	double _fps;
	int _frameHeight;
	int _frameWidth;
	int _imageHeight;
	int _imageWidth;
	int _numFrames;

public:
	ResourceImageFrameParams(double fps,
							 int frameHeight,
							 int frameWidth,
							 int imageHeight,
							 int imageWidth,
							 int numFrames)
	{
		_fps = fps;
		_frameHeight = frameHeight;
		_frameWidth = frameWidth;
		_imageHeight = imageHeight;
		_imageWidth = imageWidth;
		_numFrames = numFrames;
	}


	double fps() const { return _fps; }
	int frameHeight() const { return _frameHeight; }
	int frameWidth() const { return _frameWidth; }
	int imageHeight() const { return _imageHeight; }
	int imageWidth() const { return _imageWidth; }
	int numFrames() const { return _numFrames; }
};

#endif // RESOURCEIMAGEFRAMEPARAMS_H

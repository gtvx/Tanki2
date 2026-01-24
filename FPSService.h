#ifndef FPSSERVICE_H
#define FPSSERVICE_H


class FPSService
{
	int fps;
	double frameTime;
	int lastTime;
	int numFrames;
	bool deactiave;
	bool starting;

public:
	FPSService();
	void start();
	void onEnterFrame();
	int getFps();
	double getFrameTimeMS();

private:
	void setFps(int v);
};

#endif // FPSSERVICE_H

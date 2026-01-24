#ifndef PERFORMANCEMONITOR_H
#define PERFORMANCEMONITOR_H

class PerformanceMonitor
{
	int framesToAverage;
	int frameStartTime;
	int cumulativeTimeInMs;
	int frameCounter;
	int minFrameTimeInMs;
	int maxFrameTimeInMs;
	double averageFrameTimeInMs;

public:
	PerformanceMonitor(int framesToAverage);
	void beginFrame();
	void endFrame();
	double getAverageFrameTimeInMs();
	double getAverageFps();
	void reset();

private:
	void updateMinMaxTime(int);
};

#endif // PERFORMANCEMONITOR_H

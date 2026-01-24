#include "PerformanceMonitor.h"
#include "getTimer.h"


PerformanceMonitor::PerformanceMonitor(int framesToAverage)
{
	this->framesToAverage = framesToAverage;
	this->reset();
}


void PerformanceMonitor::beginFrame()
{
	this->frameStartTime = getTimer();
}


void PerformanceMonitor::endFrame()
{
	int v = (getTimer() - this->frameStartTime);
	this->cumulativeTimeInMs = (this->cumulativeTimeInMs + v);
	this->updateMinMaxTime(v);
	if (++this->frameCounter >= this->framesToAverage) {
		this->averageFrameTimeInMs = this->cumulativeTimeInMs / this->frameCounter;
		this->cumulativeTimeInMs = 0;
		this->frameCounter = 0;
	}
}


double PerformanceMonitor::getAverageFrameTimeInMs()
{
	return this->averageFrameTimeInMs;
}


double PerformanceMonitor::getAverageFps()
{
	return 1000 / this->averageFrameTimeInMs;
}


void PerformanceMonitor::updateMinMaxTime(int v)
{
	if (v > this->maxFrameTimeInMs) {
		this->maxFrameTimeInMs = v;
	} else {
		if (v < this->minFrameTimeInMs)
			this->minFrameTimeInMs = v;
	}
}


void PerformanceMonitor::reset()
{
	this->minFrameTimeInMs = 0;
	this->maxFrameTimeInMs = 0;
	this->averageFrameTimeInMs = 0;
	this->frameCounter = 0;
	this->cumulativeTimeInMs = 0;
}

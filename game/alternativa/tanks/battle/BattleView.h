#ifndef BATTLEVIEW_H
#define BATTLEVIEW_H

#include <QString>

class AxisIndicator;
class Canvas;
class FPSService;

class BattleView
{
	AxisIndicator *axisIndicator;
	bool showAxisIndicator;
	char last_fps;
	QString fps_str;

public:
	BattleView();
	void setShowAxisIndicator(bool enable);
	void setShowFPSIndicator(bool enable);
	void update(Canvas *canvas, FPSService *fpsService);

private:
	void setAxisIndicatorPosition();
};

#endif // BATTLEVIEW_H

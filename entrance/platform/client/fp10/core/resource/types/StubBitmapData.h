#ifndef STUBBITMAPDATA_H
#define STUBBITMAPDATA_H

#include "flash/display/BitmapData.h"

class StubBitmapData : public BitmapData
{
public:
	StubBitmapData(uint32_t color, int width = 20, int height = 20);
};

#endif // STUBBITMAPDATA_H

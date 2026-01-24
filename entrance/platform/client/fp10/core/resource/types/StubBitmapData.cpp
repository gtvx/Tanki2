#include "StubBitmapData.h"

StubBitmapData::StubBitmapData(uint32_t color, int width, int height) :
	BitmapData(width, height, false, color)
{
	int _local_4 = 0;
	while (_local_4 < width)
	{
		int _local_5 = 0;
		while (_local_5 < height)
		{
			setPixel(((_local_4 % 2) ? _local_5 : (_local_5 + 1)), _local_4, color);
			_local_5 = (_local_5 + 2);
		}
		_local_4++;
	}
}

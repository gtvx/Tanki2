#include "getTimer.h"
#include <QDateTime>

static qint64 first = QDateTime::currentMSecsSinceEpoch();

int getTimer()
{
	return QDateTime::currentMSecsSinceEpoch() - first;
}

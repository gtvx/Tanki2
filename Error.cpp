#include "Error.h"
#include <QDebug>


Error::Error(int code)
{
	qDebug("Error %d", code);
}

Error::Error(const char *text)
{
	qDebug("Error %s", text);
}

Error::Error(const QString &text)
{
	qDebug() << "Error" << text;
}



IllegalOperationError::IllegalOperationError()
{
	qDebug() << "IllegalOperationError";
}



ArgumentError::ArgumentError(const char *str)
{
	qDebug("ArgumentError %s", str);
}

ArgumentError::ArgumentError(const QString &str)
{
	qDebug() << "ArgumentError" << str;
}



RangeError::RangeError(const char *text)
{
	qDebug() << text;
}



TypeError::TypeError(const char*)
{
	qDebug("TypeError");
}


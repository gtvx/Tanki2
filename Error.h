#ifndef ERROR_H
#define ERROR_H

class QString;

class Error
{
public:
	Error(int);
	Error(const char*);
	Error(const QString &);
};

class IllegalOperationError
{
public:
	IllegalOperationError();
};

class ArgumentError
{
public:
	ArgumentError(const char *str = nullptr);
	ArgumentError(const QString &str);
};

class RangeError
{
public:
	RangeError(const char *text);
};

class TypeError
{
public:
	TypeError(const char*);
};

#endif // ERROR_H

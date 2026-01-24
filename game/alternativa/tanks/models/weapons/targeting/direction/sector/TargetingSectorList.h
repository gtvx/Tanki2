#ifndef TARGETINGSECTORLIST_H
#define TARGETINGSECTORLIST_H

#include "TargetingSector.h"

class TargetingSector;

class TargetingSectorList
{
	TargetingSector *_list;
	int _count;
	int _reserve;

public:
	TargetingSectorList();
	~TargetingSectorList();
	void clear();
	TargetingSector* add();
	TargetingSector* at(int i) { return &_list[i]; }
	int count() const { return _count; }
};

#endif // TARGETINGSECTORLIST_H

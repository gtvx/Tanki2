#include "TargetingSectorList.h"


TargetingSectorList::TargetingSectorList()
{
	_count = 0;
	_reserve = 0;
	_list = nullptr;
}


TargetingSectorList::~TargetingSectorList()
{
	if (_list != nullptr)
		delete []_list;
}


void TargetingSectorList::clear()
{
	_count = 0;
}


TargetingSector* TargetingSectorList::add()
{
	int d = _count;
	_count++;
	if (_reserve <= _count)
	{
		_reserve = _count * 2;

		TargetingSector *list = new TargetingSector[_reserve];
		if (_list == nullptr)
		{
			_list = list;
		}
		else
		{
			int c = _count - 1;
			for (int i = 0; i < c; i++)
				list[i] = _list[i];

			delete _list;
			_list = list;
		}
	}

	return &_list[d];
}

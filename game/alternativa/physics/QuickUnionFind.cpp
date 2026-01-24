#include "QuickUnionFind.h"


QuickUnionFind::QuickUnionFind()
{
	items.resize(1);
	size.resize(1);
}


void QuickUnionFind::init(int param1)
{
	this->items.resize(param1);
	this->size.resize(param1);
	int index = 0;
	while (index < param1)
	{
		this->items[index] = index;
		this->size[index] = 1;
		index++;
	}
}


void QuickUnionFind::_union(int param1, int param2)
{
	if (!this->connected(param1, param2))
	{
		int _loc3_ = this->root(param1);
		int _loc4_ = this->root(param2);
		int _loc5_ = this->size[_loc3_];
		int _loc6_ = this->size[_loc4_];
		if (_loc5_ > _loc6_)
		{
			this->items[_loc4_] = _loc3_;
			this->size[_loc3_] = this->size[_loc3_] + _loc6_;
		}
		else
		{
			this->items[_loc3_] = _loc4_;
			this->size[_loc4_] = this->size[_loc4_] + _loc5_;
		}
	}
}


bool QuickUnionFind::connected(int param1, int param2)
{
	return this->root(param1) == this->root(param2);
}


int QuickUnionFind::root(int param1)
{
	int v = param1;
	while (this->items[v] != v)
	{
		v = this->items[v];
	}
	return v;
}

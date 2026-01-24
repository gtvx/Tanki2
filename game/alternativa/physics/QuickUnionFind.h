#ifndef QUICKUNIONFIND_H
#define QUICKUNIONFIND_H

#include <QVector>

class QuickUnionFind
{
	QVector<int> items;
	QVector<int> size;

public:
	QuickUnionFind();
	void init(int param1);
	void _union(int param1, int param2);
	bool connected(int param1, int param2);
	int root(int param1);
};

#endif // QUICKUNIONFIND_H

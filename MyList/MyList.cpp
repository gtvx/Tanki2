#include "MyList.h"


static void memcpy(void **dst, void **src, int count)
{
	for (int i = 0; i < count; i++)
		dst[i] = src[i];
}

static void memset0(void **dst, int count)
{
	for (int i = 0; i < count; i++)
		dst[i] = nullptr;
}

void MyListPrivate::alloc(int count, bool down)
{
	if (this->_alloc == count)
		return;

	if (down == true && count == 0) {
		if (_list != nullptr) {
			delete this->_list;
			this->_list = nullptr;
		}
		this->_alloc = 0;
		this->_count = 0;
		return;
	}

	if (down == true || this->_alloc < count) {
		void **list = new void*[count];
		this->_alloc = count;
		if (_list != nullptr) {
			memcpy(list, this->_list, _count);
			delete this->_list;
			memset0(&list[_count], count - _count);
		} else {
			memset0(list, count);
		}
		this->_list = list;
	}
}

MyListPrivate::MyListPrivate(int count)
{
	this->_list = nullptr;
	this->_alloc = this->_count = 0;
	resize(count);
}

MyListPrivate::~MyListPrivate()
{
	if (this->_list != nullptr)
		delete []this->_list;
}

void MyListPrivate::append(void *t)
{
	const int c = this->_count + 1;
	if (this->_alloc < c)
		this->alloc(c * 2, false);
	this->_list[this->_count] = t;
	this->_count++;
}

void MyListPrivate::set(int index, void *t)
{
	this->_list[index] = t;
}

void MyListPrivate::add_set(int index, void *t)
{
	if (index >= _count) {
		const int c = index + 1;
		if (this->_alloc < c)
			this->alloc(c * 2, false);
		_count = index + 1;
	}

	this->_list[index] = t;
}

bool MyListPrivate::contains(void *t) const
{
	for (int i = 0; i < this->_count; i++)
		if (this->_list[i] == t)
			return true;
	return false;
}

void MyListPrivate::removeIndex(int i)
{
	for (i++; i < this->_count; i++)
		this->_list[i-1] = this->_list[i];
	this->_count--;
}

void MyListPrivate::removeAtSort(void *t)
{
	for (int i = 0; i < this->_count; i++) {
		if (this->_list[i] == t) {
			this->removeIndex(i);
			break;
		}
	}
}

void MyListPrivate::removeAt(void *t)
{
	const int index = indexOf(t);
	if (index > -1)
		this->_list[index] = this->_list[--_count];
}

void MyListPrivate::clear()
{
	this->_count = 0;
}

void MyListPrivate::resize(int count)
{
	if (count == 0) {
		this->_count = count;
		return;
	}

	if (this->_count > count) {
		this->_count = count;
	} else if (this->_count < count) {
		if (this->_alloc < count) {
			this->alloc(count, false);
		}
		this->_count = count;
	}
}

void MyListPrivate::reserve(int count)
{
	this->alloc(count, true);
}

int MyListPrivate::indexOf(void *t)
{
	for (int i = 0; i < _count; i++) {
		if (_list[i] == t)
			return i;
	}
	return -1;
}

void MyListPrivate::copy(const MyListPrivate *list)
{
	clear();
	const int count = list->count();
	this->resize(count);
	for (int i = 0; i < count; i++)
		this->_list[i] = list->_list[i];
}


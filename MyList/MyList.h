#ifndef MYLIST_H
#define MYLIST_H


class MyListPrivate
{
	int _alloc;
	void alloc(int count, bool down);

protected:
	int _count;
	void **_list;
	void append(void*);
	void set(int index, void*);
	void add_set(int index, void*);
	int indexOf(void*);
	void copy(const MyListPrivate*);
	void removeAtSort(void *t);
	void removeAt(void *t);
	bool contains(void*) const;

public:
	MyListPrivate(int count = 0);
	~MyListPrivate();
	void removeIndex(int i);
	void clear();
	void resize(int count);
	void reserve(int count);
	int count() const { return this->_count; }
};


template <typename T>
class MyList : public MyListPrivate
{
public:
	MyList(int count = 0) : MyListPrivate(count) {}
	MyList(const MyList&) = delete;
	void operator=(const MyList&) = delete;
	void append(T t) { MyListPrivate::append((void*)t); }
	void set(int index, T t) { MyListPrivate::set(index, (void*)t); }
	void add_set(int index, T t) { MyListPrivate::add_set(index, (void*)t); }
	bool contains(T t) { return MyListPrivate::contains((void*)t); }
	T at(int index) const { return (T)_list[index];	}
	void removeAtSort(T t) { MyListPrivate::removeAtSort((void*)t); }
	void removeAt(T t) { MyListPrivate::removeAt((void*)t); }
	int indexOf(T t) { return MyListPrivate::indexOf((void*)t); }
	void copy(const MyList<T> *t) { MyListPrivate::copy(t); }
};

#endif // MYLIST_H

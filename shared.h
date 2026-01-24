#ifndef SHARED_H
#define SHARED_H

#include <cstddef>


class SharedObject
{
	int _count_ref;

public:

	SharedObject()
	{
		_count_ref = 0;
	}

	int count()
	{
		return _count_ref;
	}

	void ref_add()
	{
		_count_ref++;
	}

	bool ref_release()
	{
		return --_count_ref != 0;
	}
};


template <typename T>
class Shared
{
	T *m_ptr;

public:

	Shared() :
		m_ptr(nullptr)
	{

	}

	Shared(const std::nullptr_t) :
		m_ptr(nullptr)
	{

	}

	void reset(T *ptr)
	{
		this->m_ptr = ptr;
		ptr->ref_add();
	}

	void reset()
	{
		m_ptr = nullptr;
	}

	Shared(const Shared<T> &other)
	{
		if (other.m_ptr != nullptr)
		{
			other.m_ptr->ref_add();
			m_ptr = other.m_ptr;
		}
		else
		{
			m_ptr = nullptr;
		}
	}

	Shared(Shared<T> &other)
	{
		if (other.m_ptr != nullptr)
		{
			other.m_ptr->ref_add();
			m_ptr = other.m_ptr;
		}
		else
		{
			m_ptr = nullptr;
		}
	}

	Shared(Shared<T> &&other) noexcept
	{
		if (other.m_ptr != nullptr)
		{
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}
		else
		{
			m_ptr = nullptr;
		}
	}

	~Shared()
	{
		if (m_ptr != nullptr)
		{
			if (!m_ptr->ref_release())
			{
				m_ptr->remove();
			}
		}
	}

	T* operator->() const
	{
		return m_ptr;
	}

	T* get() const
	{
		return m_ptr;
	}

	void set(T *other)
	{
		if (m_ptr != nullptr)
		{

			if (!m_ptr->ref_release())
			{
				m_ptr->remove();
			}
		}

		other->ref_add();
		m_ptr = other;
	}

	bool operator==(const std::nullptr_t) const
	{
		return this->m_ptr == nullptr;
	}

	bool operator!=(const std::nullptr_t) const
	{
		return this->m_ptr != nullptr;
	}

	bool operator==(const Shared<T> &other) const
	{
		return this->m_ptr == other.m_ptr;
	}

	bool operator!=(const Shared<T> &other) const
	{
		return this->m_ptr != other.m_ptr;
	}

	bool operator<(const Shared<T> &other) const
	{
		return this->m_ptr < other.m_ptr;
	}

	void operator=(const std::nullptr_t)
	{
		if (m_ptr != nullptr)
		{
			if (!m_ptr->ref_release())
			{
				m_ptr->remove();
			}
		}
		m_ptr = nullptr;
	}

	void operator=(const Shared<T> &other)
	{
		if (m_ptr != nullptr)
		{
			if (!m_ptr->ref_release())
			{
				m_ptr->remove();
			}
			m_ptr = nullptr;
		}

		if (other.m_ptr != nullptr)
		{
			other->ref_add();
		}

		m_ptr = other.m_ptr;
	}

};



template <typename T>
class Weak
{
	T *m_ptr;

public:
	Weak() : m_ptr(nullptr)
	{

	}

	Weak(const Shared<T> &other)
	{
		this->m_ptr = other.get();
	}

	Weak(const Shared<T> &&other)
	{
		this->m_ptr = other.get();
	}

	~Weak()
	{

	}

	T* operator->() const
	{
		return m_ptr;
	}

	T* get()
	{
		return m_ptr;
	}

	bool operator==(const std::nullptr_t)
	{
		return this->m_ptr == nullptr;
	}

	bool operator!=(const std::nullptr_t)
	{
		return this->m_ptr != nullptr;
	}

	void operator=(const std::nullptr_t)
	{
		m_ptr = nullptr;
	}

	bool operator==(const Shared<T> &other)
	{
		return this->m_ptr == other.m_ptr;
	}
};

#endif // SHARED_H

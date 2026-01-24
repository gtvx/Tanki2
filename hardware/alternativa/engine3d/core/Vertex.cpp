#include "Vertex.h"
#include <QString>


namespace
{
	class Collector
	{
	public:
		Shared<Vertex> collector;

		Collector()
		{

		}

		~Collector()
		{
			Shared<Vertex> temp = collector;

			while (temp != nullptr)
			{
				Shared<Vertex> next = temp->next;
				delete temp.get();
				temp.reset();
				temp = next;
			}

			collector.reset();
		}


		int count()
		{
			int count = 0;

			Weak<Vertex> temp = collector;

			while (temp != nullptr)
			{
				count++;
				temp = temp->next;
			}

			return count;
		}

	} c;
}


Vertex::Vertex()
{
	this->next = nullptr;
	this->value = nullptr;
	this->offset = 0;
	this->index = 0;
	this->drawId = 0;
	this->transformId = 0;
	this->cameraX = 0;
	this->cameraY = 0;
	this->cameraZ = 0;
	this->normalX = 0;
	this->normalY = 0;
	this->normalZ = 0;
}


Shared<Vertex> Vertex::create()
{
	Shared<Vertex> result;

	if (c.collector != nullptr)
	{
		result = c.collector;
		c.collector = result->next;
		result->next = nullptr;
	}
	else
	{
		result.reset(new Vertex);
	}

	result->transformId = 0;
	result->drawId = 0;
	result->value = nullptr;

	return result;
}


void Vertex::remove()
{
	auto temp = this;

	while (temp->next != nullptr)
	{
		if (temp->next->count() > 1)
		{
			break;
		}

		temp = temp->next.get();
	}

	temp->next = c.collector;
	c.collector.set(this);
}


QString Vertex::toString() const
{
	return QString();
}


Shared<Vertex> Vertex::createList(int count)
{
	Shared<Vertex> _local_2 = c.collector;

	if (_local_2 != nullptr)
	{
		Weak<Vertex> _local_3 = _local_2;
		while (count > 1)
		{
			_local_3->transformId = 0;
			_local_3->drawId = 0;
			if (_local_3->next == nullptr)
			{
				while (count > 1)
				{
					Shared<Vertex> result;
					result.reset(new Vertex);
					_local_3->next = result;
					_local_3 = _local_3->next;
					count--;
				}
				break;
			}
			_local_3 = _local_3->next;
			count--;
		}
		c.collector = _local_3->next;
		_local_3->transformId = 0;
		_local_3->drawId = 0;
		_local_3->next = nullptr;
	}
	else
	{
		_local_2 = Vertex::create();
		Shared<Vertex> _local_3 = _local_2;
		while (count > 1)
		{
			_local_3->next = Vertex::create();
			_local_3 = _local_3->next;
			count--;
		}
	}
	return _local_2;
}


void Vertex::clear()
{
	next = nullptr;
	value = nullptr;
}

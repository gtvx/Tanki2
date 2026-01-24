#include "Wrapper.h"
#include "Vertex.h"

namespace
{
	class Collector
	{
	public:
		Shared<Wrapper> collector;

		Collector()
		{

		}

		~Collector()
		{
			Shared<Wrapper> temp = collector;

			while (temp != nullptr)
			{
				Shared<Wrapper> next = temp->next;
				delete temp.get();
				temp.reset();
				temp = next;
			}

			collector.reset();
		}


		int count()
		{
			int count = 0;

			Weak<Wrapper> temp = collector;

			while (temp != nullptr)
			{
				count++;
				temp = temp->next;
			}

			return count;
		}

	} c;
}


Wrapper::Wrapper()
{

}


Shared<Wrapper> Wrapper::create()
{
	Shared<Wrapper> result;

	if (c.collector != nullptr)
	{
		result = c.collector;
		c.collector = result->next;
		result->next = nullptr;
	}
	else
	{
		result.reset(new Wrapper);
	}

	result->vertex = nullptr;

	return result;
}


void Wrapper::remove()
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

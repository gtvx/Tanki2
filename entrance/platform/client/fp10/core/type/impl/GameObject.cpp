#include "GameObject.h"
#include "Error.h"
#include <QDebug>
#include <typeinfo>

#ifdef __GNUC__
#include <cxxabi.h>
#endif


static std::string get_normal_class_name_std(const char *name)
{
#ifdef __GNUC__
	int status = 0;
	char *demangled_name = abi::__cxa_demangle(name, 0, 0, &status);
	std::string result(demangled_name);
	free(demangled_name);
	return result;
#else
	return std::string(name);
#endif
}



static QString get_normal_class_name_qt(const char *name)
{
#ifdef __GNUC__
	int status = 0;
	char *demangled_name = abi::__cxa_demangle(name, 0, 0, &status);
	QString result(demangled_name);
	free(demangled_name);
	return result;
#else
	return QString(name);
#endif
}

GameObject::~GameObject()
{
	QHashIterator<const char*, GameObjectValue*> iter(adapts);
	while (iter.hasNext())
	{
		iter.next();
		delete iter.value();
	}
}

int GameObject::count() const
{
	return adapts.count();
}

std::string GameObject::to_string() const
{
	std::string result;

	QHashIterator<const char*, GameObjectValue*> iter(adapts);
	while (iter.hasNext())
	{
		iter.next();

		result += get_normal_class_name_std(iter.key()) + "\n";
	}

	return result;
}

void GameObject::clear()
{

}


GameObjectValue* GameObject::_adapt(const char *key)
{
	if (!adapts.contains(key))
	{
		QString text = "GameObject::adapt_object " + this->m_id.toString() + " " + get_normal_class_name_qt(key);
		qDebug() << "error " + text;
		//throw Error(text);
		return nullptr;
	}

	return adapts[key];
}

bool GameObject::_hasModel(const char *key)
{
	return adapts.contains(key);
	//return this->getComponents(_arg_1).length > 0;
}



void GameObject::_putData(Model *model, const char *name, GameObjectValue *value)
{
	QHash<const char*, GameObjectValue*> *h = data[model];

	if (h == nullptr)
	{
		h = new QHash<const char*, GameObjectValue*>();
		data.insert(model, h);
	}

	//qDebug() << "GameObject::_putData" << model << get_normal_class_name_qt(name) << this->m_id.toString() << value;

	h->insert(name, value);
}


GameObjectValue* GameObject::_getData(Model *model, const char *name)
{
	QHash<const char*, GameObjectValue*> *h = data[model];

	if (h == nullptr)
	{
		qDebug() << "error GameObject::_getData" << model << get_normal_class_name_qt(name) << this->m_id.toString();

		return nullptr;
	}

	auto result = h->value(name);

	if (result == nullptr)
	{
		qDebug() << "error GameObject::_getData" << model << get_normal_class_name_qt(name) << this->m_id.toString();

	}

	return result;
}


void GameObject::_clearData(Model *model, const char *name)
{
	QHash<const char*, GameObjectValue*> *h = data[model];

	if (h == nullptr)
	{
		return;
	}

	h->remove(name);
}



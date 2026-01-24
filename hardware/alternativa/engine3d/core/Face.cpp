#include "Face.h"
#include "MyMath.h"
#include "Wrapper.h"
#include "Vertex.h"
#include "hardware/alternativa/engine3d/materials/Material.h"
#include "VG.h"
//#include <QHash>
//#include <QDebug>


namespace
{
	class Collector
	{
	public:
		Shared<Face> collector;

		Collector()
		{

		}

		~Collector()
		{
			Shared<Face> temp = collector;

			while (temp != nullptr)
			{
				Shared<Face> next = temp->next;
				delete temp.get();
				temp.reset();
				temp = next;
			}

			collector.reset();
		}


		int count()
		{
			int count = 0;

			Weak<Face> temp = collector;

			while (temp != nullptr)
			{
				count++;
				temp = temp->next;
			}

			return count;
		}

	} c;


	//QHash<int, Collector*> _hash;
}


Face::Face()
{
	this->distance = 0;
	this->smoothingGroups = 0;
}


Shared<Face> Face::create(int id)
{
	Shared<Face> result;
#if 0
	Collector *collector = _hash[id];
	if (collector == nullptr)
	{
		collector = new Collector;
		_hash[id] = collector;
	}
#endif

	Collector *collector = &c;

	if (collector->collector != nullptr)
	{
		result = collector->collector;
		collector->collector = result->next;
		result->next = nullptr;
	}
	else
	{
		//qDebug("Face %d", id);
		result.reset(new Face);
	}

	result->next = nullptr;
	result->material = nullptr;
	result->wrapper = nullptr;
	result->processNext = nullptr;
	result->processNegative = nullptr;
	result->processPositive = nullptr;
	result->distance = 0;
	result->geometry = nullptr;
	result->id = id;

	return result;
}


void Face::remove()
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


	this->material = nullptr;
	this->wrapper = nullptr;
	this->processNext = nullptr;
	this->processNegative = nullptr;
	this->processPositive = nullptr;
	this->geometry = nullptr;

	Collector *collector = &c;
	//Collector *collector = _hash[id];

	temp->next = collector->collector;
	collector->collector.set(this);
}


void Face::calculateBestSequenceAndNormal()
{
	Shared<Wrapper> _loc16_;

	if (this->wrapper->next->next->next != nullptr)
	{
		double _loc15_ = -1.0e22;
		Shared<Wrapper> _loc1_ = this->wrapper;
		while (_loc1_ != nullptr)
		{
			Weak<Wrapper> _loc19_ = _loc1_->next != nullptr ? _loc1_->next : this->wrapper;
			Weak<Wrapper> _loc20_ = _loc19_->next != nullptr ? _loc19_->next : this->wrapper;
			Weak<Vertex> _loc2_ = _loc1_->vertex;
			Weak<Vertex> _loc3_ = _loc19_->vertex;
			Weak<Vertex> _loc4_ = _loc20_->vertex;
			double _loc5_ = _loc3_->x - _loc2_->x;
			double _loc6_ = _loc3_->y - _loc2_->y;
			double _loc7_ = _loc3_->z - _loc2_->z;
			double _loc8_ = _loc4_->x - _loc2_->x;
			double _loc9_ = _loc4_->y - _loc2_->y;
			double _loc10_ = _loc4_->z - _loc2_->z;
			double _loc11_ = _loc10_ * _loc6_ - _loc9_ * _loc7_;
			double _loc12_ = _loc8_ * _loc7_ - _loc10_ * _loc5_;
			double _loc13_ = _loc9_ * _loc5_ - _loc8_ * _loc6_;
			double _loc14_ = _loc11_ * _loc11_ + _loc12_ * _loc12_ + _loc13_ * _loc13_;
			if (_loc14_ > _loc15_)
			{
				_loc15_ = _loc14_;
				_loc16_ = _loc1_;
			}
			_loc1_ = _loc1_->next;
		}
		if (_loc16_ != this->wrapper)
		{
			Weak<Wrapper> _loc17_ = this->wrapper->next->next->next;
			while (_loc17_->next != nullptr)
			{
				_loc17_ = _loc17_->next;
			}
			Weak<Wrapper> _loc18_ = this->wrapper;
			while (_loc18_->next != _loc16_ && _loc18_->next != nullptr)
			{
				_loc18_ = _loc18_->next;
			}
			_loc17_->next = this->wrapper;
			_loc18_->next = nullptr;
			this->wrapper = _loc16_;
		}
	}

	Weak<Wrapper> _loc1_ = this->wrapper;
	Weak<Vertex> _loc2_ = _loc1_->vertex;
	_loc1_ = _loc1_->next;
	Weak<Vertex> _loc3_ = _loc1_->vertex;
	_loc1_ = _loc1_->next;
	Weak<Vertex> _loc4_ = _loc1_->vertex;

	double _loc5_ = _loc3_->x - _loc2_->x;
	double _loc6_ = _loc3_->y - _loc2_->y;
	double _loc7_ = _loc3_->z - _loc2_->z;
	double _loc8_ = _loc4_->x - _loc2_->x;
	double _loc9_ = _loc4_->y - _loc2_->y;
	double _loc10_ = _loc4_->z - _loc2_->z;
	double _loc11_ = _loc10_ * _loc6_ - _loc9_ * _loc7_;
	double _loc12_ = _loc8_ * _loc7_ - _loc10_ * _loc5_;
	double _loc13_ = _loc9_ * _loc5_ - _loc8_ * _loc6_;
	double _loc14_ = _loc11_ * _loc11_ + _loc12_ * _loc12_ + _loc13_ * _loc13_;
	if (_loc14_ > 0)
	{
		_loc14_ = 1. / MyMath::sqrt(_loc14_);
		_loc11_ = _loc11_ * _loc14_;
		_loc12_ = _loc12_ * _loc14_;
		_loc13_ = _loc13_ * _loc14_;
		this->normalX = _loc11_;
		this->normalY = _loc12_;
		this->normalZ = _loc13_;
	}
	this->offset = _loc2_->x * _loc11_ + _loc2_->y * _loc12_ + _loc2_->z * _loc13_;
}


void Face::copy(const Face *src)
{
	this->material = src->material;
	this->wrapper = src->wrapper;
	this->smoothingGroups = src->smoothingGroups;
	this->normalX = src->normalX;
	this->normalY = src->normalY;
	this->normalZ = src->normalZ;
	this->offset = src->offset;
	this->processNext = src->processNext;
	this->distance = src->distance;
	this->geometry = src->geometry;
}

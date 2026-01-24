#include "Object3DContainer.h"
#include "Error.h"
#include "flash/geom/Vector3D.h"
#include "VG.h"
#include <QMap>


std::shared_ptr<Object3DContainer> new_Object3DContainer()
{
	return std::make_shared<Object3DContainer>();
}


Object3DContainer::Object3DContainer()
{
	childrenList = nullptr;
	mouseChildren = true;
	numVisibleChildren = 0;
	name_class = "Object3DContainer";
}


std::shared_ptr<Object3D> Object3DContainer::addChild(std::shared_ptr<Object3D> object3d)
{
	if (object3d == nullptr)
		throw TypeError("Parameter child must be non-null.");

	if (object3d == this->shared_from_this())
		throw ArgumentError("An object cannot be added as a child of itself.");

	Object3DContainer *_loc2_ = this->_parent.get();
	while (_loc2_ != nullptr)
	{
		if (_loc2_ == object3d.get())
			throw ArgumentError("An object cannot be added as a child to one of it\'s children (or children\'s children, etc.).");

		_loc2_ = _loc2_->_parent.get();
	}
	if (object3d->_parent != nullptr)
	{
		object3d->_parent->removeChild(object3d);
	}

	this->addToList(object3d);
	return object3d;
}


void Object3DContainer::addToList(std::shared_ptr<Object3D> param1, std::shared_ptr<Object3D> param2)
{
	param1->next = param2;

	param1->setParent(std::dynamic_pointer_cast<Object3DContainer>(this->shared_from_this()));

	if (param2 == this->childrenList)
	{
		this->childrenList = param1;
	}
	else
	{
		std::shared_ptr<Object3D> _loc3_ = this->childrenList;
		while (_loc3_ != nullptr)
		{
			if (_loc3_->next == param2)
			{
				_loc3_->next = param1;
				break;
			}
			_loc3_ = _loc3_->next;
		}
	}
}


void Object3DContainer::removeChild(std::shared_ptr<Object3D> param1)
{
	if (param1 == nullptr)
	{
		throw TypeError("Parameter child must be non-null.");
	}
	if (param1->_parent != this->shared_from_this())
	{
		throw ArgumentError("The supplied Object3D must be a child of the caller.");
	}

	std::shared_ptr<Object3D> _loc2_ = nullptr;
	std::shared_ptr<Object3D> _loc3_ = this->childrenList;
	while (_loc3_ != nullptr)
	{
		if (_loc3_ == param1)
		{
			if (_loc2_ != nullptr)
			{
				_loc2_->next = _loc3_->next;
			}
			else
			{
				this->childrenList = _loc3_->next;
			}
			_loc3_->next = nullptr;
			_loc3_->setParent(nullptr);
			return;
		}
		_loc2_ = _loc3_;
		_loc3_ = _loc3_->next;
	}
	throw ArgumentError("Cannot remove child.");
}


std::shared_ptr<Object3D> Object3DContainer::clone() const
{
	std::shared_ptr<Object3D> o = new_Object3DContainer();
	o->clonePropertiesFrom(this);
	return o;
}


void Object3DContainer::clonePropertiesFrom(const Object3D *src)
{
	std::shared_ptr<Object3D> _loc4_ = nullptr;
	std::shared_ptr<Object3D> _loc5_ = nullptr;
	Object3D::clonePropertiesFrom(src);
	Object3DContainer *_loc2_ = (Object3DContainer*)src;
	this->mouseChildren = _loc2_->mouseChildren;
	std::shared_ptr<Object3D> _loc3_ = _loc2_->childrenList;
	while (_loc3_ != nullptr)
	{
		_loc5_ = _loc3_->clone();
		if (this->childrenList != nullptr)
		{
			_loc4_->next = _loc5_;
		}
		else
		{
			this->childrenList = _loc5_;
		}
		_loc4_ = _loc5_;
		_loc5_->setParent(std::dynamic_pointer_cast<Object3DContainer>(this->shared_from_this()));
		_loc3_ = _loc3_->next;
	}
}


void Object3DContainer::collectPlanes(const Vector3D *param1, const Vector3D *param2, const Vector3D *param3,
							 const Vector3D *param4, const Vector3D *param5, Faces *param6, void *param7)
{
	//QVector<Face*> *param6, Dictionary param7 = nullptr
	QMap<Object3D*, void*> *_param7 = (QMap<Object3D*, void*>*)param7;
	if (_param7 != nullptr && _param7->contains(this))
	{
		return;
	}

	Vector3D v;
	calculateSphere(param1, param2, param3, param4, param5, &v);
	if (!boundIntersectSphere(&v, this->bound.boundMinX, this->bound.boundMinY, this->bound.boundMinZ,
							 this->bound.boundMaxX, this->bound.boundMaxY, this->bound.boundMaxZ))
	{
		return;
	}

	std::shared_ptr<Object3D> o = this->childrenList;
	while (o != nullptr)
	{
		o->composeAndAppend(this);
		o->collectPlanes(param1, param2, param3, param4, param5, param6, param7);
		o = o->next;
	}
}


std::shared_ptr<Object3D> Object3DContainer::addChildAt(std::shared_ptr<Object3D> _arg_1, int _arg_2)
{
	if (_arg_1 == nullptr)
	{
		throw TypeError("Parameter child must be non-null.");
	}
	if (_arg_1.get() == this)
	{
		throw ArgumentError("An object cannot be added as a child of itself.");
	}
	if (_arg_2 < 0)
	{
		throw RangeError("The supplied index is out of bounds.");
	}

	std::shared_ptr<Object3DContainer> _local_3 = _parent;
	while (_local_3 != nullptr)
	{
		if (_local_3 == _arg_1)
		{
			throw ArgumentError("An object cannot be added as a child to one of it's children (or children's children, etc.).");
		}
		_local_3 = _local_3->_parent;
	}

	std::shared_ptr<Object3D> _local_4 = this->childrenList;

	int _local_5 = 0;
	while (_local_5 < _arg_2)
	{
		if (_local_4 == nullptr)
		{
			throw RangeError("The supplied index is out of bounds.");
		}
		_local_4 = _local_4->next;
		_local_5++;
	}

	if (_arg_1->_parent != nullptr)
	{
		_arg_1->_parent->removeChild(_arg_1);
	}

	this->addToList(_arg_1, _local_4);

	return _arg_1;
}


void Object3DContainer::updateBounds(Object3D *_arg_1, Object3D *_arg_2)
{
	Object3D *_local_3 = this->childrenList.get();
	while (_local_3 != nullptr)
	{
		if (_arg_2 != nullptr)
		{
			_local_3->composeAndAppend(_arg_2);
		}
		else
		{
			_local_3->composeMatrix();
		}
		_local_3->updateBounds(_arg_1, _local_3);
		_local_3 = _local_3->next.get();
	}
}


void Object3DContainer::draw(Camera3D *camera)
{
	this->numVisibleChildren = 0;
	Object3D *object = this->childrenList.get();
	while (object != nullptr)
	{
		if (object->visible)
		{
			object->composeAndAppend(this);
			if (object->cullingInCamera(camera, culling) >= 0)
			{
				object->concat(this);

				if (visibleChildren.length() <= this->numVisibleChildren)
					visibleChildren.resize(this->numVisibleChildren + 10);

				this->visibleChildren[this->numVisibleChildren] = object;
				this->numVisibleChildren++;
			}
		}
		object = object->next.get();
	}

	if (this->numVisibleChildren > 0)
	{
		//if (((_arg_1.debug) && ((_local_2 = _arg_1.checkInDebug(this)) > 0)))
		//{
		//	if ((_local_2 & Debug.BOUNDS))
		//	{
		//		Debug.drawBounds(_arg_1, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
		//	}
		//}
		this->drawVisibleChildren(camera);
	}
}


void Object3DContainer::drawVisibleChildren(Camera3D *camera)
{
	int i = this->numVisibleChildren - 1;

	while (i >= 0)
	{
		Object3D *object = this->visibleChildren[i];
		object->draw(camera);
		this->visibleChildren[i] = nullptr;
		i--;
	}
}


Shared<VG> Object3DContainer::getVG(Camera3D *camera)
{
	Shared<VG> _local_2 = nullptr;
	Shared<VG> _local_3 = nullptr;

	Object3D *_local_4 = this->childrenList.get();
	while (_local_4 != nullptr)
	{
		if (_local_4->visible)
		{
			_local_4->composeAndAppend(this);
			if (_local_4->cullingInCamera(camera, culling) >= 0)
			{
				_local_4->concat(this);
				Shared<VG> _local_5 = _local_4->getVG(camera);
				if (_local_5 != nullptr)
				{
					if (_local_2 != nullptr)
					{
						_local_3->next = _local_5;
					}
					else
					{
						_local_2 = _local_5;
						_local_3 = _local_5;
					}
					while (_local_3->next != nullptr)
					{
						_local_3 = _local_3->next;
					}
				}
			}
		}
		_local_4 = _local_4->next.get();
	}

	return _local_2;
}

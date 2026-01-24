#include "ConflictContainer.h"
#include "../core/VG.h"
#include "../core/Vertex.h"
#include "../core/Wrapper.h"
#include "../core/Face.h"
#include "../core/Camera3D.h"
#include "../materials/Material.h"
#include "MyMath.h"


ConflictContainer::ConflictContainer()
{
	threshold = 0.01;
	resolveByAABB = true;
	resolveByOOBB = true;
	name_class = "ConflictContainer";
}


void ConflictContainer::draw(Camera3D *camera)
{
	qDebug("ConflictContainer::draw");

	Shared<VG> _local_3 = getVG(camera);

	if (_local_3 != nullptr)
	{
		//if (((camera.debug) && ((_local_2 = camera.checkInDebug(this)) > 0)))
		//{
		//	if ((_local_2 & Debug.BOUNDS))
		//	{
		//		Debug.drawBounds(camera, this, boundMinX, boundMinY, boundMinZ, boundMaxX, boundMaxY, boundMaxZ);
		//	}
		//}

		if (_local_3->next != nullptr)
		{
			calculateInverseMatrix();
			if (this->resolveByAABB)
			{
				Weak<VG> _local_4 = _local_3;
				while (_local_4 != nullptr)
				{
					_local_4->calculateAABB(this->im.ma,
											this->im.mb,
											this->im.mc,
											this->im.md,
											this->im.me,
											this->im.mf,
											this->im.mg,
											this->im.mh,
											this->im.mi,
											this->im.mj,
											this->im.mk,
											this->im.ml);

					_local_4 = _local_4->next;
				}
				this->drawAABBGeometry(camera, _local_3);
			}
			else
			{
				if (this->resolveByOOBB)
				{
					Weak<VG> _local_4 = _local_3;
					while (_local_4 != nullptr)
					{
						_local_4->calculateOOBB(this);
						_local_4 = _local_4->next;
					}
					this->drawOOBBGeometry(camera, _local_3);
				}
				else
				{
					this->drawConflictGeometry(camera, _local_3);
				}
			}
		}
		else
		{
			_local_3->draw(camera, this->threshold, this);
			_local_3->destroy();
		}
	}
}


std::shared_ptr<Object3D> ConflictContainer::clone() const
{
	qDebug("used ConflictContainer::clone");
	return nullptr;
}


void ConflictContainer::drawAABBGeometry(Camera3D *_arg_1,
										 Shared<VG> _arg_2,
										 bool _arg_3,
										 bool _arg_4,
										 bool _arg_5,
										 int _arg_6)
{
	bool _local_7 = false;
	bool _local_8 = false;
	bool  _local_9 = false;

	Shared<VG> _local_10 = ((_arg_5) ? this->sortGeometry(_arg_2, _arg_3, _arg_4) : _arg_2);
	Shared<VG> _local_11 = _local_10;
	Shared<VG> _local_12 = _local_10->next;
	double _local_13 = _local_10->boundMax;

	while (_local_12 != nullptr)
	{
		bool _local_14 = (_local_12->boundMin >= (_local_13 - this->threshold));
		if (((_local_14) || (_local_12->next == nullptr)))
		{
			if (_local_14)
			{
				_local_11->next = nullptr;
				_arg_6 = 0;
			}
			else
			{
				_local_12 = nullptr;
				_arg_6++;
			}
			if (_arg_3)
			{
				_local_7 = (this->im.md < _local_13);
				_local_8 = false;
				_local_9 = true;
			}
			else
			{
				if (_arg_4)
				{
					_local_7 = (this->im.mh < _local_13);
					_local_8 = false;
					_local_9 = false;
				}
				else
				{
					_local_7 = (this->im.ml < _local_13);
					_local_8 = true;
					_local_9 = false;
				}
			}
			if (_local_7)
			{
				if (_local_10->next != nullptr)
				{
					if (_arg_6 < 2)
					{
						this->drawAABBGeometry(_arg_1, _local_10, _local_8, _local_9, true, _arg_6);
					}
					else
					{
						if (this->resolveByOOBB)
						{
							_local_11 = _local_10;
							while (_local_11 != nullptr)
							{
								_local_11->calculateOOBB(this);
								_local_11 = _local_11->next;
							}
							this->drawOOBBGeometry(_arg_1, _local_10);
						}
						else
						{
							this->drawConflictGeometry(_arg_1, _local_10);
						}
					}
				}
				else
				{
					_local_10->draw(_arg_1, this->threshold, this);
					_local_10->destroy();
				}
				if (_local_12 != nullptr)
				{
					if (_local_12->next != nullptr)
					{
						this->drawAABBGeometry(_arg_1, _local_12, _arg_3, _arg_4, false, -1);
					}
					else
					{
						_local_12->draw(_arg_1, this->threshold, this);
						_local_12->destroy();
					}
				}
			}
			else
			{
				if (_local_12 != nullptr)
				{
					if (_local_12->next != nullptr)
					{
						this->drawAABBGeometry(_arg_1, _local_12, _arg_3, _arg_4, false, -1);
					}
					else
					{
						_local_12->draw(_arg_1, this->threshold, this);
						_local_12->destroy();
					}
				}
				if (_local_10->next != nullptr)
				{
					if (_arg_6 < 2)
					{
						this->drawAABBGeometry(_arg_1, _local_10, _local_8, _local_9, true, _arg_6);
					}
					else
					{
						if (this->resolveByOOBB)
						{
							_local_11 = _local_10;
							while (_local_11 != nullptr)
							{
								_local_11->calculateOOBB(this);
								_local_11 = _local_11->next;
							}
							this->drawOOBBGeometry(_arg_1, _local_10);
						}
						else
						{
							this->drawConflictGeometry(_arg_1, _local_10);
						}
					}
				}
				else
				{
					_local_10->draw(_arg_1, this->threshold, this);
					_local_10->destroy();
				}
			}
			return;
		}
		if (_local_12->boundMax > _local_13)
		{
			_local_13 = _local_12->boundMax;
		}
		_local_11 = _local_12;
		_local_12 = _local_12->next;
	}
}


void ConflictContainer::drawOOBBGeometry(Camera3D *_arg_1, Shared<VG> _arg_2)
{
	Shared<Vertex> _local_3;
	Shared<Vertex> _local_4;
	Shared<Wrapper> _local_5;
	double _local_6;
	double _local_7 = 0;
	double _local_8 = 0;
	double _local_9 = 0;
	double _local_10;
	bool _local_11;
	bool _local_12;
	Shared<VG> _local_14;
	bool _local_15;
	Shared<VG> _local_16;
	Shared<VG> _local_17;
	Shared<VG> _local_18;
	Shared<VG> _local_19;

	Shared<VG> _local_13 = _arg_2;

	while (_local_13 != nullptr)
	{
		if (_local_13->viewAligned)
		{
			_local_10 = _local_13->object->matrix4.ml;
			_local_14 = _arg_2;
			while (_local_14 != nullptr)
			{
				if ((!(_local_14->viewAligned)))
				{
					_local_11 = false;
					_local_12 = false;
					_local_3 = _local_14->boundVertexList;
					while (_local_3 != nullptr)
					{
						if (_local_3->cameraZ > _local_10)
						{
							if (_local_11) break;
							_local_12 = true;
						}
						else
						{
							if (_local_12) break;
							_local_11 = true;
						}
						_local_3 = _local_3->next;
					}
					if (_local_3 != nullptr) break;
				}
				_local_14 = _local_14->next;
			}
			if (_local_14 == nullptr) break;
		}
		else
		{
			_local_4 = _local_13->boundPlaneList;
			while (_local_4 != nullptr)
			{
				_local_7 = _local_4->cameraX;
				_local_8 = _local_4->cameraY;
				_local_9 = _local_4->cameraZ;
				_local_10 = _local_4->offset;
				_local_15 = false;
				_local_14 = _arg_2;
				while (_local_14 != nullptr)
				{
					if (_local_13 != _local_14)
					{
						_local_11 = false;
						_local_12 = false;
						if (_local_14->viewAligned)
						{
							_local_5 = _local_14->faceStruct->wrapper;
							while (_local_5 != nullptr)
							{
								_local_3 = _local_5->vertex;
								if ((((_local_3->cameraX * _local_7) + (_local_3->cameraY * _local_8)) + (_local_3->cameraZ * _local_9)) >= (_local_10 - this->threshold))
								{
									if (_local_11) break;
									_local_15 = true;
									_local_12 = true;
								}
								else
								{
									if (_local_12) break;
									_local_11 = true;
								}
								_local_5 = _local_5->next;
							}
							if (_local_5 != nullptr) break;
						}
						else
						{
							_local_3 = _local_14->boundVertexList;
							while (_local_3 != nullptr)
							{
								if ((((_local_3->cameraX * _local_7) + (_local_3->cameraY * _local_8)) + (_local_3->cameraZ * _local_9)) >= (_local_10 - this->threshold))
								{
									if (_local_11) break;
									_local_15 = true;
									_local_12 = true;
								}
								else
								{
									if (_local_12) break;
									_local_11 = true;
								}
								_local_3 = _local_3->next;
							}
							if (_local_3 != nullptr) break;
						}
					}
					_local_14 = _local_14->next;
				}
				if (((_local_14 == nullptr) && (_local_15))) break;
				_local_4 = _local_4->next;
			}
			if (_local_4 != nullptr) break;
		}
		_local_13 = _local_13->next;
	}
	if (_local_13 != nullptr)
	{
		if (_local_13->viewAligned)
		{
			while (_arg_2 != nullptr)
			{
				_local_16 = _arg_2->next;
				if (_arg_2->viewAligned)
				{
					_local_6 = (_arg_2->object->matrix4.ml - _local_10);
					if (_local_6 < -(this->threshold))
					{
						_arg_2->next = _local_19;
						_local_19 = _arg_2;
					}
					else
					{
						if (_local_6 > this->threshold)
						{
							_arg_2->next = _local_17;
							_local_17 = _arg_2;
						}
						else
						{
							_arg_2->next = _local_18;
							_local_18 = _arg_2;
						}
					}
				}
				else
				{
					_local_3 = _arg_2->boundVertexList;
					while (_local_3 != nullptr)
					{
						_local_6 = (_local_3->cameraZ - _local_10);
						if (_local_6 < -(this->threshold))
						{
							_arg_2->next = _local_19;
							_local_19 = _arg_2;
							break;
						}
						if (_local_6 > this->threshold)
						{
							_arg_2->next = _local_17;
							_local_17 = _arg_2;
							break;
						}
						_local_3 = _local_3->next;
					}
					if (_local_3 == nullptr)
					{
						_arg_2->next = _local_18;
						_local_18 = _arg_2;
					}
				}
				_arg_2 = _local_16;
			}
		}
		else
		{
			while (_arg_2 != nullptr)
			{
				_local_16 = _arg_2->next;
				if (_arg_2->viewAligned)
				{
					_local_5 = _arg_2->faceStruct->wrapper;
					while (_local_5 != nullptr)
					{
						_local_3 = _local_5->vertex;
						_local_6 = ((((_local_3->cameraX * _local_7) + (_local_3->cameraY * _local_8)) + (_local_3->cameraZ * _local_9)) - _local_10);
						if (_local_6 < -(this->threshold))
						{
							_arg_2->next = _local_17;
							_local_17 = _arg_2;
							break;
						}
						if (_local_6 > this->threshold)
						{
							_arg_2->next = _local_19;
							_local_19 = _arg_2;
							break;
						}
						_local_5 = _local_5->next;
					}
					if (_local_5 == nullptr)
					{
						_arg_2->next = _local_18;
						_local_18 = _arg_2;
					}
				}
				else
				{
					_local_3 = _arg_2->boundVertexList;
					while (_local_3 != nullptr)
					{
						_local_6 = ((((_local_3->cameraX * _local_7) + (_local_3->cameraY * _local_8)) + (_local_3->cameraZ * _local_9)) - _local_10);
						if (_local_6 < -(this->threshold))
						{
							_arg_2->next = _local_17;
							_local_17 = _arg_2;
							break;
						}
						if (_local_6 > this->threshold)
						{
							_arg_2->next = _local_19;
							_local_19 = _arg_2;
							break;
						}
						_local_3 = _local_3->next;
					}
					if (_local_3 == nullptr)
					{
						_arg_2->next = _local_18;
						_local_18 = _arg_2;
					}
				}
				_arg_2 = _local_16;
			}
		}
		if (((_local_13->viewAligned) || (_local_10 < 0)))
		{
			if (_local_19 != nullptr)
			{
				if (_local_19->next != nullptr)
				{
					this->drawOOBBGeometry(_arg_1, _local_19);
				}
				else
				{
					_local_19->draw(_arg_1, this->threshold, this);
					_local_19->destroy();
				}
			}
			while (_local_18 != nullptr)
			{
				_local_16 = _local_18->next;
				_local_18->draw(_arg_1, this->threshold, this);
				_local_18->destroy();
				_local_18 = _local_16;
			}
			if (_local_17 != nullptr)
			{
				if (_local_17->next != nullptr)
				{
					this->drawOOBBGeometry(_arg_1, _local_17);
				}
				else
				{
					_local_17->draw(_arg_1, this->threshold, this);
					_local_17->destroy();
				}
			}
		}
		else
		{
			if (_local_17 != nullptr)
			{
				if (_local_17->next != nullptr)
				{
					this->drawOOBBGeometry(_arg_1, _local_17);
				}
				else
				{
					_local_17->draw(_arg_1, this->threshold, this);
					_local_17->destroy();
				}
			}
			while (_local_18 != nullptr)
			{
				_local_16 = _local_18->next;
				_local_18->draw(_arg_1, this->threshold, this);
				_local_18->destroy();
				_local_18 = _local_16;
			}
			if (_local_19 != nullptr)
			{
				if (_local_19->next != nullptr)
				{
					this->drawOOBBGeometry(_arg_1, _local_19);
				}
				else
				{
					_local_19->draw(_arg_1, this->threshold, this);
					_local_19->destroy();
				}
			}
		}
	}
	else
	{
		this->drawConflictGeometry(_arg_1, _arg_2);
	}
}


static Shared<Face> collectNode(Shared<Face> _arg_1, Shared<Face> _arg_2, Camera3D *_arg_3, double _arg_4, bool _arg_5, Shared<Face> _arg_6 = nullptr)
{
	Shared<Vertex> _local_10;
	Shared<Vertex> _local_11;
	double _local_12;
	double _local_13;
	double _local_14;
	double _local_15;
	Shared<Face> _local_16 = nullptr;
	Shared<Face> _local_17 = nullptr;
	Shared<Face> _local_18 = nullptr;
	Shared<VG> _local_19 = nullptr;
	Shared<Face> _local_22 = nullptr;
	Shared<Face> _local_23 = nullptr;
	Shared<Face> _local_24 = nullptr;
	Shared<Face> _local_25 = nullptr;
	Shared<Face> _local_26;
	double _local_28;
	double _local_29;
	double _local_30;
	double _local_31;
	double _local_32;
	double _local_33;
	double _local_34;
	double _local_35;
	double _local_36;
	double _local_37;
	double _local_38;
	double _local_39;
	double _local_40;
	double _local_41;
	double _local_42;
	double _local_43;
	double _local_44;
	bool _local_45;
	bool _local_46;
	double _local_47;
	Shared<Face> _local_48;
	Shared<Face> _local_49;
	Shared<Wrapper> _local_50;
	Shared<Wrapper> _local_51;
	Shared<Wrapper> _local_52;
	bool _local_53;
	double _local_54;


	if (_arg_1 != nullptr)
	{
		_local_19 = _arg_1->geometry;
		if (_arg_1->offset < 0)
		{
			_local_17 = _arg_1->processNegative;
			_local_18 = _arg_1->processPositive;
			_local_12 = _arg_1->normalX;
			_local_13 = _arg_1->normalY;
			_local_14 = _arg_1->normalZ;
			_local_15 = _arg_1->offset;
		}
		else
		{
			_local_17 = _arg_1->processPositive;
			_local_18 = _arg_1->processNegative;
			_local_12 = -(_arg_1->normalX);
			_local_13 = -(_arg_1->normalY);
			_local_14 = -(_arg_1->normalZ);
			_local_15 = -(_arg_1->offset);
		}
		_arg_1->processNegative = nullptr;
		_arg_1->processPositive = nullptr;
		if (_arg_1->wrapper != nullptr)
		{
			_local_16 = _arg_1;
			while (_local_16->processNext != nullptr)
			{
				_local_16 = _local_16->processNext;
				_local_16->geometry = _local_19;
			}
		}
		else
		{
			_arg_1->geometry = nullptr;
			_arg_1 = nullptr;
		}
	}
	else
	{
		_arg_1 = _arg_2;
		_arg_2 = _arg_1->processNext;
		_local_16 = _arg_1;
		Weak<Wrapper> _local_7 = _arg_1->wrapper;
		Vertex *_local_8 = _local_7->vertex.get();
		_local_7 = _local_7->next;
		Vertex *_local_9 = _local_7->vertex.get();
		_local_28 = _local_8->cameraX;
		_local_29 = _local_8->cameraY;
		_local_30 = _local_8->cameraZ;
		_local_31 = (_local_9->cameraX - _local_28);
		_local_32 = (_local_9->cameraY - _local_29);
		_local_33 = (_local_9->cameraZ - _local_30);
		_local_12 = 0;
		_local_13 = 0;
		_local_14 = 1;
		_local_15 = _local_30;
		_local_34 = 0;
		_local_7 = _local_7->next;
		while (_local_7 != nullptr)
		{
			_local_11 = _local_7->vertex;
			_local_35 = (_local_11->cameraX - _local_28);
			_local_36 = (_local_11->cameraY - _local_29);
			_local_37 = (_local_11->cameraZ - _local_30);
			_local_38 = ((_local_37 * _local_32) - (_local_36 * _local_33));
			_local_39 = ((_local_35 * _local_33) - (_local_37 * _local_31));
			_local_40 = ((_local_36 * _local_31) - (_local_35 * _local_32));
			_local_41 = (((_local_38 * _local_38) + (_local_39 * _local_39)) + (_local_40 * _local_40));
			if (_local_41 > _arg_4)
			{
				_local_41 = (1 / MyMath::sqrt(_local_41));
				_local_12 = (_local_38 * _local_41);
				_local_13 = (_local_39 * _local_41);
				_local_14 = (_local_40 * _local_41);
				_local_15 = (((_local_28 * _local_12) + (_local_29 * _local_13)) + (_local_30 * _local_14));
				break;
			}
			if (_local_41 > _local_34)
			{
				_local_41 = (1 / MyMath::sqrt(_local_41));
				_local_12 = (_local_38 * _local_41);
				_local_13 = (_local_39 * _local_41);
				_local_14 = (_local_40 * _local_41);
				_local_15 = (((_local_28 * _local_12) + (_local_29 * _local_13)) + (_local_30 * _local_14));
				_local_34 = _local_41;
			}
			_local_7 = _local_7->next;
		}
	}

	double _local_20 = (_local_15 - _arg_4);
	double _local_21 = (_local_15 + _arg_4);
	Shared<Face> _local_27 = _arg_2;
	while (_local_27 != nullptr)
	{
		_local_26 = _local_27->processNext;
		Shared<Wrapper> _local_7 = _local_27->wrapper;
		Vertex *_local_8 = _local_7->vertex.get();
		_local_7 = _local_7->next;
		Shared<Vertex> _local_9 = _local_7->vertex;
		_local_7 = _local_7->next;
		_local_10 = _local_7->vertex;
		_local_7 = _local_7->next;
		_local_42 = (((_local_8->cameraX * _local_12) + (_local_8->cameraY * _local_13)) + (_local_8->cameraZ * _local_14));
		_local_43 = (((_local_9->cameraX * _local_12) + (_local_9->cameraY * _local_13)) + (_local_9->cameraZ * _local_14));
		_local_44 = (((_local_10->cameraX * _local_12) + (_local_10->cameraY * _local_13)) + (_local_10->cameraZ * _local_14));
		_local_45 = (((_local_42 < _local_20) || (_local_43 < _local_20)) || (_local_44 < _local_20));
		_local_46 = (((_local_42 > _local_21) || (_local_43 > _local_21)) || (_local_44 > _local_21));
		while (_local_7 != nullptr)
		{
			_local_11 = _local_7->vertex;
			_local_47 = (((_local_11->cameraX * _local_12) + (_local_11->cameraY * _local_13)) + (_local_11->cameraZ * _local_14));
			if (_local_47 < _local_20)
			{
				_local_45 = true;
			}
			else
			{
				if (_local_47 > _local_21)
				{
					_local_46 = true;
				}
			}
			_local_11->offset = _local_47;
			_local_7 = _local_7->next;
		}
		if ((!(_local_45)))
		{
			if ((!(_local_46)))
			{
				if (_arg_1 != nullptr)
				{
					_local_16->processNext = _local_27;
				}
				else
				{
					_arg_1 = _local_27;
				}
				_local_16 = _local_27;
			}
			else
			{
				if (_local_24 != nullptr)
				{
					_local_25->processNext = _local_27;
				}
				else
				{
					_local_24 = _local_27;
				}
				_local_25 = _local_27;
			}
		}
		else
		{
			if ((!(_local_46)))
			{
				if (_local_22 != nullptr)
				{
					_local_23->processNext = _local_27;
				}
				else
				{
					_local_22 = _local_27;
				}
				_local_23 = _local_27;
			}
			else
			{
				_local_8->offset = _local_42;
				_local_9->offset = _local_43;
				_local_10->offset = _local_44;
				_local_48 = _local_27->create(1);
				_local_48->material = _local_27->material;
				_local_48->geometry = _local_27->geometry;
				_arg_3->lastFace->next = _local_48;
				_arg_3->lastFace = _local_48;
				_local_49 = _local_27->create(2);
				_local_49->material = _local_27->material;
				_local_49->geometry = _local_27->geometry;
				_arg_3->lastFace->next = _local_49;
				_arg_3->lastFace = _local_49;
				_local_50 = nullptr;
				_local_51 = nullptr;
				Shared<Wrapper> _local_7 = _local_27->wrapper->next->next;
				while (_local_7->next != nullptr)
				{
					_local_7 = _local_7->next;
				}
				Shared<Vertex> _local_8 = _local_7->vertex;
				_local_42 = _local_8->offset;
				_local_53 = ((!(_local_27->material == nullptr)) && (_local_27->material->useVerticesNormals));
				_local_7 = _local_27->wrapper;
				while (_local_7 != nullptr)
				{
					_local_9 = _local_7->vertex;
					_local_43 = _local_9->offset;
					if ((((_local_42 < _local_20) && (_local_43 > _local_21)) || ((_local_42 > _local_21) && (_local_43 < _local_20))))
					{
						_local_54 = ((_local_15 - _local_42) / (_local_43 - _local_42));
						_local_11 = _local_9->create();
						_arg_3->lastVertex->next = _local_11;
						_arg_3->lastVertex = _local_11;
						_local_11->cameraX = (_local_8->cameraX + ((_local_9->cameraX - _local_8->cameraX) * _local_54));
						_local_11->cameraY = (_local_8->cameraY + ((_local_9->cameraY - _local_8->cameraY) * _local_54));
						_local_11->cameraZ = (_local_8->cameraZ + ((_local_9->cameraZ - _local_8->cameraZ) * _local_54));
						_local_11->u = (_local_8->u + ((_local_9->u - _local_8->u) * _local_54));
						_local_11->v = (_local_8->v + ((_local_9->v - _local_8->v) * _local_54));
						if (_local_53)
						{
							_local_11->x = (_local_8->x + ((_local_9->x - _local_8->x) * _local_54));
							_local_11->y = (_local_8->y + ((_local_9->y - _local_8->y) * _local_54));
							_local_11->z = (_local_8->z + ((_local_9->z - _local_8->z) * _local_54));
							_local_11->normalX = (_local_8->normalX + ((_local_9->normalX - _local_8->normalX) * _local_54));
							_local_11->normalY = (_local_8->normalY + ((_local_9->normalY - _local_8->normalY) * _local_54));
							_local_11->normalZ = (_local_8->normalZ + ((_local_9->normalZ - _local_8->normalZ) * _local_54));
						}
						_local_52 = _local_7->create();
						_local_52->vertex = _local_11;
						if (_local_50 != nullptr)
						{
							_local_50->next = _local_52;
						}
						else
						{
							_local_48->wrapper = _local_52;
						}
						_local_50 = _local_52;
						_local_52 = _local_7->create();
						_local_52->vertex = _local_11;
						if (_local_51 != nullptr)
						{
							_local_51->next = _local_52;
						}
						else
						{
							_local_49->wrapper = _local_52;
						}
						_local_51 = _local_52;
					}
					if (_local_43 <= _local_21)
					{
						_local_52 = _local_7->create();
						_local_52->vertex = _local_9;
						if (_local_50 != nullptr)
						{
							_local_50->next = _local_52;
						}
						else
						{
							_local_48->wrapper = _local_52;
						}
						_local_50 = _local_52;
					}
					if (_local_43 >= _local_20)
					{
						_local_52 = _local_7->create();
						_local_52->vertex = _local_9;
						if (_local_51 != nullptr)
						{
							_local_51->next = _local_52;
						}
						else
						{
							_local_49->wrapper = _local_52;
						}
						_local_51 = _local_52;
					}
					_local_8 = _local_9;
					_local_42 = _local_43;
					_local_7 = _local_7->next;
				}
				if (_local_22 != nullptr)
				{
					_local_23->processNext = _local_48;
				}
				else
				{
					_local_22 = _local_48;
				}
				_local_23 = _local_48;
				if (_local_24 != nullptr)
				{
					_local_25->processNext = _local_49;
				}
				else
				{
					_local_24 = _local_49;
				}
				_local_25 = _local_49;
				_local_27->processNext = nullptr;
				_local_27->geometry = nullptr;
			}
		}
		_local_27 = _local_26;
	}
	if (_local_18 != nullptr)
	{
		_local_18->geometry = _local_19;
		if (_local_25 != nullptr)
		{
			_local_25->processNext = nullptr;
		}
		_arg_6 = collectNode(_local_18, _local_24, _arg_3, _arg_4, _arg_5, _arg_6);
	}
	else
	{
		if (_local_24 != nullptr)
		{
			if (((_arg_5) && (!(_local_24 == _local_25))))
			{
				if (_local_25 != nullptr)
				{
					_local_25->processNext = nullptr;
				}
				if (_local_24->geometry->sorting == 2)
				{
					_arg_6 = collectNode(nullptr, _local_24, _arg_3, _arg_4, _arg_5, _arg_6);
				}
				else
				{
					_local_24 = _arg_3->sortByAverageZ(_local_24);
					_local_25 = _local_24->processNext;
					while (_local_25->processNext != nullptr)
					{
						_local_25 = _local_25->processNext;
					}
					_local_25->processNext = _arg_6;
					_arg_6 = _local_24;
				}
			}
			else
			{
				_local_25->processNext = _arg_6;
				_arg_6 = _local_24;
			}
		}
	}

	if (_arg_1 != nullptr)
	{
		_local_16->processNext = _arg_6;
		_arg_6 = _arg_1;
	}

	if (_local_17 != nullptr)
	{
		_local_17->geometry = _local_19;
		if (_local_23 != nullptr)
		{
			_local_23->processNext = nullptr;
		}
		_arg_6 = collectNode(_local_17, _local_22, _arg_3, _arg_4, _arg_5, _arg_6);
	}
	else
	{
		if (_local_22 != nullptr)
		{
			if (((_arg_5) && (!(_local_22 == _local_23))))
			{
				if (_local_23 != nullptr)
				{
					_local_23->processNext = nullptr;
				}
				if (_local_22->geometry->sorting == 2)
				{
					_arg_6 = collectNode(nullptr, _local_22, _arg_3, _arg_4, _arg_5, _arg_6);
				}
				else
				{
					_local_22 = _arg_3->sortByAverageZ(_local_22);
					_local_23 = _local_22->processNext;
					while (_local_23->processNext != nullptr)
					{
						_local_23 = _local_23->processNext;
					}
					_local_23->processNext = _arg_6;
					_arg_6 = _local_22;
				}
			}
			else
			{
				_local_23->processNext = _arg_6;
				_arg_6 = _local_22;
			}
		}
	}
	return _arg_6;
}


void ConflictContainer::drawConflictGeometry(Camera3D *_arg_1, Shared<VG> _arg_2)
{
	Shared<VG> _local_6 = nullptr;
	Shared<VG> _local_7 = nullptr;
	Shared<Face> _local_8 = nullptr;
	Shared<Face> _local_9 = nullptr;
	Shared<Face> _local_10 = nullptr;
	Shared<Face> _local_11 = nullptr;
	Shared<Face> _local_12 = nullptr;
	Shared<Face> _local_13 = nullptr;
	Shared<Face> _local_14 = nullptr;
	Shared<Face> _local_15 = nullptr;


	while (_arg_2 != nullptr)
	{
		Shared<VG> _local_5 = _arg_2->next;
		if (_arg_2->space == 1)
		{
			_arg_2->transformStruct(_arg_2->faceStruct,
									++_arg_2->object->transformId,
									this->matrix4.ma,
									this->matrix4.mb,
									this->matrix4.mc,
									this->matrix4.md,
									this->matrix4.me,
									this->matrix4.mf,
									this->matrix4.mg,
									this->matrix4.mh,
									this->matrix4.mi,
									this->matrix4.mj,
									this->matrix4.mk,
									this->matrix4.ml);
		}
		if (_arg_2->sorting == 3)
		{
			_arg_2->next = _local_6;
			_local_6 = _arg_2;
		}
		else
		{
			if (_arg_2->sorting == 2)
			{
				if (_local_8 != nullptr)
				{
					_local_9->processNext = _arg_2->faceStruct;
				}
				else
				{
					_local_8 = _arg_2->faceStruct;
				}
				_local_9 = _arg_2->faceStruct;
				_local_9->geometry = _arg_2;
				while (_local_9->processNext != nullptr)
				{
					_local_9 = _local_9->processNext;
					_local_9->geometry = _arg_2;
				}
			}
			else
			{
				if (_local_10 != nullptr)
				{
					_local_11->processNext = _arg_2->faceStruct;
				}
				else
				{
					_local_10 = _arg_2->faceStruct;
				}
				_local_11 = _arg_2->faceStruct;
				_local_11->geometry = _arg_2;
				while (_local_11->processNext != nullptr)
				{
					_local_11 = _local_11->processNext;
					_local_11->geometry = _arg_2;
				}
			}
			_arg_2->faceStruct = nullptr;
			_arg_2->next = _local_7;
			_local_7 = _arg_2;
		}
		_arg_2 = _local_5;
	}
	if (_local_7 != nullptr)
	{
		_arg_2 = _local_7;
		while (_arg_2->next != nullptr)
		{
			_arg_2 = _arg_2->next;
		}
		_arg_2->next = _local_6;
	}
	else
	{
		_local_7 = _local_6;
	}
	if (_local_8 != nullptr)
	{
		_local_12 = _local_8;
		_local_9->processNext = _local_10;
	}
	else
	{
		_local_12 = _local_10;
	}
	if (_local_6 != nullptr)
	{
		_local_6->faceStruct->geometry = _local_6;
		_local_12 = collectNode(_local_6->faceStruct, _local_12, _arg_1, this->threshold, true);
		_local_6->faceStruct = nullptr;
		_local_6 = _local_6->next;
		while (_local_6 != nullptr)
		{
			_local_6->faceStruct->geometry = _local_6;
			_local_12 = collectNode(_local_6->faceStruct, _local_12, _arg_1, this->threshold, false);
			_local_6->faceStruct = nullptr;
			_local_6 = _local_6->next;
		}
	}
	else
	{
		if (_local_8 != nullptr)
		{
			_local_12 = collectNode(nullptr, _local_12, _arg_1, this->threshold, true);
		}
		else
		{
			if (_local_10 != nullptr)
			{
				_local_12 = _arg_1->sortByAverageZ(_local_12);
			}
		}
	}

	Shared<Face> _local_3 = _local_12;
	while (_local_3 != nullptr)
	{
		Shared<Face> _local_4 = _local_3->processNext;
		_arg_2 = _local_3->geometry;
		_local_3->geometry = nullptr;
		bool _local_17 = ((_local_4 == nullptr) || (!(_arg_2 == _local_4->geometry)));
		if (((_local_17) || (!(_local_3->material == _local_4->material))))
		{
			_local_3->processNext = nullptr;
			if (_local_17)
			{
				if (_local_13 != nullptr)
				{
					_local_14->processNegative = _local_12;
					_local_13 = nullptr;
					_local_14 = nullptr;
				}
				else
				{
					_local_12->processPositive = _local_15;
					_local_15 = _local_12;
					_local_15->geometry = _arg_2;
				}
			}
			else
			{
				if (_local_13 != nullptr)
				{
					_local_14->processNegative = _local_12;
				}
				else
				{
					_local_12->processPositive = _local_15;
					_local_15 = _local_12;
					_local_15->geometry = _arg_2;
					_local_13 = _local_12;
				}
				_local_14 = _local_12;
			}
			_local_12 = _local_4;
		}
		_local_3 = _local_4;
	}
/*
	if (_arg_1->debug)
	{
		_local_12 = _local_15;
		while (_local_12 != nullptr)
		{
			if ((_local_12.geometry.debug & Debug.EDGES))
			{
				_local_3 = _local_12;
				while (_local_3 != nullptr)
				{
					Debug.drawEdges(_arg_1, _local_3, 0xFF0000);
					_local_3 = _local_3->processNegative;
				}
			}
			_local_12 = _local_12.processPositive;
		}
	}
*/
	while (_local_15 != nullptr)
	{
		_local_12 = _local_15;
		_local_15 = _local_12->processPositive;
		_local_12->processPositive = nullptr;
		_arg_2 = _local_12->geometry;
		_local_12->geometry = nullptr;
		Shared<Face> _local_16 = nullptr;
		while (_local_12 != nullptr)
		{
			Shared<Face> _local_4 = _local_12->processNegative;
			if (_local_12->material != nullptr)
			{
				_local_12->processNegative = _local_16;
				_local_16 = _local_12;
			}
			else
			{
				_local_12->processNegative = nullptr;
				while (_local_12 != nullptr)
				{
					_local_3 = _local_12->processNext;
					_local_12->processNext = nullptr;
					_local_12 = _local_3;
				}
			}
			_local_12 = _local_4;
		}
		_local_12 = _local_16;
		while (_local_12 != nullptr)
		{
			Shared<Face> _local_4 = _local_12->processNegative;
			_local_12->processNegative = nullptr;
			_arg_1->addTransparent(_local_12, _arg_2->object.get());
			_local_12 = _local_4;
		}
	}
	_arg_2 = _local_7;
	while (_arg_2 != nullptr)
	{
		Shared<VG> _local_5 = _arg_2->next;
		_arg_2->destroy();
		_arg_2 = _local_5;
	}
}


Shared<VG> ConflictContainer::sortGeometry(Shared<VG> _arg_1, bool _arg_2, bool _arg_3)
{
	Shared<VG> _local_4 = _arg_1;
	Shared<VG> _local_5 = _arg_1->next;
	while (((!(_local_5 == nullptr)) && (!(_local_5->next == nullptr))))
	{
		_arg_1 = _arg_1->next;
		_local_5 = _local_5->next->next;
	}
	_local_5 = _arg_1->next;
	_arg_1->next = nullptr;
	if (_local_4->next != nullptr)
	{
		_local_4 = this->sortGeometry(_local_4, _arg_2, _arg_3);
	}
	else
	{
		if (_arg_2)
		{
			_local_4->boundMin = _local_4->boundMinX;
			_local_4->boundMax = _local_4->boundMaxX;
		}
		else
		{
			if (_arg_3)
			{
				_local_4->boundMin = _local_4->boundMinY;
				_local_4->boundMax = _local_4->boundMaxY;
			}
			else
			{
				_local_4->boundMin = _local_4->boundMinZ;
				_local_4->boundMax = _local_4->boundMaxZ;
			}
		}
	}
	if (_local_5->next != nullptr)
	{
		_local_5 = this->sortGeometry(_local_5, _arg_2, _arg_3);
	}
	else
	{
		if (_arg_2)
		{
			_local_5->boundMin = _local_5->boundMinX;
			_local_5->boundMax = _local_5->boundMaxX;
		}
		else
		{
			if (_arg_3)
			{
				_local_5->boundMin = _local_5->boundMinY;
				_local_5->boundMax = _local_5->boundMaxY;
			}
			else
			{
				_local_5->boundMin = _local_5->boundMinZ;
				_local_5->boundMax = _local_5->boundMaxZ;
			}
		}
	}
	bool _local_6 = (_local_4->boundMin < _local_5->boundMin);
	if (_local_6)
	{
		_arg_1 = _local_4;
		_local_4 = _local_4->next;
	}
	else
	{
		_arg_1 = _local_5;
		_local_5 = _local_5->next;
	}

	Shared<VG> _local_7 = _arg_1;
	while (true)
	{
		if (_local_4 == nullptr)
		{
			_local_7->next = _local_5;
			return (_arg_1);
		}
		if (_local_5 == nullptr)
		{
			_local_7->next = _local_4;
			return (_arg_1);
		}
		if (_local_6)
		{
			if (_local_4->boundMin < _local_5->boundMin)
			{
				_local_7 = _local_4;
				_local_4 = _local_4->next;
			}
			else
			{
				_local_7->next = _local_5;
				_local_7 = _local_5;
				_local_5 = _local_5->next;
				_local_6 = false;
			}
		}
		else
		{
			if (_local_5->boundMin < _local_4->boundMin)
			{
				_local_7 = _local_5;
				_local_5 = _local_5->next;
			}
			else
			{
				_local_7->next = _local_4;
				_local_7 = _local_4;
				_local_4 = _local_4->next;
				_local_6 = true;
			}
		}
	}
	return nullptr;
}

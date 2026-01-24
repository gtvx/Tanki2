#include "KDNode.h"
#include "../core/Vertex.h"
#include "../core/Face.h"
#include "../core/Wrapper.h"
#include "../materials/TextureMaterial.h"
#include "../core/Object3D.h"
#include "Object3D_isType.h"
#include "../objects/Mesh.h"
#include "../objects/BSP.h"
#include "Object3D_isType.h"
#include "../objects/Decal.h"
#include "../materials/Material.h"


class Receiver
{
public:
   std::shared_ptr<Receiver> next = nullptr;
   bool transparent = false;
   int buffer = -1;
   int firstIndex = -1;
   int numTriangles = 0;
};



KDNode::KDNode()
{
	setNegative(nullptr);
	setPositive(nullptr);
	axis = 0;
	objectList = nullptr;
	objectBoundList = nullptr;
	occluderList = nullptr;
	occluderBoundList = nullptr;
	receiverList = nullptr;
	coord = 0.;
	minCoord = 0.;
	maxCoord = 0.;
	boundMinX = 0.;
	boundMinY = 0.;
	boundMinZ = 0.;
	boundMaxX = 0.;
	boundMaxY = 0.;
	boundMaxZ = 0.;
}


void KDNode::createReceivers(QVector<QVector<float>*> &param1, QVector<QVector<uint>*> &param2)
{
   std::shared_ptr<Receiver> _loc3_ = nullptr;
   std::shared_ptr<Receiver> _loc5_ = nullptr;
   Shared<Vertex> _loc6_ = nullptr;
   Shared<Vertex> _loc7_ = nullptr;
   QVector<Shared<Face>> _loc8_;
   int _loc9_ = 0;
   std::shared_ptr<TextureMaterial> _loc10_;
   int _loc11_ = 0;
   int _loc12_ = 0;
   QVector<float> *_loc13_ = nullptr;
   QVector<uint> *_loc14_ = nullptr;
   int _loc15_ = 0;
   int _loc16_ = 0;
   int _loc17_ = 0;
   int _loc18_ = 0;
   Shared<Face> _loc19_ = nullptr;
   Shared<Wrapper> _loc20_ = nullptr;
   uint _loc21_ = 0;
   uint _loc22_ = 0;
   uint _loc23_ = 0;
   this->receiverList = nullptr;

   std::shared_ptr<Object3D> _loc4_ = this->objectList;

   while (_loc4_ != nullptr)
   {
	  _loc4_->composeMatrix();
	  _loc5_ = std::make_shared<Receiver>();
	  if (_loc3_ != nullptr)
	  {
		 _loc3_->next = _loc5_;
	  }
	  else
	  {
		 this->receiverList = _loc5_;
	  }

	  _loc3_ = _loc5_;


	  if (Object3D_isType<Mesh>(_loc4_.get()))
	  {
		  std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(_loc4_);
		 _loc7_ = mesh->vertexList;
		 _loc8_ = mesh->getFaces();

	  }
	  else if (Object3D_isType<BSP>(_loc4_.get()))
	  {
		   std::shared_ptr<BSP> bsp = std::dynamic_pointer_cast<BSP>(_loc4_);
		   _loc7_ = bsp->vertexList;
		   _loc8_ = bsp->faces;
	  }

	  _loc9_ = _loc8_.length();


	  _loc10_ = std::dynamic_pointer_cast<TextureMaterial>(_loc8_.at(0)->material);


	  if (_loc9_ > 0 && _loc10_ != nullptr)
	  {
		 _loc11_ = 0;
		 _loc6_ = _loc7_;

		 while (_loc6_ != nullptr)
		 {
			_loc11_++;
			_loc6_ = _loc6_->next;
		 }

		 _loc12_ = param1.length() - 1;
		 _loc13_ = param1[_loc12_];

		 if (_loc13_->length() / 3 + _loc11_ > 0xffff)
		 {
			_loc12_++;

			param1[_loc12_] = new QVector<float>();
			param2[_loc12_] = new QVector<uint>();

			_loc13_ = param1[_loc12_];
		 }

		 _loc14_ = param2[_loc12_];
		 _loc15_ = _loc13_->length();
		 _loc16_ = _loc15_ / 3;
		 _loc17_ = _loc14_->length();
		 _loc5_->buffer = _loc12_;
		 _loc5_->firstIndex = _loc17_;
		 _loc5_->transparent = _loc10_->transparent();
		 _loc6_ = _loc7_;

		 while (_loc6_ != nullptr)
		 {
			double a = _loc6_->x * _loc4_->matrix4.ma + _loc6_->y * _loc4_->matrix4.mb + _loc6_->z * _loc4_->matrix4.mc + _loc4_->matrix4.md;
			double b = _loc6_->x * _loc4_->matrix4.me + _loc6_->y * _loc4_->matrix4.mf + _loc6_->z * _loc4_->matrix4.mg + _loc4_->matrix4.mh;
			double c = _loc6_->x * _loc4_->matrix4.mi + _loc6_->y * _loc4_->matrix4.mj + _loc6_->z * _loc4_->matrix4.mk + _loc4_->matrix4.ml;


			_loc13_->append(a);
			_loc13_->append(b);
			_loc13_->append(c);
			_loc15_ += 3;

			_loc6_->index = _loc16_;
			_loc16_++;
			_loc6_ = _loc6_->next;
		 }

		 _loc18_ = 0;

		 while (_loc18_ < _loc9_)
		 {
			_loc19_ = _loc8_[_loc18_];
			if (_loc19_->normalX * _loc4_->matrix4.mi + _loc19_->normalY * _loc4_->matrix4.mj + _loc19_->normalZ * _loc4_->matrix4.mk >= -0.5)
			{
			   _loc20_ = _loc19_->wrapper;
			   _loc21_ = _loc20_->vertex->index;
			   _loc20_ = _loc20_->next;
			   _loc22_ = _loc20_->vertex->index;
			   _loc20_ = _loc20_->next;
			   while (_loc20_ != nullptr)
			   {
				  _loc23_ = _loc20_->vertex->index;

				  _loc14_->append(_loc21_);
				  _loc14_->append(_loc22_);
				  _loc14_->append(_loc23_);
				  _loc17_ += 3;
				  _loc5_->numTriangles++;
				  _loc22_ = _loc23_;
				  _loc20_ = _loc20_->next;
			   }
			}
			_loc18_++;
		 }
	  }
	  _loc4_ = _loc4_->next;
   }

   if (this->negative() != nullptr)
   {
	  this->negative()->createReceivers(param1, param2);
   }

   if (this->positive() != nullptr)
   {
	  this->positive()->createReceivers(param1, param2);
   }

}


void KDNode::collectReceivers(Shadow *param1)
{
	qDebug("used KDNode::collectReceivers");
	(void)param1;
}


void KDNode::collectPolygons(Decal *_arg_1,
							 double _arg_2,
							 double _arg_3,
							 double _arg_4,
							 double _arg_5,
							 double _arg_6,
							 double  _arg_7,
							 double  _arg_8,
							 double  _arg_9)
{
	if (this->negative() != nullptr)
	{
		bool _local_12 = (this->axis == 0);
		bool _local_13 = (this->axis == 1);
		double _local_14 = ((_local_12) ? _arg_4 : ((_local_13) ? _arg_6 : _arg_8));
		double _local_15 = ((_local_12) ? _arg_5 : ((_local_13) ? _arg_7 : _arg_9));
		if (_local_15 <= this->maxCoord)
		{
			this->negative()->collectPolygons(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
		}
		else
		{
			if (_local_14 >= this->minCoord)
			{
				this->positive()->collectPolygons(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
			}
			else
			{
				Object3D *_local_11 = this->objectBoundList.get();
				Object3D *_local_10 = this->objectList.get();
				while (_local_11 != nullptr)
				{
					if (_local_12)
					{
						if (((((_arg_6 < _local_11->bound.MaxY) && (_arg_7 > _local_11->bound.MinY)) && (_arg_8 < _local_11->bound.MaxZ)) && (_arg_9 > _local_11->bound.MinZ)))
						{
							this->clip(_arg_1, _arg_2, _arg_3, _local_10);
						}
					}
					else
					{
						if (_local_13)
						{
							if (((((_arg_4 < _local_11->bound.MaxX) && (_arg_5 > _local_11->bound.MinX)) && (_arg_8 < _local_11->bound.MaxZ)) && (_arg_9 > _local_11->bound.MinZ)))
							{
								this->clip(_arg_1, _arg_2, _arg_3, _local_10);
							}
						}
						else
						{
							if (((((_arg_4 < _local_11->bound.MaxX) && (_arg_5 > _local_11->bound.MinX)) && (_arg_6 < _local_11->bound.MaxY)) && (_arg_7 > _local_11->bound.MinY)))
							{
								this->clip(_arg_1, _arg_2, _arg_3, _local_10);
							}
						}
					}
					_local_11 = _local_11->next.get();
					_local_10 = _local_10->next.get();
				}
				this->negative()->collectPolygons(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
				this->positive()->collectPolygons(_arg_1, _arg_2, _arg_3, _arg_4, _arg_5, _arg_6, _arg_7, _arg_8, _arg_9);
			}
		}
	}
	else
	{
		Object3D *_local_10 = this->objectList.get();
		while (_local_10 != nullptr)
		{
			this->clip(_arg_1, _arg_2, _arg_3, _local_10);
			_local_10 = _local_10->next.get();
		}
	}
}


void KDNode::clip(Decal *_arg_1, double _arg_2, double _arg_3, Object3D *_arg_4)
{
	Shared<Face> _local_5 = nullptr;
	Shared<Vertex> _local_6 = nullptr;

	Shared<Wrapper> _local_7 = nullptr;


	QVector<Shared<Face>> _local_9;


	int _local_11;

	double _local_12;
	double _local_13;

	Shared<Vertex> _local_14;
	Shared<Vertex> _local_15;
	Shared<Vertex> _local_16;
	Shared<Vertex> _local_17;
	Shared<Vertex> _local_18;
	Shared<Vertex> _local_19;



	if (Object3D_isType<Mesh>(_arg_4))
	{
		//_local_8 = ((Mesh*)_arg_4)->vertexList;
		_local_5 = ((Mesh*)_arg_4)->faceList;
		if (((_local_5->material == nullptr) || (_local_5->material->transparent())))
		{
			return;
		}
		_local_9 = ((Mesh*)_arg_4)->getFaces();
	}
	else
	{
		if (Object3D_isType<BSP>(_arg_4))
		{
			//_local_8 = ((BSP*)_arg_4)->vertexList;
			_local_9 =  ((BSP*)_arg_4)->faces;
			_local_5 = _local_9[0];
			if (((_local_5->material == nullptr) || (_local_5->material->transparent())))
			{
				return;
			}
		}
	}

	_arg_4->composeAndAppend(_arg_1);
	_arg_4->calculateInverseMatrix();
	_arg_4->transformId++;

	int _local_10 = _local_9.length();

	_local_11 = 0;
	while (_local_11 < _local_10)
	{
		_local_5 = _local_9[_local_11];
		if ((((-(_local_5->normalX) * _arg_4->im.mc) - (_local_5->normalY * _arg_4->im.mg)) - (_local_5->normalZ * _arg_4->im.mk)) >= _arg_3)
		{
			_local_12 = (((_local_5->normalX * _arg_4->im.md) + (_local_5->normalY * _arg_4->im.mh)) + (_local_5->normalZ * _arg_4->im.ml));
			if (!((_local_12 <= (_local_5->offset - _arg_2)) || (_local_12 >= (_local_5->offset + _arg_2))))
			{
				_local_7 = _local_5->wrapper;
				while (_local_7 != nullptr)
				{
					_local_6 = _local_7->vertex;
					if (_local_6->transformId != _arg_4->transformId)
					{
						_local_6->cameraX = ((((_arg_4->matrix4.ma * _local_6->x) + (_arg_4->matrix4.mb * _local_6->y)) + (_arg_4->matrix4.mc * _local_6->z)) + _arg_4->matrix4.md);
						_local_6->cameraY = ((((_arg_4->matrix4.me * _local_6->x) + (_arg_4->matrix4.mf * _local_6->y)) + (_arg_4->matrix4.mg * _local_6->z)) + _arg_4->matrix4.mh);
						_local_6->cameraZ = ((((_arg_4->matrix4.mi * _local_6->x) + (_arg_4->matrix4.mj * _local_6->y)) + (_arg_4->matrix4.mk * _local_6->z)) + _arg_4->matrix4.ml);
						_local_6->transformId = _arg_4->transformId;
					}
					_local_7 = _local_7->next;
				}
				_local_7 = _local_5->wrapper;
				while (_local_7 != nullptr)
				{
					if (_local_7->vertex->cameraX > _arg_1->bound.MinX) break;
					_local_7 = _local_7->next;
				}
				if (_local_7 != nullptr)
				{
					_local_7 = _local_5->wrapper;
					while (_local_7 != nullptr)
					{
						if (_local_7->vertex->cameraX < _arg_1->bound.MaxX) break;
						_local_7 = _local_7->next;
					}
					if (_local_7 != nullptr)
					{
						_local_7 = _local_5->wrapper;
						while (_local_7 != nullptr)
						{
							if (_local_7->vertex->cameraY > _arg_1->bound.MinY) break;
							_local_7 = _local_7->next;
						}
						if (_local_7 != nullptr)
						{
							_local_7 = _local_5->wrapper;
							while (_local_7 != nullptr)
							{
								if (_local_7->vertex->cameraY < _arg_1->bound.MaxY) break;
								_local_7 = _local_7->next;
							}
							if (_local_7 != nullptr)
							{
								_local_7 = _local_5->wrapper;
								while (_local_7 != nullptr)
								{
									if (_local_7->vertex->cameraZ > _arg_1->bound.MinZ) break;
									_local_7 = _local_7->next;
								}
								if (_local_7 != nullptr)
								{
									_local_7 = _local_5->wrapper;
									while (_local_7 != nullptr)
									{
										if (_local_7->vertex->cameraZ < _arg_1->bound.MaxZ) break;
										_local_7 = _local_7->next;
									}
									if (_local_7 != nullptr)
									{
										_local_18 = nullptr;
										_local_19 = nullptr;
										_local_7 = _local_5->wrapper;
										while (_local_7 != nullptr)
										{
											_local_6 = _local_7->vertex;
											_local_16 = Vertex::create();
											_local_16->x = _local_6->cameraX;
											_local_16->y = _local_6->cameraY;
											_local_16->z = _local_6->cameraZ;
											_local_16->normalX = (((_arg_4->matrix4.ma * _local_6->normalX) + (_arg_4->matrix4.mb * _local_6->normalY)) + (_arg_4->matrix4.mc * _local_6->normalZ));
											_local_16->normalY = (((_arg_4->matrix4.me * _local_6->normalX) + (_arg_4->matrix4.mf * _local_6->normalY)) + (_arg_4->matrix4.mg * _local_6->normalZ));
											_local_16->normalZ = (((_arg_4->matrix4.mi * _local_6->normalX) + (_arg_4->matrix4.mj * _local_6->normalY)) + (_arg_4->matrix4.mk * _local_6->normalZ));
											if (_local_19 != nullptr)
											{
												_local_19->next = _local_16;
											}
											else
											{
												_local_18 = _local_16;
											}
											_local_19 = _local_16;
											_local_7 = _local_7->next;
										}
										_local_14 = _local_19;
										_local_15 = _local_18;
										_local_18 = nullptr;
										_local_19 = nullptr;
										while (_local_15 != nullptr)
										{
											_local_17 = _local_15->next;
											_local_15->next = nullptr;
											if ((((_local_15->z > _arg_1->bound.MinZ) && (_local_14->z <= _arg_1->bound.MinZ)) || ((_local_15->z <= _arg_1->bound.MinZ) && (_local_14->z > _arg_1->bound.MinZ))))
											{
												_local_13 = ((_arg_1->bound.MinZ - _local_14->z) / (_local_15->z - _local_14->z));
												_local_16 = Vertex::create();
												_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
												_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
												_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
												_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
												_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
												_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
												if (_local_19 != nullptr)
												{
													_local_19->next = _local_16;
												}
												else
												{
													_local_18 = _local_16;
												}
												_local_19 = _local_16;
											}
											if (_local_15->z > _arg_1->bound.MinZ)
											{
												if (_local_19 != nullptr)
												{
													_local_19->next = _local_15;
												}
												else
												{
													_local_18 = _local_15;
												}
												_local_19 = _local_15;
											}
											_local_14 = _local_15;
											_local_15 = _local_17;
										}
										if (_local_18 != nullptr)
										{
											_local_14 = _local_19;
											_local_15 = _local_18;
											_local_18 = nullptr;
											_local_19 = nullptr;
											while (_local_15 != nullptr)
											{
												_local_17 = _local_15->next;
												_local_15->next = nullptr;
												if ((((_local_15->z < _arg_1->bound.MaxZ) && (_local_14->z >= _arg_1->bound.MaxZ)) || ((_local_15->z >= _arg_1->bound.MaxZ) && (_local_14->z < _arg_1->bound.MaxZ))))
												{
													_local_13 = ((_arg_1->bound.MaxZ - _local_14->z) / (_local_15->z - _local_14->z));
													_local_16 = Vertex::create();
													_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
													_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
													_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
													_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
													_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
													_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
													if (_local_19 != nullptr)
													{
														_local_19->next = _local_16;
													}
													else
													{
														_local_18 = _local_16;
													}
													_local_19 = _local_16;
												}
												if (_local_15->z < _arg_1->bound.MaxZ)
												{
													if (_local_19 != nullptr)
													{
														_local_19->next = _local_15;
													}
													else
													{
														_local_18 = _local_15;
													}
													_local_19 = _local_15;
												}
												_local_14 = _local_15;
												_local_15 = _local_17;
											}
											if (_local_18 != nullptr)
											{
												_local_14 = _local_19;
												_local_15 = _local_18;
												_local_18 = nullptr;
												_local_19 = nullptr;
												while (_local_15 != nullptr)
												{
													_local_17 = _local_15->next;
													_local_15->next = nullptr;
													if ((((_local_15->x > _arg_1->bound.MinX) && (_local_14->x <= _arg_1->bound.MinX)) || ((_local_15->x <= _arg_1->bound.MinX) && (_local_14->x > _arg_1->bound.MinX))))
													{
														_local_13 = ((_arg_1->bound.MinX - _local_14->x) / (_local_15->x - _local_14->x));
														_local_16 = Vertex::create();
														_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
														_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
														_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
														_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
														_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
														_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
														if (_local_19 != nullptr)
														{
															_local_19->next = _local_16;
														}
														else
														{
															_local_18 = _local_16;
														}
														_local_19 = _local_16;
													}
													if (_local_15->x > _arg_1->bound.MinX)
													{
														if (_local_19 != nullptr)
														{
															_local_19->next = _local_15;
														}
														else
														{
															_local_18 = _local_15;
														}
														_local_19 = _local_15;
													}
													_local_14 = _local_15;
													_local_15 = _local_17;
												}
												if (_local_18 != nullptr)
												{
													_local_14 = _local_19;
													_local_15 = _local_18;
													_local_18 = nullptr;
													_local_19 = nullptr;
													while (_local_15 != nullptr)
													{
														_local_17 = _local_15->next;
														_local_15->next = nullptr;
														if ((((_local_15->x < _arg_1->bound.MaxX) && (_local_14->x >= _arg_1->bound.MaxX)) || ((_local_15->x >= _arg_1->bound.MaxX) && (_local_14->x < _arg_1->bound.MaxX))))
														{
															_local_13 = ((_arg_1->bound.MaxX - _local_14->x) / (_local_15->x - _local_14->x));
															_local_16 = Vertex::create();
															_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
															_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
															_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
															_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
															_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
															_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
															if (_local_19 != nullptr)
															{
																_local_19->next = _local_16;
															}
															else
															{
																_local_18 = _local_16;
															}
															_local_19 = _local_16;
														}
														if (_local_15->x < _arg_1->bound.MaxX)
														{
															if (_local_19 != nullptr)
															{
																_local_19->next = _local_15;
															}
															else
															{
																_local_18 = _local_15;
															}
															_local_19 = _local_15;
														}
														_local_14 = _local_15;
														_local_15 = _local_17;
													}
													if (_local_18 != nullptr)
													{
														_local_14 = _local_19;
														_local_15 = _local_18;
														_local_18 = nullptr;
														_local_19 = nullptr;
														while (_local_15 != nullptr)
														{
															_local_17 = _local_15->next;
															_local_15->next = nullptr;
															if ((((_local_15->y > _arg_1->bound.MinY) && (_local_14->y <= _arg_1->bound.MinY)) || ((_local_15->y <= _arg_1->bound.MinY) && (_local_14->y > _arg_1->bound.MinY))))
															{
																_local_13 = ((_arg_1->bound.MinY - _local_14->y) / (_local_15->y - _local_14->y));
																_local_16 = Vertex::create();
																_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
																_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
																_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
																_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
																_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
																_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
																if (_local_19 != nullptr)
																{
																	_local_19->next = _local_16;
																}
																else
																{
																	_local_18 = _local_16;
																}
																_local_19 = _local_16;
															}
															if (_local_15->y > _arg_1->bound.MinY)
															{
																if (_local_19 != nullptr)
																{
																	_local_19->next = _local_15;
																}
																else
																{
																	_local_18 = _local_15;
																}
																_local_19 = _local_15;
															}
															_local_14 = _local_15;
															_local_15 = _local_17;
														}
														if (_local_18 != nullptr)
														{
															_local_14 = _local_19;
															_local_15 = _local_18;
															_local_18 = nullptr;
															_local_19 = nullptr;
															while (_local_15 != nullptr)
															{
																_local_17 = _local_15->next;
																_local_15->next = nullptr;
																if ((((_local_15->y < _arg_1->bound.MaxY) && (_local_14->y >= _arg_1->bound.MaxY)) || ((_local_15->y >= _arg_1->bound.MaxY) && (_local_14->y < _arg_1->bound.MaxY))))
																{
																	_local_13 = ((_arg_1->bound.MaxY - _local_14->y) / (_local_15->y - _local_14->y));
																	_local_16 = Vertex::create();
																	_local_16->x = (_local_14->x + ((_local_15->x - _local_14->x) * _local_13));
																	_local_16->y = (_local_14->y + ((_local_15->y - _local_14->y) * _local_13));
																	_local_16->z = (_local_14->z + ((_local_15->z - _local_14->z) * _local_13));
																	_local_16->normalX = (_local_14->normalX + ((_local_15->normalX - _local_14->normalX) * _local_13));
																	_local_16->normalY = (_local_14->normalY + ((_local_15->normalY - _local_14->normalY) * _local_13));
																	_local_16->normalZ = (_local_14->normalZ + ((_local_15->normalZ - _local_14->normalZ) * _local_13));
																	if (_local_19 != nullptr)
																	{
																		_local_19->next = _local_16;
																	}
																	else
																	{
																		_local_18 = _local_16;
																	}
																	_local_19 = _local_16;
																}
																if (_local_15->y < _arg_1->bound.MaxY)
																{
																	if (_local_19 != nullptr)
																	{
																		_local_19->next = _local_15;
																	}
																	else
																	{
																		_local_18 = _local_15;
																	}
																	_local_19 = _local_15;
																}
																_local_14 = _local_15;
																_local_15 = _local_17;
															}
															if (_local_18 != nullptr)
															{
																_local_5 = Face::create(3);
																Shared<Wrapper> _local_20 = nullptr;
																_local_6 = _local_18;
																while (_local_6 != nullptr)
																{
																	_local_17 = _local_6->next;
																	_local_6->next = _arg_1->vertexList;
																	_arg_1->vertexList = _local_6;
																	_local_6->u = ((_local_6->x - _arg_1->bound.MinX) / (_arg_1->bound.MaxX - _arg_1->bound.MinX));
																	_local_6->v = ((_local_6->y - _arg_1->bound.MinY) / (_arg_1->bound.MaxY - _arg_1->bound.MinY));
																	if (_local_20 != nullptr)
																	{
																		_local_20->next = Wrapper::create();
																		_local_20 = _local_20->next;
																	}
																	else
																	{
																		_local_5->wrapper = Wrapper::create();
																		_local_20 = _local_5->wrapper;
																	}
																	_local_20->vertex = _local_6;
																	_local_6 = _local_17;
																}
																_local_5->calculateBestSequenceAndNormal();
																_local_5->next = _arg_1->faceList;
																_arg_1->faceList = _local_5;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		_local_11++;
	}
}

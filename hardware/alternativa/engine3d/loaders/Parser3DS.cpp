#include "Parser3DS.h"
#include "qhash_std_string.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "hardware/alternativa/engine3d/materials/Material.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "hardware/alternativa/engine3d/materials/FillMaterial.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "MyMath.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "Error.h"
#include <QDebug>
#include "flash/geom/Matrix.h"
#include "flash/geom/Vector3D.h"
#include "stream/stream_read.h"
#include "qhash_shared_ptr.h"




namespace
{

	struct StreamRead : public stream_read
	{
	private:
		uint8_t const * begin;

	public:

		void init(const QByteArray &data)
		{
			begin = (const uint8_t*)data.constData();
			stream_read_init(this, begin, data.size());
		}

		int getPosition()
		{
			return data - begin;
		}

		void setPosition(int pos)
		{
			data = begin + pos;
		}

		int bytesAvailable()
		{
			return stream_read_available(this);
		}

		uint8_t readUnsignedByte()
		{
			return stream_read_uint8(this);
		}

		uint16_t readUnsignedShort()
		{
			return stream_read_uint16_le(this);
		}

		uint32_t readUnsignedInt()
		{
			return stream_read_uint32_le(this);
		}

		float readFloat()
		{
			return stream_read_float_le(this);
		}

		const uint8_t* getData()
		{
			return this->data;
		}
	};


	struct ChunkInfo
	{
		int id;
		int size;
		int dataSize;
		int dataPosition;
		int nextChunkPosition;
	};


	struct ObjectData
	{
		std::string name;

		bool vertices_valid = false;
		QVector<double> vertices;

		bool uvs_valid = false;
		QVector<double> uvs;

		bool faces_valid = false;
		QVector<int> faces;

		bool smoothingGroups_valid = false;
		QVector<uint> smoothingGroups;

		bool surfaces_valid = false;
		QHash<std::string, QVector<int>> surfaces;

		double a;
		double b;
		double c;
		double d;
		double e;
		double f;
		double g;
		double h;
		double i;
		double j;
		double k;
		double l;
	};


	struct AnimationData
	{
		std::string objectName;

		std::shared_ptr<Object3D> object;

		int parentIndex;

		bool pivot_valid = false;
		Vector3D pivot;

		bool position_valid = false;
		Vector3D position;

		bool rotation_valid = false;
		Vector3D rotation;

		bool scale_valid = false;
		Vector3D scale;

		bool isInstance = false;
	};


	struct MapData
	{
		QString filename;
		double scaleU = 1;
		double scaleV = 1;
		double offsetU = 0;
		double offsetV = 0;
		double rotation = 0;
	};


	struct MaterialData
	{
		std::string name;

		int color;
		int specular;
		int glossiness;
		int transparency;

		bool diffuseMap_valid = false;
		MapData diffuseMap;

		bool opacityMap_valid = false;
		MapData opacityMap;

		bool matrix_valid = false;
		Matrix matrix;

		std::shared_ptr<Material> material;
	};


	enum
	{
		CHUNK_MAIN = 19789,
		CHUNK_VERSION = 2,
		CHUNK_SCENE = 15677,
		CHUNK_ANIMATION = 45056,
		CHUNK_OBJECT = 16384,
		CHUNK_TRIMESH = 16640,
		CHUNK_VERTICES = 16656,
		CHUNK_FACES = 16672,
		CHUNK_FACESMATERIAL = 16688,
		CHUNK_FACESSMOOTH = 16720,
		CHUNK_MAPPINGCOORDS = 16704,
		CHUNK_TRANSFORMATION = 16736,
		CHUNK_MATERIAL = 45055,
	};

}


static int id = 100;


class Parser3DSPrivate
{
	bool objectDatas_valid = false;
	QHash<std::string, std::shared_ptr<ObjectData>> objectDatas; //Object

	bool materialDatas_valid = false;
	QHash<std::string, std::shared_ptr<MaterialData>> materialDatas; //Object

	bool animationDatas_valid = false;
	QVector<std::shared_ptr<AnimationData>> animationDatas; //Array

	StreamRead data;
	std::string name;
	int id;

public:

	QVector<std::shared_ptr<Object3D>> objects;
	QVector<std::shared_ptr<Object3D>> parents;
	QVector<std::shared_ptr<Material>> materials;
	QVector<std::shared_ptr<TextureMaterial>> textureMaterials;


	Parser3DSPrivate()
	{
		this->id = ::id++;
	}


	~Parser3DSPrivate()
	{

	}


	void setName(const QString &name)
	{
		this->name = name.toStdString();
		//qDebug() << this->id << name;
	}


	bool parse(const QByteArray &data, const QString &texturesBaseURL, double scale)
	{
		this->data.init(data);

		if (this->data.bytesAvailable() < 6)
			return false;

		this->parse3DSChunk(this->data.getPosition(), this->data.bytesAvailable());
		this->buildContent(texturesBaseURL, scale);

		return true;
	}


	void parse3DSChunk(int position, int param2)
	{
		if (param2 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(position);
		this->data.setPosition(position);
		switch(chunkinfo.id)
		{
		case CHUNK_MAIN:
			this->parseMainChunk(chunkinfo.dataPosition, chunkinfo.dataSize);
			break;
		}

		this->parse3DSChunk(chunkinfo.nextChunkPosition, param2 - chunkinfo.size);
	}


	ChunkInfo readChunkInfo(int param1)
	{
		this->data.setPosition(param1);
		ChunkInfo chunkinfo;
		chunkinfo.id = this->data.readUnsignedShort();
		chunkinfo.size = this->data.readUnsignedInt();
		chunkinfo.dataSize = chunkinfo.size - 6;
		chunkinfo.dataPosition = this->data.getPosition();
		chunkinfo.nextChunkPosition = param1 + chunkinfo.size;
		return chunkinfo;
	}


	void buildContent(const QString &texturesBaseURL, double scale)
	{
		QHashIterator<std::string, std::shared_ptr<MaterialData>> iter(this->materialDatas);
		while (iter.hasNext())
		{
			iter.next();
			MaterialData *_loc7_ = iter.value().get();


			if (_loc7_->diffuseMap_valid)
			{
				const MapData *_loc8_ = &_loc7_->diffuseMap;

				Matrix *_loc9_ = &_loc7_->matrix;
				double _loc10_ = (_loc8_->rotation * MyMath::PI) / 180;
				_loc9_->translate(-_loc8_->offsetU, _loc8_->offsetV);
				_loc9_->translate(-0.5,-0.5);
				_loc9_->rotate(-_loc10_);
				_loc9_->scale(_loc8_->scaleU, _loc8_->scaleV);
				_loc9_->translate(0.5,0.5);
				_loc7_->matrix_valid = true;

				std::shared_ptr<TextureMaterial> _loc11_ = new_TextureMaterial(id);
				_loc11_->name = iter.key();
				_loc11_->diffuseMapURL = texturesBaseURL + _loc8_->filename;
				_loc11_->opacityMapURL = _loc7_->opacityMap_valid ? (texturesBaseURL + _loc7_->opacityMap.filename) : nullptr;
				_loc7_->material = _loc11_;
				_loc11_->name = _loc7_->name;
				this->textureMaterials.append(_loc11_);
			}
			else
			{
				std::shared_ptr<FillMaterial> _loc12_ = new_FillMaterial(_loc7_->color);
				_loc7_->material = _loc12_;
				_loc12_->name = _loc7_->name;
			}
			this->materials.append(_loc7_->material);
		}

		if (this->animationDatas_valid)
		{
			if (this->objectDatas_valid)
			{
				int _loc14_ = this->animationDatas.count();
				int _loc13_ = 0;
				while (_loc13_ < _loc14_)
				{
					AnimationData *_loc15_ = this->animationDatas.at(_loc13_).get();
					std::string _loc4_ = _loc15_->objectName;
					ObjectData *_loc5_ = this->objectDatas.value(_loc4_).get();

					if (_loc5_ != nullptr)
					{
						int _loc16_ = _loc13_ + 1;
						int _loc17_ = 1;
						while (_loc16_ < _loc14_)
						{
							AnimationData *_loc18_ = this->animationDatas.at(_loc16_).get();
							if (!_loc18_->isInstance && _loc4_ == _loc18_->objectName)
							{
								ObjectData *_loc19_ = new ObjectData();
								std::string _loc20_ = _loc4_ + QString::number(_loc17_++).toStdString();
								_loc19_->name = _loc20_;
								this->objectDatas.insert(_loc20_, std::shared_ptr<ObjectData>(_loc19_));

								_loc18_->objectName = _loc20_;
								_loc19_->vertices = _loc5_->vertices;
								_loc19_->uvs = _loc5_->uvs;
								_loc19_->faces = _loc5_->faces;
								_loc19_->smoothingGroups = _loc5_->smoothingGroups;
								_loc19_->surfaces = _loc5_->surfaces;
								_loc19_->a = _loc5_->a;
								_loc19_->b = _loc5_->b;
								_loc19_->c = _loc5_->c;
								_loc19_->d = _loc5_->d;
								_loc19_->e = _loc5_->e;
								_loc19_->f = _loc5_->f;
								_loc19_->g = _loc5_->g;
								_loc19_->h = _loc5_->h;
								_loc19_->i = _loc5_->i;
								_loc19_->j = _loc5_->j;
								_loc19_->k = _loc5_->k;
								_loc19_->l = _loc5_->l;
							}
							_loc16_++;
						}
					}

					std::shared_ptr<Object3D> _loc6_;

					if (_loc5_ != nullptr && _loc5_->vertices.count() != 0) {
						_loc6_ = new_Mesh();
						this->buildMesh((Mesh*)_loc6_.get(), _loc5_, _loc15_, scale);
					} else
						_loc6_ = new_Object3D();

					_loc6_->name = _loc4_;

					_loc6_->name_class = this->name;

					_loc15_->object = _loc6_;
					if (_loc15_->position_valid) {
						_loc6_->x = _loc15_->position.getX() * scale;
						_loc6_->y = _loc15_->position.getY() * scale;
						_loc6_->z = _loc15_->position.getZ() * scale;
					} else {
						_loc6_->x = 0;
						_loc6_->y = 0;
						_loc6_->z = 0;
					}
					if (_loc15_->rotation_valid) {
						_loc6_->rotationX = _loc15_->rotation.getX();
						_loc6_->rotationY = _loc15_->rotation.getY();
						_loc6_->rotationZ = _loc15_->rotation.getZ();
					} else {
						_loc6_->rotationX = 0;
						_loc6_->rotationY = 0;
						_loc6_->rotationZ = 0;
					}
					if (_loc15_->scale_valid) {
						_loc6_->scaleX = _loc15_->scale.getX();
						_loc6_->scaleY = _loc15_->scale.getY();
						_loc6_->scaleZ = _loc15_->scale.getZ();
					} else {
						_loc6_->scaleX = 1.0;
						_loc6_->scaleY = 1.0;
						_loc6_->scaleZ = 1.0;
					}
					_loc13_++;
				}
				_loc13_ = 0;
				while (_loc13_ < _loc14_) {
					AnimationData *animationData = this->animationDatas.at(_loc13_).get();
					this->objects.append(animationData->object);
					this->parents.append(animationData->parentIndex == 65535 ? nullptr : this->animationDatas.at(animationData->parentIndex)->object);
					_loc13_++;
				}
			}
		}
		else
		{
			QHashIterator<std::string, std::shared_ptr<ObjectData>> iter(objectDatas);
			while (iter.hasNext()) {
				iter.next();
				ObjectData *objectdata = iter.value().get();
				if (objectdata->vertices_valid) {
					std::shared_ptr<Object3D> object3d = new_Mesh();
					object3d->name = iter.key();
					this->buildMesh((Mesh*)object3d.get(), objectdata, nullptr, scale);
					this->objects.append(object3d);
					this->parents.append(nullptr);
				}
			}
		}
	}


	void parseMainChunk(int param1, int param2)
	{
		if (param2 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param1);
		switch (chunkinfo.id) {
		case CHUNK_VERSION:
			break;
		case CHUNK_SCENE:
			this->parse3DChunk(chunkinfo.dataPosition, chunkinfo.dataSize);
			break;
		case CHUNK_ANIMATION:
			this->parseAnimationChunk(chunkinfo.dataPosition, chunkinfo.dataSize);
			break;
		}

		this->parseMainChunk(chunkinfo.nextChunkPosition, param2 - chunkinfo.size);
	}


	void buildMesh(Mesh *param1, ObjectData *param2, AnimationData *param3, double param4)
	{
		QVector<Shared<Vertex>> _loc5_;
		QVector<Shared<Face>> _loc6_;

		bool _loc13_ = false;

		if (param3 != nullptr)
		{
			double _loc16_ = param2->a;
			double _loc17_ = param2->b;
			double _loc18_ = param2->c;
			double _loc19_ = param2->d;
			double _loc20_ = param2->e;
			double _loc21_ = param2->f;
			double _loc22_ = param2->g;
			double _loc23_ = param2->h;
			double _loc24_ = param2->i;
			double _loc25_ = param2->j;
			double _loc26_ = param2->k;
			double _loc27_ = param2->l;
			double _loc28_ = 1 / (-_loc18_ * _loc21_ * _loc24_ + _loc17_ * _loc22_ * _loc24_ + _loc18_ * _loc20_ * _loc25_ - _loc16_ * _loc22_ * _loc25_ - _loc17_ * _loc20_ * _loc26_ + _loc16_ * _loc21_ * _loc26_);
			param2->a = (-_loc22_ * _loc25_ + _loc21_ * _loc26_) * _loc28_;
			param2->b = (_loc18_ * _loc25_ - _loc17_ * _loc26_) * _loc28_;
			param2->c = (-_loc18_ * _loc21_ + _loc17_ * _loc22_) * _loc28_;
			param2->d = (_loc19_ * _loc22_ * _loc25_ - _loc18_ * _loc23_ * _loc25_ - _loc19_ * _loc21_ * _loc26_ + _loc17_ * _loc23_ * _loc26_ + _loc18_ * _loc21_ * _loc27_ - _loc17_ * _loc22_ * _loc27_) * _loc28_;
			param2->e = (_loc22_ * _loc24_ - _loc20_ * _loc26_) * _loc28_;
			param2->f = (-_loc18_ * _loc24_ + _loc16_ * _loc26_) * _loc28_;
			param2->g = (_loc18_ * _loc20_ - _loc16_ * _loc22_) * _loc28_;
			param2->h = (_loc18_ * _loc23_ * _loc24_ - _loc19_ * _loc22_ * _loc24_ + _loc19_ * _loc20_ * _loc26_ - _loc16_ * _loc23_ * _loc26_ - _loc18_ * _loc20_ * _loc27_ + _loc16_ * _loc22_ * _loc27_) * _loc28_;
			param2->i = (-_loc21_ * _loc24_ + _loc20_ * _loc25_) * _loc28_;
			param2->j = (_loc17_ * _loc24_ - _loc16_ * _loc25_) * _loc28_;
			param2->k = (-_loc17_ * _loc20_ + _loc16_ * _loc21_) * _loc28_;
			param2->l = (_loc19_ * _loc21_ * _loc24_ - _loc17_ * _loc23_ * _loc24_ - _loc19_ * _loc20_ * _loc25_ + _loc16_ * _loc23_ * _loc25_ + _loc17_ * _loc20_ * _loc27_ - _loc16_ * _loc21_ * _loc27_) * _loc28_;
			if (param3->pivot_valid)
			{
				param2->d = param2->d - param3->pivot.getX();
				param2->h = param2->h - param3->pivot.getY();
				param2->l = param2->l - param3->pivot.getZ();
			}
			_loc13_ = true;
		}

		if (param2->vertices_valid)
		{
			bool _loc29_ = param2->uvs.count() > 0;
			int _loc9_ = 0;
			int _loc10_ = 0;
			int _loc14_ = param2->vertices.count();
			while (_loc9_ < _loc14_)
			{
				Shared<Vertex> _loc12_ = Vertex::create();
				if (_loc13_) {
					double _loc30_ = param2->vertices.at(_loc9_++);
					double _loc31_ = param2->vertices.at(_loc9_++);
					double _loc32_ = param2->vertices.at(_loc9_++);
					_loc12_->x = param2->a * _loc30_ + param2->b * _loc31_ + param2->c * _loc32_ + param2->d;
					_loc12_->y = param2->e * _loc30_ + param2->f * _loc31_ + param2->g * _loc32_ + param2->h;
					_loc12_->z = param2->i * _loc30_ + param2->j * _loc31_ + param2->k * _loc32_ + param2->l;
				} else {
					_loc12_->x = param2->vertices.at(_loc9_++);
					_loc12_->y = param2->vertices.at(_loc9_++);
					_loc12_->z = param2->vertices.at(_loc9_++);
				}
				_loc12_->x = _loc12_->x * param4;
				_loc12_->y = _loc12_->y * param4;
				_loc12_->z = _loc12_->z * param4;
				if (_loc29_) {
					_loc12_->u = param2->uvs.at(_loc10_++);
					_loc12_->v = 1 - param2->uvs.at(_loc10_++);
				}
				_loc12_->transformId = -1;
				_loc5_.append(_loc12_);
				_loc12_->next = param1->vertexList;
				param1->vertexList = _loc12_;
			}
		}

		if (param2->faces_valid)
		{
			int _loc9_ = 0;
			int _loc10_ = 0;
			int _loc14_ = param2->faces.count();

			Shared<Face> _loc33_ = nullptr;

			while (_loc9_ < _loc14_)
			{
				Shared<Face> face = Face::create(16);
				face->wrapper = Wrapper::create();
				face->wrapper->next = Wrapper::create();
				face->wrapper->next->next = Wrapper::create();
				face->wrapper->vertex = _loc5_.at(param2->faces.at(_loc9_++));
				face->wrapper->next->vertex = _loc5_.at(param2->faces.at(_loc9_++));
				face->wrapper->next->next->vertex = _loc5_.at(param2->faces.at(_loc9_++));

				if (_loc10_ < param2->smoothingGroups.count())
				{
					face->smoothingGroups = param2->smoothingGroups.at(_loc10_++);
				}
				else
				{
					qDebug() << "error Parser3DS::buildMesh" << param2 << param2->smoothingGroups << param2->smoothingGroups.count() << _loc10_;
					face->smoothingGroups = 0;
				}

				_loc6_.append(face);

				if (_loc33_ != nullptr)
					_loc33_->next = face;
				else
					param1->faceList = face;

				_loc33_ = face;

			}
		}

		if (param2->surfaces_valid)
		{
			QHashIterator<std::string, QVector<int>> iter(param2->surfaces);
			while (iter.hasNext())
			{
				iter.next();

				const QVector<int> _loc35_ = iter.value();
				MaterialData *_loc36_ = this->materialDatas.value(iter.key()).get();
				std::shared_ptr<Material> _loc37_ = _loc36_->material;
				int _loc9_ = 0;
				while (_loc9_ < _loc35_.length())
				{
					int d = _loc35_[_loc9_];
					Shared<Face> _loc11_ = _loc6_.at(d);
					_loc11_->material = _loc37_;
					if (_loc36_->matrix_valid)
					{
						Weak<Wrapper> _loc38_ = _loc11_->wrapper;
						while (_loc38_ != nullptr)
						{
							Vertex *_loc12_ = _loc38_->vertex.get();
							if (_loc12_->transformId < 0)
							{
								double _loc39_ = _loc12_->u;
								double _loc40_ = _loc12_->v;
								_loc12_->u = _loc36_->matrix.a * _loc39_ + _loc36_->matrix.b * _loc40_ + _loc36_->matrix.tx;
								_loc12_->v = _loc36_->matrix.c * _loc39_ + _loc36_->matrix.d * _loc40_ + _loc36_->matrix.ty;
								_loc12_->transformId = 0;
							}
							_loc38_ = _loc38_->next;
						}
					}
					_loc9_++;
				}
			}
		}


		std::shared_ptr<FillMaterial> _loc15_ = new_FillMaterial(8355711);
		_loc15_->name = "default";

		Weak<Face> face = param1->faceList;
		while (face != nullptr)
		{
			if (face->material == nullptr)
			{
				face->material = _loc15_;
			}

			face = face->next;
		}

		param1->calculateFacesNormals(true);
		param1->calculateBounds();
	}


	void parse3DChunk(int param1, int param2)
	{
		while (param2 >= 6) {
			const ChunkInfo chunkinfo = this->readChunkInfo(param1);
			switch(chunkinfo.id)
			{
			case CHUNK_MATERIAL: {
				auto materialData = std::make_shared<MaterialData>();
				this->parseMaterialChunk(materialData, chunkinfo.dataPosition, chunkinfo.dataSize);
				break;
			}
			case CHUNK_OBJECT:
				this->parseObject(chunkinfo);
				break;
			}

			param1 = chunkinfo.nextChunkPosition;
			param2 = param2 - chunkinfo.size;
		}
	}


	void parseMaterialChunk(std::shared_ptr<MaterialData> &param1, int param2, int param3)
	{
		if (param3 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param2);
		switch(chunkinfo.id)
		{
		case 0xa000:
			this->parseMaterialName(param1);
			break;
		case 0xa010:
			break;
		case 0xa020:
			this->data.setPosition(chunkinfo.dataPosition + 6);
			param1->color = (this->data.readUnsignedByte() << 16) + (this->data.readUnsignedByte() << 8) + this->data.readUnsignedByte();
			break;
		case 0xa030:
			break;
		case 0xa040:
			this->data.setPosition(chunkinfo.dataPosition + 6);
			param1->glossiness = this->data.readUnsignedShort();
			break;
		case 41025:
			this->data.setPosition(chunkinfo.dataPosition + 6);
			param1->specular = this->data.readUnsignedShort();
			break;
		case 0xa050:
			this->data.setPosition(chunkinfo.dataPosition + 6);
			param1->transparency = this->data.readUnsignedShort();
			break;
		case 41472:
			param1->diffuseMap_valid = true;
			this->parseMapChunk(param1->name, &param1->diffuseMap, chunkinfo.dataPosition, chunkinfo.dataSize);
			break;
		case 41786:
			break;
		case 41488:
			param1->opacityMap_valid = true;
			this->parseMapChunk(param1->name, &param1->opacityMap, chunkinfo.dataPosition, chunkinfo.dataSize);
			break;
		case 692 * 60:
			break;
		case 41788:
			break;
		case 41476:
			break;
		case 41789:
			break;
		case 41504:
			break;
		}
		this->parseMaterialChunk(param1, chunkinfo.nextChunkPosition, param3 - chunkinfo.size);
	}


	void parseMapChunk(const std::string &param1, MapData *param2, int param3, int param4)
	{
		if (param4 < 6)
			return;

		const ChunkInfo chunkinfo= this->readChunkInfo(param3);
		switch(chunkinfo.id)
		{
			case 41728:
				param2->filename = QString::fromStdString(this->getString(chunkinfo.dataPosition)).toLower();
				break;
			case 41809:
				break;
			case 41812:
				param2->scaleU = this->data.readFloat();
				break;
			case 41814:
				param2->scaleV = this->data.readFloat();
				break;
			case 41816:
				param2->offsetU = this->data.readFloat();
				break;
			case 41818:
				param2->offsetV = this->data.readFloat();
				break;
			case 697 * 60:
				param2->rotation = this->data.readFloat();
				break;
		}
		this->parseMapChunk(param1, param2, chunkinfo.nextChunkPosition, param4 - chunkinfo.size);
	}


	std::string getString(int param1)
	{
		this->data.setPosition(param1);
		QString d = QString::fromUtf8((char*)this->data.getData());
		this->data.setPosition(this->data.getPosition() + d.length() + 1);
		return d.toStdString();
	}


	void parseMaterialName(std::shared_ptr<MaterialData> &daterialData)
	{
		materialDatas_valid = true;
		daterialData->name = this->getString(this->data.getPosition());
		this->materialDatas.insert(daterialData->name, daterialData);
	}


	void parseObject(const ChunkInfo &chunkInfo)
	{
		this->objectDatas_valid = true;
		ObjectData *objectdata = new ObjectData();
		objectdata->name = this->getString(chunkInfo.dataPosition);
		this->objectDatas.insert(objectdata->name, std::shared_ptr<ObjectData>(objectdata));
		int _loc3_ = objectdata->name.length() + 1;
		this->parseObjectChunk(objectdata, chunkInfo.dataPosition + _loc3_, chunkInfo.dataSize - _loc3_);
	}


	void parseObjectChunk(ObjectData *objectData, int param2, int param3)
	{
		if (param3 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param2);

		switch (chunkinfo.id)
		{
			case CHUNK_TRIMESH:
				this->parseMeshChunk(objectData, chunkinfo.dataPosition, chunkinfo.dataSize);
				break;
			case 17920:
				break;
			case 18176:
				break;
		}
		this->parseObjectChunk(objectData, chunkinfo.nextChunkPosition, param3 - chunkinfo.size);
	}


	void parseMeshChunk(ObjectData *objectData, int param2, int param3)
	{
		if (param3 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param2);

		switch(chunkinfo.id)
		{
		case CHUNK_VERTICES:
			this->parseVertices(objectData);
			break;
		case CHUNK_MAPPINGCOORDS:
			this->parseUVs(objectData);
			break;
		case CHUNK_TRANSFORMATION:
			this->parseMatrix(objectData);
			break;
		case CHUNK_FACES:
			this->parseFaces(objectData, chunkinfo);
		}

		this->parseMeshChunk(objectData, chunkinfo.nextChunkPosition, param3 - chunkinfo.size);
	}


	void parseVertices(ObjectData *objectData)
	{
		int count = this->data.readUnsignedShort();

		objectData->vertices.clear();
		objectData->vertices_valid = true;
		objectData->vertices.reserve(count * 3);

		int i = 0;
		while (i < count) {
			objectData->vertices.append(this->data.readFloat());
			objectData->vertices.append(this->data.readFloat());
			objectData->vertices.append(this->data.readFloat());
			i++;
		}
	}


	void parseUVs(ObjectData *objectData)
	{
		int count = this->data.readUnsignedShort();

		objectData->uvs.clear();
		objectData->uvs_valid = true;
		objectData->uvs.reserve(count * 2);

		int i = 0;
		while (i < count) {
			objectData->uvs.append(this->data.readFloat());
			objectData->uvs.append(this->data.readFloat());
			i++;
		}
	}


	void parseMatrix(ObjectData *objectData)
	{
		objectData->a = this->data.readFloat();
		objectData->e = this->data.readFloat();
		objectData->i = this->data.readFloat();
		objectData->b = this->data.readFloat();
		objectData->f = this->data.readFloat();
		objectData->j = this->data.readFloat();
		objectData->c = this->data.readFloat();
		objectData->g = this->data.readFloat();
		objectData->k = this->data.readFloat();
		objectData->d = this->data.readFloat();
		objectData->h = this->data.readFloat();
		objectData->l = this->data.readFloat();
	}


	void parseFaces(ObjectData *objectData, const ChunkInfo &chunkInfo)
	{
		int count = this->data.readUnsignedShort();

		objectData->faces.clear();
		objectData->faces_valid = true;
		objectData->faces.reserve(count * 3);

		objectData->smoothingGroups.clear();
		objectData->smoothingGroups_valid = true;
		objectData->smoothingGroups.resize(count);
		objectData->smoothingGroups.fill(0);

		int i = 0;
		while (i < count)
		{
			objectData->faces.append(this->data.readUnsignedShort());
			objectData->faces.append(this->data.readUnsignedShort());
			objectData->faces.append(this->data.readUnsignedShort());
			this->data.setPosition(this->data.getPosition() + 2);
			i++;
		}

		int _loc6_ = 2 + (8 * count);

		this->parseFacesChunk(objectData, chunkInfo.dataPosition + _loc6_, chunkInfo.dataSize - _loc6_);
	}


	void parseFacesChunk(ObjectData *objectData, int param2, int param3)
	{
		if (param3 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param2);
		switch(chunkinfo.id)
		{
		case CHUNK_FACESMATERIAL:
			this->parseSurface(objectData);
			break;
		case CHUNK_FACESSMOOTH:
			this->parseSmoothingGroups(objectData);
		}

		this->parseFacesChunk(objectData, chunkinfo.nextChunkPosition, param3 - chunkinfo.size);
	}


	void parseSurface(ObjectData *objectData)
	{
		objectData->surfaces_valid = true;
		objectData->surfaces.clear();

		QVector<int> _loc2_;

		std::string str = this->getString(this->data.getPosition());

		int _loc3_ = this->data.readUnsignedShort();

		_loc2_.reserve(_loc3_);

		int _loc4_ = 0;
		while (_loc4_ < _loc3_)
		{
			_loc2_.append(this->data.readUnsignedShort());
			_loc4_++;
		}

		objectData->surfaces.insert(str, _loc2_);
	}


	void parseSmoothingGroups(ObjectData *objectData)
	{
		int count = objectData->faces.count() / 3;

		if (objectData->smoothingGroups.count() < count)
		{
			qDebug() << "error Parser3DS::parseSmoothingGroups";
		}

		for (int i = 0; i < count; i++)
		{
			objectData->smoothingGroups.replace(i, this->data.readUnsignedInt());
		}
	}


	void parseAnimationChunk(int param1, int param2)
	{
		while (param2 >= 6)
		{
			const ChunkInfo chunkinfo = this->readChunkInfo(param1);
			switch(chunkinfo.id)
			{
			case 45057:
			case 45058:
			case 45059:
			case 751 * 60:
			case 45061:
			case 45062:
			case 45063:
			{
				this->animationDatas_valid = true;
				auto animationData = std::make_shared<AnimationData>();
				this->animationDatas.append(animationData);
				this->parseObjectAnimationChunk(animationData.get(), chunkinfo.dataPosition, chunkinfo.dataSize);
				break;
			}
			case 45064:
				break;
			}
			param1 = chunkinfo.nextChunkPosition;
			param2 = param2 - chunkinfo.size;
		}
	}


	void parseObjectAnimationChunk(AnimationData *animationData, int param2, int param3)
	{
		if (param3 < 6)
			return;

		const ChunkInfo chunkinfo = this->readChunkInfo(param2);
		switch(chunkinfo.id)
		{
			case 0xb010:
				animationData->objectName = this->getString(this->data.getPosition());
				this->data.setPosition(this->data.getPosition() + 4);
				animationData->parentIndex = this->data.readUnsignedShort();
				break;
			case 45073:
				animationData->objectName = this->getString(this->data.getPosition());
				break;
			case 45075:
			{
				float x = this->data.readFloat();
				float y = this->data.readFloat();
				float z = this->data.readFloat();
				animationData->pivot = Vector3D(x, y, z);
				animationData->pivot_valid = true;
				break;
			}
			case 0xb020:
			{
				this->data.setPosition(this->data.getPosition() + 20);
				float x = this->data.readFloat();
				float y = this->data.readFloat();
				float z = this->data.readFloat();
				animationData->position = Vector3D(x, y, z);
				animationData->position_valid = true;
				break;
			}
			case 45089:
			{
				this->data.setPosition(this->data.getPosition() + 20);
				float x = this->data.readFloat();
				float y = this->data.readFloat();
				float z = this->data.readFloat();
				float w = this->data.readFloat();
				animationData->rotation = this->getRotationFrom3DSAngleAxis(x, y, z, w);
				animationData->rotation_valid = true;
				break;
			}
			case 45090:
			{
				this->data.setPosition(this->data.getPosition() + 20);
				float x = this->data.readFloat();
				float y = this->data.readFloat();
				float z = this->data.readFloat();
				animationData->scale = Vector3D(x, y, z);
				animationData->scale_valid = true;
				break;
			}
		}
		this->parseObjectAnimationChunk(animationData, chunkinfo.nextChunkPosition, param3 - chunkinfo.size);
	}


	Vector3D getRotationFrom3DSAngleAxis(double param1, double param2, double param3, double param4)
	{
		Vector3D _loc5_;
		double _loc6_ = MyMath::sin(param1);
		double _loc7_ = MyMath::cos(param1);
		double _loc8_ = 1 - _loc7_;
		double _loc9_ = param2 * param4 * _loc8_ + param3 * _loc6_;
		if (_loc9_ >= 1)
		{
			double _loc10_ = param1 / 2.;
			_loc5_.setZ(-2 * MyMath::atan2(param2 * MyMath::sin(_loc10_), MyMath::cos(_loc10_)));
			_loc5_.setY(-MyMath::PI / 2.);
			_loc5_.setX(0);
			return _loc5_;
		}
		if (_loc9_ <= -1)
		{
			double _loc10_ = param1 / 2.;
			_loc5_.setZ(2 * MyMath::atan2(param2 * MyMath::sin(_loc10_), MyMath::cos(_loc10_)));
			_loc5_.setY(MyMath::PI / 2.);
			_loc5_.setX(0);
			return _loc5_;
		}

		_loc5_.setX(-MyMath::atan2((param4 * _loc6_) - (param2 * param3 * _loc8_), 1 - (((param4 * param4) + (param3 * param3)) * _loc8_)));
		_loc5_.setY(-MyMath::asin((param2 * param4 * _loc8_) + (param3 * _loc6_)));
		_loc5_.setZ(-MyMath::atan2((param2 * _loc6_) - (param4 * param3 * _loc8_), 1 - (((param2 * param2) + (param3 * param3)) * _loc8_)));

		return _loc5_;
	}
};





Parser3DSResult Parser3DS(const QString &name,
						  const QByteArray &data,
						  const QString &texturesBaseURL,
						  double scale)
{

	Parser3DSResult result;

	Parser3DSPrivate p;

	p.setName(name);

	if (p.parse(data, texturesBaseURL, scale))
	{
		result.objects = p.objects;
		result.parents = p.parents;
		result.materials = p.materials;
		result.textureMaterials = p.textureMaterials;
		result.ok = true;
		return result;
	}

	result.ok = false;
	return result;
}



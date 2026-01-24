#include "MapBuilder.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "CollisionGeometryParser.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "hardware/alternativa/engine3d/containers/KDContainer.h"
#include "MapGeometryParser.h"
#include "MapTexturesBuilder.h"
#include "HelperMesh.h"
#include "hardware/alternativa/engine3d/containers/KDContainer.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "hardware/alternativa/engine3d/objects/BSP.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include "hardware/alternativa/engine3d/materials/Material.h"
#include "game/alternativa/tanks/utils/objectpool/ObjectPool.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "game/alternativa/math/Vector3.h"
#include "game/alternativa/tanks/sfx/StaticObject3DPositionProvider.h"
#include "game/alternativa/tanks/sfx/christmas/ChristmasTreeToyEffect.h"
#include "hardware/alternativa/engine3d/objects/Sprite3D.h"
#include "Error.h"
#include <QDomDocument>
#include <QDebug>


static const double KDTREE_THRESHOLD = 0.1;
static const int TEXTURE_BUILDER_BATCH_SIZE = 20;

extern BattleService *battleService;


MapBuilder::MapBuilder(TextureMaterialRegistry *materialRegistry,
					   PropLibRegistry *propLibRegistry)
{
	this->materialRegistry = materialRegistry;
	this->propLibRegistry = propLibRegistry;
	this->mapGeometryParser = nullptr;
}


void MapBuilder::initPhysicsGeometry(const QDomElement &xml)
{
	MyList<CollisionShape*> *list = CollisionGeometryParser::parse(xml);
	battleService->getBattleRunner()->initStaticGeometry(list);
	//this->addMapValidators(list);
}


void MapBuilder::build(const QByteArray &data)
{
	QDomDocument doc;

	doc.setContent(data);

	if (!doc.isDocument())
	{
		throw Error("MapBuilder::build");
	}

	QDomElement xml = doc.documentElement();

	//this->buildCompleteCallback = param2;
	this->initPhysicsGeometry(xml);
	this->parseMapGeometry(xml);
	this->buildTextures();
}


std::shared_ptr<KDContainer> MapBuilder::getMapContainer()
{
	return this->mapContainer;
}


void MapBuilder::createMap()
{
	this->mapContainer = new_KDContainer();
	this->mapContainer->threshold = KDTREE_THRESHOLD;
	this->mapContainer->ignoreChildrenInCollider = true;

	QVector<std::shared_ptr<Object3D>> vector = this->mapGeometryParser->getObjects();
	vector.append(new_HelperMesh());


	QVector<std::shared_ptr<Occluder>> occluder_list = this->mapGeometryParser->getOccluders();

	this->mapContainer->createTree(vector, &occluder_list);
	BattleScene3D *battleScene3D = battleService->getBattleScene3D();


	for (std::shared_ptr<Object3D> object : this->mapGeometryParser->getSprites())
	{
		//this->mapContainer->addChild(object);
		//battleScene3D->hidableGraphicObjects->add(new HidableObject3DWrapper(object));
	}


   this->mapContainer->calculateBounds();
   battleScene3D->setMapContainer(this->mapContainer);

}


void MapBuilder::parseMapGeometry(const QDomElement &xml)
{
   this->mapGeometryParser = new MapGeometryParser(this->propLibRegistry);
   this->mapGeometryParser->parse(xml);
   this->mapBounds = this->mapGeometryParser->getMapBounds();
}


void MapBuilder::buildTextures()
{
	this->texturesBuilder = new MapTexturesBuilder(this->materialRegistry, TEXTURE_BUILDER_BATCH_SIZE);
	//this->texturesBuilder->addEventListener(Event.COMPLETE, this->onTexturesReady);
	this->texturesBuilder->run(this->mapGeometryParser->getTexturedPropsCollections());
	this->onTexturesReady();
}


void MapBuilder::onTexturesReady()
{
	//var _local_2:int;
	//this.texturesReady = true;
	//this.texturesBuilder.removeEventListener(Event.COMPLETE, this.onTexturesReady);
	//this.completeTimeoutId = setTimeout(this.complete, _local_2);
	this->complete();
}


void MapBuilder::complete()
{
	//this->isComplete = true;
	//clearTimeout(this.completeTimeoutId);
	this->textures = this->texturesBuilder->getTextures();
	this->setupChristmasTree();
	this->createMap();
	this->setupBillboards();
	this->setupChristmasTreeToys();
	this->mapGeometryParser->clear();
	this->mapGeometryParser = nullptr;
	this->texturesBuilder = nullptr;
	this->materialRegistry = nullptr;
	this->propLibRegistry = nullptr;
	//this->buildCompleteCallback();
}


void MapBuilder::setupBillboards()
{
	QVector<std::shared_ptr<Mesh>> list = this->mapGeometryParser->getBillboards();

	BattleScene3D *battleScene3D = battleService->getBattleScene3D();

	for (std::shared_ptr<Mesh> &mesh : list)
	{
		battleScene3D->addBillboard(mesh);
	}
}


void MapBuilder::setupChristmasTree()
{
	if (this->mapGeometryParser->getChristmasTree() == nullptr)
	{
		return;
	}

	std::shared_ptr<Object3D> v = this->mapGeometryParser->getChristmasTree();
	std::shared_ptr<BSP> bsp = std::dynamic_pointer_cast<BSP>(v);
	bsp->faces[0]->material->alphaTestThreshold = 0.5;
}


void MapBuilder::setupChristmasTreeToys()
{
	if (this->mapGeometryParser->getChristmasTree() == nullptr)
	{
		return;
	}

	QVector<std::shared_ptr<Object3D>> _local_3 = this->mapGeometryParser->getChristmasTreeToys();
	int _local_4 = _local_3.length();

	ObjectPool *_local_5 = battleService->getObjectPool();
	Vector3 _local_6;
	_local_6.reset();

	int _local_1 = 0;
	while (_local_1 < _local_4)
	{
		std::shared_ptr<Object3D> _local_2 = _local_3[_local_1];
		_local_6.x = (_local_6.x + _local_2->x);
		_local_6.y = (_local_6.y + _local_2->y);
		_local_6.z = (_local_6.z + _local_2->z);
		_local_1++;
	}

	_local_6.x = (_local_6.x / _local_4);
	_local_6.y = (_local_6.y / _local_4);
	_local_6.z = (_local_6.z / _local_4);

	_local_1 = 0;

	while (_local_1 < _local_4)
	{
		std::shared_ptr<Object3D> _local_2 = _local_3[_local_1];

		Vector3 _local_7;
		_local_7.x = _local_2->x;
		_local_7.y = _local_2->y;
		_local_7.z = _local_2->z;
		StaticObject3DPositionProvider *_local_8 = _local_5->getObject<StaticObject3DPositionProvider>();
		_local_8->init(&_local_7, 150);
		ChristmasTreeToyEffect *_local_9 = _local_5->getObject<ChristmasTreeToyEffect>();

		_local_9->init(std::dynamic_pointer_cast<Sprite3D>(_local_2), _local_8, &_local_6);

		battleService->getBattleScene3D()->addGraphicEffect(_local_9);
		_local_1++;
	}
}

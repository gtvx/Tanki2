#include "_init.h"
#include "game/alternativa/tanks/models/tank/TankModel.h"
#include "game/alternativa/tanks/models/tank/ITankModelAdapt.h"
#include "game/alternativa/tanks/models/tank/configuration/TankConfiguration.h"
#include "entrance/alternativa/types/Long.h"
#include "game/projects/tanks/client/battlefield/models/user/speedcharacteristics/SpeedCharacteristicsCC.h"
#include "game/alternativa/tanks/models/tank/speedcharacteristics/SpeedCharacteristicsModel.h"
#include "game/alternativa/tanks/models/tank/speedcharacteristics/SpeedCharacteristicsAdapt.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/LocalChassisController.h"
#include "game/alternativa/tanks/models/tank/hullcommon/HullCommon.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/TankCC.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/TankLogicState.h"
#include "entrance/projects/tanks/client/battleservice/model/battle/team/BattleTeam.h"
#include "game/projects/tanks/clients/flash/resources/resource/Tanks3DSResource.h"
#include "game/projects/tanks/clients/flash/resources/object3ds/IObject3DS.h"
#include "game/projects/tanks/client/battlefield/types/Vector3d.h"
#include "game/projects/tanks/client/battlefield/types/TankState.h"
#include "game/alternativa/tanks/models/weapon/turret/RotatingTurretModel.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/models/weapon/AllGlobalGunParams.h"
#include "game/alternativa/tanks/models/weapon/WeaponConst.h"
#include "game/alternativa/tanks/utils/EncryptedNumber.h"
#include "game/projects/tanks/client/battlefield/models/user/tank/commands/TurretStateCommand.h"
#include "MyMath.h"
#include "path.h"
#include "game/alternativa/physics/PhysicsUtils.h"
#include "game/alternativa/math/Matrix3.h"
#include "game/alternativa/physics/Body.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "game/alternativa/tanks/battle/BattleRunner.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/physics/CollisionGroup.h"
#include "game/alternativa/physics/PhysicsMaterial.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/tanks/battle/BattleUtils.h"
#include "global_class.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/autoaiming/VerticalAutoAiming.h"
#include "game/alternativa/tanks/models/weapon/IWeaponModel.h"
#include "game/alternativa/tanks/models/weapon/WeaponObject.h"
#include "game/alternativa/tanks/models/weapon/WeaponForces.h"
#include "game/alternativa/tanks/models/weapon/smoky/SmokyWeapon.h"
#include "game/alternativa/tanks/models/weapons/targeting/CommonTargetingSystem.h"
#include "game/alternativa/tanks/models/weapon/shared/SimpleWeaponController.h"
#include "game/alternativa/tanks/models/weapon/smoky/RemoteSmokyWeapon.h"
#include "game/alternativa/tanks/models/weapon/weakening/DistanceWeakening.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/common/WeaponCommonCC.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModel.h"
#include "game/alternativa/tanks/models/weapon/common/WeaponCommonData.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModelAdapt.h"
#include "game/alternativa/tanks/models/weapon/shared/shot/IShotModel.h"
#include "game/alternativa/tanks/models/weapon/shared/shot/IShotModelAdapt.h"
#include "file.h"
#include "flash/display/BitmapData.h"
#include <QDebug>
#include <QImage>
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "entrance/platform/client/fp10/core/type/impl/GameObject.h"
#include "game/alternativa/tanks/models/weapon/IWeaponModelAdapt.h"
#include "game/alternativa/tanks/models/tank/WeaponMountProviderAdapt.h"
#include "game/alternativa/tanks/models/weapon/weakening/IWeaponWeakeningModel.h"
#include "game/alternativa/tanks/models/weapon/weakening/WeaponWeakeningModel.h"
#include "game/alternativa/tanks/models/weapon/weakening/IWeaponWeakeningModelAdapt.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/weakening/WeaponWeakeningCC.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/twins/TwinsCC.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/sfx/shoot/twins/TwinsShootSFXCC.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsSFXModel.h"
#include "game/alternativa/tanks/models/weapon/twins/ITwinsSFXModelAdapt.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsWeaponCallbackAdapt.h"
#include "game/alternativa/tanks/models/weapon/twins/TwinsEffects.h"
#include "entrance/platform/client/fp10/core/resource/types/TextureResource.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/autoaiming/VerticalAutoAimingModel.h"
#include "entrance/platform/client/fp10/core/resource/types/MultiframeTextureResource.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/verticalangles/WeaponVerticalAnglesModel.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/verticalangles/VerticalAnglesAdapt.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/angles/verticals/verticalangles/VerticalAnglesCC.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/autoaiming/VerticalAutoAimingAdapt.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/shot/ShotCC.h"
#include "game/alternativa/tanks/models/weapon/shared/shot/DiscreteShotModel.h"
#include "entrance/platform/client/fp10/core/type/impl/Space.h"
#include "game/alternativa/tanks/models/tank/configuration/TankConfigurationModel.h"
#include "game/alternativa/tanks/models/tank/configuration/TankConfigurationAdapt.h"
#include "game/projects/tanks/client/battlefield/models/user/configuration/TankConfigurationModelBase.h"
#include "game/projects/tanks/client/battlefield/models/user/configuration/TankConfigurationCC.h"
#include "game/alternativa/tanks/models/tank/hullcommon/HullCommonAdapt.h"
#include "game/alternativa/tanks/models/tank/hullcommon/HullCommonModel.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/armor/common/HullCommonCC.h"
#include "game/alternativa/tanks/models/weapon/angles/verticals/verticalangles/VerticalAngles.h"
#include "game/alternativa/tanks/model/item3d/Object3DSModel.h"
#include "game/projects/tanks/clients/flash/resources/object3ds/IObject3DSAdapt.h"
#include "game/projects/tanks/client/garage/models/item/object3ds/Object3DSCC.h"
#include "entrance/platform/client/fp10/core/registry/ResourceRegistry.h"
#include "game/alternativa/tanks/models/weapon/common/WeaponCommonModel.h"
#include "game/alternativa/tanks/models/weapon/common/IWeaponCommonModelAdapt.h"
#include "game/projects/tanks/client/battlefield/models/tankparts/weapon/turret/RotatingTurretCC.h"
#include "game/alternativa/tanks/models/weapon/turret/RotatingTurretModel.h"
#include "game/alternativa/tanks/models/weapon/turret/IRotatingTurretModelAdapt.h"
#include "game/alternativa/tanks/models/tank/InitTankPartAdapt.h"



std::shared_ptr<ITankModel> Tank_getAdapt(IGameObject *object,
										  std::shared_ptr<TankModel> model)
{
	return std::make_shared<ITankModelAdapt>(object, model);
}

std::shared_ptr<SpeedCharacteristics> SpeedCharacteristics_getAdapt(IGameObject *object,
										  std::shared_ptr<SpeedCharacteristicsModel> model)
{
	return std::make_shared<SpeedCharacteristicsAdapt>(object, model);
}


std::shared_ptr<WeaponMountProvider> WeaponMountProvider_getAdapt(IGameObject *object,
										  std::shared_ptr<WeaponMountProvider> model)
{
	return std::make_shared<WeaponMountProviderAdapt>(object, model);
}


std::shared_ptr<IWeaponCommonModel> IWeaponCommonModel_getAdapt(IGameObject *object,
										  std::shared_ptr<IWeaponCommonModel> model)
{
	return std::make_shared<IWeaponCommonModelAdapt>(object, model);
}


std::shared_ptr<IWeaponWeakeningModel> IWeaponWeakeningModel_getAdapt(IGameObject *object,
										  std::shared_ptr<IWeaponWeakeningModel> model)
{
	return std::make_shared<IWeaponWeakeningModelAdapt>(object, model);
}

std::shared_ptr<IShotModel> IShotModel_getAdapt(IGameObject *object,
										  std::shared_ptr<IShotModel> model)
{
	return std::make_shared<IShotModelAdapt>(object, model);
}


std::shared_ptr<TwinsWeaponCallback> TwinsWeaponCallback_getAdapt(IGameObject *object,
																  std::shared_ptr<TwinsWeaponCallback> model)
{
	return std::make_shared<TwinsWeaponCallbackAdapt>(object, model);
}

std::shared_ptr<ITwinsSFXModel> ITwinsSFXModel_getAdapt(IGameObject *object,
														std::shared_ptr<ITwinsSFXModel> model)
{
	return std::make_shared<ITwinsSFXModelAdapt>(object, model);
}




std::shared_ptr<VerticalAngles> VerticalAngles_getAdapt(IGameObject *object,
														std::shared_ptr<VerticalAngles> model)
{
	return std::make_shared<VerticalAnglesAdapt>(object, model);
}

std::shared_ptr<VerticalAutoAiming> VerticalAutoAiming_getAdapt(IGameObject *object,
														std::shared_ptr<VerticalAutoAiming> model)
{
	return std::make_shared<VerticalAutoAimingAdapt>(object, model);
}



std::shared_ptr<TankConfiguration> TankConfiguration_getAdapt(IGameObject *object,
															  std::shared_ptr<TankConfiguration> model)
{
	return std::make_shared<TankConfigurationAdapt>(object, model);
}



std::shared_ptr<HullCommon> HullCommon_getAdapt(IGameObject *object,
													std::shared_ptr<HullCommon> model)
{
	return std::make_shared<HullCommonAdapt>(object, model);
}


std::shared_ptr<IObject3DS> IObject3DS_getAdapt(IGameObject *object,
													std::shared_ptr<IObject3DS> model)
{
	return std::make_shared<IObject3DSAdapt>(object, model);
}





std::shared_ptr<IWeaponModel> IWeaponModel_getAdapt(IGameObject *object,
													std::shared_ptr<IWeaponModel> model)
{
	return std::make_shared<IWeaponModelAdapt>(object, model);
}

std::shared_ptr<IRotatingTurretModel> IRotatingTurretModel_getAdapt(IGameObject *object,
													std::shared_ptr<IRotatingTurretModel> model)
{
	return std::make_shared<IRotatingTurretModelAdapt>(object, model);
}


std::shared_ptr<InitTankPart> InitTankPart_getAdapt(IGameObject *object,
													std::shared_ptr<InitTankPart> model)
{
	return std::make_shared<InitTankPartAdapt>(object, model);
}






std::shared_ptr<BitmapData> getDetails(const QByteArray &details, const QByteArray &alpha)
{
	QImage image_details;
	image_details.loadFromData(details);

	QImage image_details_alpha;
	image_details_alpha.loadFromData(alpha);

	auto result_details = std::make_shared<BitmapData>(image_details.width(), image_details.height(), true);

	QImage *result = result_details->qimage();

	uint8_t *byte_result = (uint8_t*)result->bits();
	uint8_t *byte_details = (uint8_t*)image_details.bits();
	uint8_t *byte_alpha = (uint8_t*)image_details_alpha.bits();

	int depth_result = result->depth() / 8;
	int depth_details = image_details.depth() / 8;
	int depth_alpha = image_details_alpha.depth() / 8;

	int count = result->sizeInBytes() / (result->depth() / 8);

	for (int i = 0; i < count; i++)
	{
		uint8_t alpha = byte_alpha[0];

		byte_result[0] = (byte_details[0] * alpha) / 255;
		byte_result[1] = (byte_details[1] * alpha) / 255;
		byte_result[2] = (byte_details[2] * alpha) / 255;

		byte_result[3] = alpha;

		byte_result += depth_result;
		byte_details += depth_details;
		byte_alpha += depth_alpha;
	}

	return result_details;

}

static const QString DETAILS = "details.png";
static const QString LIGHTMAP = "lightmap.jpg";






template <typename T>
void Object_putAdapt(IGameObject *object, std::shared_ptr<T> p)
{
	GameObject *o = (GameObject*)object;
	o->putAdapt<T>(p);
}




class TankManager
{
	ISpace *space;

	std::shared_ptr<WeaponWeakeningModel> _WeaponWeakeningModel;
	std::shared_ptr<WeaponCommonModel> common_model;
	std::shared_ptr<TwinsSFXModel> _TwinsSFXModel;
	std::shared_ptr<WeaponVerticalAnglesModel> _WeaponVerticalAnglesModel;
	std::shared_ptr<HullCommonModel> _HullCommonModel;
	std::shared_ptr<TwinsModel> twinsModel;
	std::shared_ptr<DiscreteShotModel> _DiscreteShotModel;
	std::shared_ptr<SpeedCharacteristicsModel> _SpeedCharacteristicsModel;
	std::shared_ptr<VerticalAutoAimingModel> _VerticalAutoAimingModel;
	std::shared_ptr<Object3DSModel> _Object3DSModel;
	std::shared_ptr<TankModel> tankmodel;

	ResourceRegistry *resourceRegistry;

	IGameObject *weaponObject;
	IGameObject *HullObject;




public:
	TankManager(ResourceRegistry *resourceRegistry)
	{
		this->resourceRegistry = resourceRegistry;

		space = new Space(Long(10, 0), nullptr, nullptr, false);

		_TwinsSFXModel = std::make_shared<TwinsSFXModel>();
		common_model = std::make_shared<WeaponCommonModel>();
		_WeaponVerticalAnglesModel = std::make_shared<WeaponVerticalAnglesModel>();
		_HullCommonModel = std::make_shared<HullCommonModel>();
		twinsModel = std::make_shared<TwinsModel>();
		_DiscreteShotModel = std::make_shared<DiscreteShotModel>();
		_SpeedCharacteristicsModel = std::make_shared<SpeedCharacteristicsModel>();
		_VerticalAutoAimingModel = std::make_shared<VerticalAutoAimingModel>();
		_WeaponWeakeningModel = std::make_shared<WeaponWeakeningModel>();
		_Object3DSModel = std::make_shared<Object3DSModel>();
		tankmodel = std::make_shared<TankModel>();
		BattleRunner::localTankInfoService = tankmodel.get();


		init_weaponObject_1();
		init_hullObject_1();

		initTwinsModel();
	}

private:
	std::shared_ptr<IObject3DS> LoadModel(ISpace *space, const QString &dir, const Long &id, const Long &version)
	{
		ResourceInfo *resourceInfo = new ResourceInfo(0, id, version, false);


		Tanks3DSResource *resurce = new Tanks3DSResource(resourceInfo);


		Object3DSCC *cc = new Object3DSCC(id);


		IGameObject *object  = space->createObject(id, nullptr, "IObject3DS");


		std::shared_ptr<IObject3DS> adapt = IObject3DS_getAdapt(object, _Object3DSModel);



		QString filename_3ds = dir + "object.3ds";
		QString filename_details = dir + "details.jpg";
		QString filename_details_alpha = dir + "details_alpha.jpg";
		QString filename_lightmap = dir + "lightmap.jpg";


		QByteArray data = File::read(filename_3ds);

		if (data.isEmpty())
			return nullptr;

		if (!resurce->buildResourceData(data, filename_3ds))
			return nullptr;


		QByteArray details = File::read(filename_details);
		QByteArray details_alpha = File::read(filename_details_alpha);

		resurce->textures[DETAILS] = getDetails(details, details_alpha);

		std::shared_ptr<BitmapData> bitmap_lightmap = std::make_shared<BitmapData>();
		bitmap_lightmap->loadFromData(File::read(filename_lightmap));
		resurce->textures[LIGHTMAP] = bitmap_lightmap;



		Model::set_object(object);
		_Object3DSModel->putInitParams(std::shared_ptr<Object3DSCC>(cc));
		Model::popObject();


		resourceRegistry->registerResource(resurce);

		return adapt;
	}



	void init_weaponObject_1()
	{
		Long weapon_id(1, 301); //0x12D
		std::shared_ptr<IObject3DS> Object3DS_Weapon = LoadModel(space, path::getResources() + "/weapon/twins/00000000 004C5BA7 0000016B 9DB7775A/", Long(0, 2), Long(0, 1));
		weaponObject = space->createObject(weapon_id, nullptr, "Object3DS_Weapon");
		Object_putAdapt<IObject3DS>(weaponObject, Object3DS_Weapon);
	}

	void init_hullObject_1()
	{
		Long hull_id(1, 302); //0x12E
		std::shared_ptr<IObject3DS> Object3DS_Hull = LoadModel(space, path::getResources() + "/hull/mamont/00000000 004C5EF9 0000016B 9DD8BE7B/", Long(0, 1), Long(0, 1));
		HullObject = space->createObject(hull_id, nullptr, "Object3DS_Hull");
		Object_putAdapt<IObject3DS>(HullObject, Object3DS_Hull);
	}


	void initTwinsModel()
	{

		ResourceInfo *resourceInfo = new ResourceInfo(10, Long(12, 1), Long(1, 1), false);
		MultiframeTextureResource *twins_MultiframeTextureResource = new MultiframeTextureResource(resourceInfo);
		QByteArray data = File::read(path::getResources() + "/MULTIFRAME_IMAGE/00000166 CA8B7072 0000016B 9EBED9A0/image.tara");
		twins_MultiframeTextureResource->buildFrames(data);


		std::shared_ptr<ITwinsSFXModel> _TwinsSFXModelAdapt;

		{
			IGameObject *object = space->createObject(Long(1, 15), nullptr, "TwinsSFXModel");

			_TwinsSFXModelAdapt = ITwinsSFXModel_getAdapt(object, _TwinsSFXModel);


			ResourceInfo *resourceInfo_2 = new ResourceInfo(10, Long(12, 402), Long(1, 1), false);
			ResourceInfo *resourceInfo_3 = new ResourceInfo(10, Long(12, 403), Long(1, 1), false);


			TextureResource *res_2 = new TextureResource(resourceInfo_2);

			res_2->data = std::make_shared<BitmapData>();
			res_2->data->loadFromData(File::read(path::getResources() + "/decal/plasma.png"));

			TextureResource *res_3 = new TextureResource(resourceInfo_3);
			res_3->data = std::make_shared<BitmapData>(100, 100, false, 0xFFFFFF);



			TwinsShootSFXCC *cc = new TwinsShootSFXCC;

			cc->explosionTexture = twins_MultiframeTextureResource;
			cc->hitMarkTexture = res_2;
			cc->lightingSFXEntity = nullptr;
			cc->muzzleFlashTexture = res_3;
			cc->shotSound = nullptr;
			cc->shotTexture = twins_MultiframeTextureResource;


			Model::set_object(object);
			_TwinsSFXModel->putInitParams(std::shared_ptr<TwinsShootSFXCC>(cc));
			_TwinsSFXModel->objectLoadedPost();
			Model::popObject();
		}

		std::shared_ptr<IWeaponCommonModel> IWeaponCommonModel_adapt;

		{
			IGameObject *object_common  = space->createObject(Long(1, 4), nullptr, "WeaponCommonModel");

			IWeaponCommonModel_adapt = IWeaponCommonModel_getAdapt(object_common, common_model);

			{
				WeaponCommonCC *cc = new WeaponCommonCC;

				cc->buffShotCooldownMs = 700;
				cc->buffed = false;
				cc->highlightingDistance = 56.705921173095703125;
				cc->impactForce = 1.3352941274642944336;
				cc->kickback = 1.101764678955078125;
				cc->turretRotationAcceleration = 1.8058989048004150391;
				cc->turretRotationSpeed = 1.8058989048004150391;

				Model::set_object(object_common);
				common_model->putInitParams(std::shared_ptr<WeaponCommonCC>(cc));
				common_model->objectLoaded();
				Model::popObject();
			}

			{

				std::shared_ptr<RotatingTurretModel> rotatingTurretModel = std::make_shared<RotatingTurretModel>();

				IGameObject *object  = space->createObject(Long(1, 36), nullptr, "RotatingTurretModel"); //0x24

				Object_putAdapt<IWeaponCommonModel>(object, IWeaponCommonModel_adapt);


				std::shared_ptr<IRotatingTurretModel> rotatingTurretModelAdapt = IRotatingTurretModel_getAdapt(object, rotatingTurretModel);

				std::shared_ptr<RotatingTurretCC> rotatingTurretCC = std::make_shared<RotatingTurretCC>();
				rotatingTurretCC->turretState =  std::make_shared<TurretStateCommand>();


				Model::set_object(object);
				rotatingTurretModel->putInitParams(rotatingTurretCC);
				rotatingTurretModel->objectLoaded();
				Model::popObject();

				Object_putAdapt<IRotatingTurretModel>(weaponObject, rotatingTurretModelAdapt);


				std::shared_ptr<InitTankPart> _InitTankPartAdapt = InitTankPart_getAdapt(object, rotatingTurretModel);


				Object_putAdapt<InitTankPart>(weaponObject, _InitTankPartAdapt);

				std::shared_ptr<WeaponMountProvider> WeaponMountProviderAdapt = WeaponMountProvider_getAdapt(object, rotatingTurretModel);

				Object_putAdapt<WeaponMountProvider>(weaponObject, WeaponMountProviderAdapt);
			}

		}


		std::shared_ptr<IWeaponWeakeningModel> _IWeaponWeakeningModelAdapt;


		{
			IGameObject *o = space->createObject(Long(1, 14), nullptr, "WeaponWeakeningModel");


			auto cc = new WeaponWeakeningCC(41.82, 25, 82.3);

			Model::set_object(o);
			_WeaponWeakeningModel->putInitParams(std::shared_ptr<WeaponWeakeningCC>(cc));
			_WeaponWeakeningModel->objectLoaded();
			Model::popObject();

			Object_putAdapt<IWeaponWeakeningModel>(weaponObject, _WeaponWeakeningModel);

			_IWeaponWeakeningModelAdapt = IWeaponWeakeningModel_getAdapt(o, _WeaponWeakeningModel);
		}



		std::shared_ptr<VerticalAngles> verticalAnglesAdapt;

		{
			std::shared_ptr<VerticalAnglesCC> cc = std::make_shared<VerticalAnglesCC>(0.244346, 0.191986);

			IGameObject *object = space->createObject(Long(1, 20), nullptr, "WeaponVerticalAnglesModel");

			verticalAnglesAdapt = VerticalAngles_getAdapt(object, _WeaponVerticalAnglesModel);

			Model::set_object(object);
			_WeaponVerticalAnglesModel->putInitParams(cc);;
			Model::popObject();

		}

		IGameObject *object = space->createObject(Long(1, 6), nullptr, "TwinsModel");

		Object_putAdapt<IWeaponWeakeningModel>(object, _IWeaponWeakeningModelAdapt);
		Object_putAdapt<IWeaponCommonModel>(object, IWeaponCommonModel_adapt);
		Object_putAdapt<ITwinsSFXModel>(object, _TwinsSFXModelAdapt);


		{
			std::shared_ptr<VerticalAutoAiming> _VerticalAutoAimingModelAdapt = VerticalAutoAiming_getAdapt(object, _VerticalAutoAimingModel);

			Object_putAdapt<VerticalAutoAiming>(object, _VerticalAutoAimingModelAdapt);
		}



		std::shared_ptr<IShotModel> _IShotModelAdapt;

		{
			IGameObject *object = space->createObject(Long(1, 21), nullptr, "DiscreteShotModel");



			_IShotModelAdapt = IShotModel_getAdapt(object, _DiscreteShotModel);

			std::shared_ptr<ShotCC> cc = std::make_shared<ShotCC>(273);

			Model::set_object(object);
			_DiscreteShotModel->putInitParams(cc);
			_DiscreteShotModel->objectLoaded();
			Model::popObject();
		}

		Object_putAdapt<IShotModel>(object, _IShotModelAdapt);

		Object_putAdapt<TwinsWeaponCallback>(object, TwinsWeaponCallback_getAdapt(object, twinsModel));

		Object_putAdapt<VerticalAngles>(object, verticalAnglesAdapt);

		std::shared_ptr<TwinsCC> cc = std::make_shared<TwinsCC>(75.29, 75.29);

		std::shared_ptr<IWeaponModel> _IWeaponModel = IWeaponModel_getAdapt(object, twinsModel);

		Object_putAdapt<IWeaponModel>(weaponObject, _IWeaponModel);

		Model::set_object(object);
		twinsModel->putInitParams(cc);
		twinsModel->objectLoaded();
		Model::popObject();
	}


public:
	Tank* createTank(uint32_t _id, bool local, float health, TankState *tankstate)
	{

		IGameObject *object = space->createObject(Long(1, _id), nullptr, "TankModel");


		std::shared_ptr<ITankModel> _ITankModelAdapt = Tank_getAdapt(object, tankmodel);


		IGameObject *TankConfigurationObject = space->createObject(Long(1, 25), nullptr, "TankConfigurationModel");

		std::shared_ptr<TankConfigurationModel> tankConfigurationModel = std::make_shared<TankConfigurationModel>();

		std::shared_ptr<TankConfiguration> tankConfigurationAdapt = TankConfiguration_getAdapt(TankConfigurationObject, tankConfigurationModel);


		TankConfigurationCC *tankConfigurationCC = new TankConfigurationCC;


		Model::set_object(TankConfigurationObject);
		tankConfigurationModel->putInitParams(std::shared_ptr<TankConfigurationCC>(tankConfigurationCC));
		Model::popObject();




		tankConfigurationCC->weaponId = weaponObject->id();
		tankConfigurationCC->hullId = HullObject->id();






		IGameObject *HullCommonModelObject = space->createObject(Long(1, 24), nullptr, "HullCommonModel");


		std::shared_ptr<HullCommonCC> hullCommonCC = std::make_shared<HullCommonCC>();

		hullCommonCC->mass = 2492.941162109375;


		std::shared_ptr<HullCommon> _HullCommonAdapt = HullCommon_getAdapt(HullCommonModelObject, _HullCommonModel);

		Object_putAdapt<HullCommon>(HullObject, _HullCommonAdapt);


		Model::set_object(HullCommonModelObject);
		_HullCommonModel->putInitParams(std::shared_ptr<HullCommonCC>(hullCommonCC));
		Model::popObject();










		{
			IGameObject *o = space->createObject(Long(1, 11), nullptr, "SpeedCharacteristicsModel");

			Object_putAdapt<ITankModel>(o, _ITankModelAdapt);


			SpeedCharacteristicsCC *cc = new SpeedCharacteristicsCC;

			{
				cc->_baseAcceleration = 8.51764678955078125;
				cc->_baseSpeed = 10.132352828979492188;
				cc->_baseTurnSpeed = 1.7799791097640991211;
				cc->_baseTurretRotationSpeed = 1.8058989048004150391;
				cc->_currentAcceleration = 8.51764678955078125;
				cc->_currentSpeed = 10.132352828979492188;
				cc->_currentTurnSpeed = 1.7799791097640991211;
				cc->_currentTurretRotationSpeed = 1.8058989048004150391;
				cc->_reverseAcceleration = 12.397058486938476563;
				cc->_reverseTurnAcceleration = 4.9793219566345214844;
				cc->_sideAcceleration = 15.59558868408203125;
				cc->_turnAcceleration = 2.2920279502868652344;
				cc->_turnStabilizationAcceleration = 4.9793219566345214844;
			}

			Model::set_object(o);
			_SpeedCharacteristicsModel->putInitParams(std::shared_ptr<SpeedCharacteristicsCC>(cc));
			Model::popObject();


			Object_putAdapt<SpeedCharacteristics>(object, SpeedCharacteristics_getAdapt(o, _SpeedCharacteristicsModel));
		}


		Object_putAdapt<TankConfiguration>(object, tankConfigurationAdapt);

		{
			TankCC *tankcc = new TankCC;
			tankcc->health = health;
			tankcc->local = local;
			tankcc->logicState = TankLogicState::NEW;
			tankcc->movementDistanceBorderUntilTankCorrection = 2000;
			tankcc->movementTimeoutUntilTankCorrection = 4000;
			tankcc->tankState = tankstate;
			tankcc->team = BattleTeam::NONE;

			Object_putAdapt<ITankModel>(object, _ITankModelAdapt);

			Model::set_object(object);
			tankmodel->putInitParams(std::shared_ptr<TankCC>(tankcc));
			tankmodel->objectLoaded(object);
			Model::popObject();
		}



		Tank *tank;

		Model::set_object(object);
		tankmodel->activateTank();
		tank = tankmodel->getTank();
		Model::popObject();



		return tank;
	}

};




_init::_init(ResourceRegistry *resourceRegistry)
{
	tankManager = new TankManager(resourceRegistry);
}

Tank* _init::createTank(uint32_t id, bool local, float health, TankState *tankstate)
{
	return tankManager->createTank(id, local, health, tankstate);
}



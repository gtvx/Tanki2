#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hardware/alternativa/engine3d/loaders/Parser3DS.h"
#include "hardware/alternativa/engine3d/materials/TextureMaterial.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "hardware/alternativa/engine3d/objects/Mesh.h"
#include "hardware/alternativa/engine3d/core/Camera3D.h"
#include "hardware/alternativa/engine3d/core/Face.h"
#include <QFile>
#include <QDebug>
#include <QWindow>
#include "hardware/alternativa/engine3d/core/Vertex.h"
#include "MyMath.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDir>
#include "hardware/alternativa/engine3d/materials/FillMaterial.h"
#include "hardware/alternativa/engine3d/core/Wrapper.h"
#include "game/alternativa/tanks/battle/BattleService.h"
#include "game/alternativa/tanks/battle/objects/tank/WeaponMount.h"
#include "game/alternativa/tanks/battle/objects/tank/controllers/Turret.h"
#include "game/alternativa/tanks/battle/objects/tank/Weapon.h"
#include "game/alternativa/tanks/models/weapon/shared/SimpleWeaponController.h"
#include "game/alternativa/physics/collision/types/RayHit.h"
#include <QMessageBox>
#include "game/alternativa/tanks/models/weapon/weakening/DistanceWeakening.h"
#include "getBattleRunner.h"
#include "game/alternativa/tanks/services/tankregistry/TankUsersRegistry.h"
#include "game/alternativa/tanks/models/tank/LocalTankInfoService.h"
#include "game/alternativa/tanks/battle/objects/tank/Tank.h"
#include "game/alternativa/tanks/physics/TankBody.h"
#include "game/alternativa/tanks/battle/objects/tank/tankskin/TankSkin.h"
#include "_init.h"
#include "game/alternativa/tanks/models/battle/battlefield/BattlefieldModel.h"
#include "game/alternativa/tanks/battle/scene3d/BattleScene3D.h"
#include "game/alternativa/tanks/camera/GameCamera.h"
#include "game/alternativa/tanks/services/battleinput/BattleInputServiceImpl.h"
#include "_global.h"
#include "game/alternativa/physics/collision/primitives/CollisionBox.h"
#include "game/alternativa/physics/collision/primitives/CollisionRect.h"
#include "game/alternativa/physics/collision/primitives/CollisionSphere.h"
#include "game/alternativa/physics/collision/primitives/CollisionTriangle.h"
#include "game/alternativa/physics/Body.h"
#include "global_class.h"
#include "game/alternativa/tanks/physics/TanksCollisionDetector.h"
#include "game/alternativa/physics/collision/CollisionKdTree.h"
#include "game/alternativa/physics/collision/CollisionKdNode.h"
#include "game/alternativa/physics/collision/CollisionKdTree2D.h"
#include "path.h"
#include "file.h"
#include "vulkan/VulkanFunctions.h"
#include "file.h"
#include "hardware/alternativa/engine3d/core/View.h"
#include "vulkan/EVulkanWindow.h"
#include "entrance/platform/client/fp10/core/type/IGameObject.h"
#include "game/alternativa/tanks/models/tank/ITankModel.h"
#include "game/projects/tanks/client/battlefield/types/TankState.h"
#include "vulkan/VK_createInstance.h"
#include "view/vulkan/VulkanWindow2.h"
#include "view/primitive/WidgetPrimitive.h"
#include "game/projects/tanks/clients/flash/resources/resource/PropLibResource.h"
#include "entrance/platform/client/fp10/core/registry/impl/ResourceRegistryImpl.h"
#include "entrance/platform/client/fp10/core/resource/tara/TARAParser.h"
#include "game/alternativa/tanks/model/item3d/Object3DSModel.h"
#include "game/projects/tanks/clients/flash/resources/resource/MapResource.h"


extern BattleService *battleService;
static BattleInputServiceImpl battleInputServiceImpl;
BattleInputService *battleInputService = &battleInputServiceImpl;
static ResourceRegistry *resourceRegistry = new ResourceRegistryImpl;





void load(const QString &type, const Long &id, const Long &version)
{
	QString path_map_proplibs = path::getResources() + "/" + type + "/" + id.toString() + " " + version.toString() + "/library.tara";

	//qDebug() << path_map_proplibs;
	QByteArray data = File::read(path_map_proplibs);


#if 0

	TARAParser *tara = new TARAParser(data);

	const QMap<QString, QByteArray> map = tara->data();

	QMapIterator<QString, QByteArray> i(map);
	while (i.hasNext())
	{
		i.next();
		QString name = i.key();
		QByteArray data = i.value();

		QString dir = path::get() + "/" + type + "/" + id.toString() + " " + version.toString() + "/tara";

		QDir().mkdir(dir);

		writeFile(dir + "/" + name, data);
	}
#endif


	ResourceInfo *resourceInfo = new ResourceInfo(7, id, version, false);
	PropLibResource *propLibResource = new PropLibResource(resourceInfo);

	propLibResource->createLibrary(data);


	resourceRegistry->registerResource(propLibResource);
}






MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setMouseTracking(true);
	centralWidget()->setMouseTracking(true);

	ui->frame->hide();

	timer = new QTimer;
	connect(timer, &QTimer::timeout, this, &MainWindow::slotTime);


#if 0
	{
		QByteArray data;
		if (!File::read("shaders.ini", data))
		{
			qDebug("error shaders.ini");
			return;
		}

		ShaderDir::set(QString::fromUtf8(data).trimmed());
	}
#endif

	vulkanFunctions = new VulkanFunctions;
	if (!vulkanFunctions->load())
	{
		qDebug() << "error VulkanFunctions";
	}





	MapResource::resourceRegistry = resourceRegistry;
	Object3DSModel::resourceRegistry = resourceRegistry;


	load("PROP_LIB", Long(0x00000000, 0x00989E5C), Long(0x0000016B, 0x899DBD39)); //00000000 00989E5C 0000016B 899DBD39
	load("PROP_LIB", Long(0x00000000, 0x1DD10D54), Long(0x0000016B, 0x8A835130)); //00000000 1DD10D54 0000016B 8A835130
	load("PROP_LIB", Long(0x00000000, 0x00989E4D), Long(0x0000016B, 0x8B1803E4)); //00000000 00989E4D 0000016B 8B1803E4
	load("PROP_LIB", Long(0x00000000, 0x00989E5F), Long(0x0000016B, 0x89BDE730)); //00000000 00989E5F 0000016B 89BDE730
	load("PROP_LIB", Long(0x00000000, 0x00989E50), Long(0x0000016B, 0x8A0E3D19)); //00000000 00989E50 0000016B 8A0E3D19
	load("PROP_LIB", Long(0x00000000, 0x00989E56), Long(0x0000016B, 0x8A53F1BD)); //00000000 00989E56 0000016B 8A53F1BD
	load("PROP_LIB", Long(0x00000000, 0x00989E77), Long(0x0000016B, 0xDA6EE1B2)); //00000000 00989E77 0000016B DA6EE1B2
	load("PROP_LIB", Long(0x00000000, 0x00989E80), Long(0x0000016B, 0x89FD37DB)); //00000000 00989E80 0000016B 89FD37DB
	load("PROP_LIB", Long(0x00000000, 0x00989E83), Long(0x0000016B, 0x8993EF70)); //00000000 00989E83 0000016B 8993EF70
	load("PROP_LIB", Long(0x00000000, 0x00989E59), Long(0x0000016B, 0x8A796FD3)); //00000000 00989E59 0000016B 8A796FD3
	load("PROP_LIB", Long(0x00000000, 0x00989E98), Long(0x0000016B, 0x89D01F35)); //00000000 00989E98 0000016B 89D01F35
	load("PROP_LIB", Long(0x00000000, 0x00989E7A), Long(0x0000016B, 0x89E7EE88)); //00000000 00989E7A 0000016B 89E7EE88
	load("PROP_LIB", Long(0x00000000, 0x00989E89), Long(0x0000016B, 0x8999EFD5)); //00000000 00989E89 0000016B 8999EFD5
	load("PROP_LIB", Long(0x00000000, 0x00989DF9), Long(0x0000016B, 0x8A176B07)); //00000000 00989DF9 0000016B 8A176B07
	load("PROP_LIB", Long(0x00000000, 0x00989DF3), Long(0x0000016B, 0x8A05DAFE)); //00000000 00989DF3 0000016B 8A05DAFE
	load("PROP_LIB", Long(0x00000000, 0x00989E7D), Long(0x0000016B, 0x89F981BC)); //00000000 00989E7D 0000016B 89F981BC


	battlefieldModel = new BattlefieldModel;
	battlefieldModel->objectLoaded();

	this->init = new _init(resourceRegistry);


	camera = battlefieldModel->getBattleScene3D()->getCamera();


	{
		Vector3d angularVelocity(0, 0, 0);
		int chassisControl = 0;
		int hassisTurnSpeedNumber = 0;
		Vector3d linearVelocity(0, 0, 0);

		for (int i = 0; i < 0; i++)
		{
			Vector3d orientation(-0.000758318, 0, 0);
			Vector3d position(1508 + (i * 500), -6062, 1600);

			TankState *tankstate = new TankState(&angularVelocity, chassisControl,
												 hassisTurnSpeedNumber, &linearVelocity,
												 &orientation, &position);

			this->init->createTank(0x10002 + i, false, 100, tankstate);
		}

		{
			Vector3d orientation(-0.000758318, 0, 0);
			Vector3d position(1008., -6062., 1600);

			TankState *tankstate = new TankState(&angularVelocity, chassisControl,
												 hassisTurnSpeedNumber, &linearVelocity,
												 &orientation, &position);

			localTank = this->init->createTank(0x10001, true, 100, tankstate);
		}

	}

	battleService->setCameraPosition();

	if (0)
	{
		widgetPrimitive = new WidgetPrimitive;
		widgetPrimitive->setBattlefieldModel(battlefieldModel);
		widgetPrimitive->setGameCamera(camera);

		QWidget *widget = widgetPrimitive; //QWidget::createWindowContainer(widgetPrimitive);
		//widgetPrimitive->setFocusPolicy(Qt::StrongFocus);
		widget->setFocus();
		widget->setParent(this);
		widget->setGeometry(0, 0, this->width() / 2, this->height());
	}


	{

		VkInstance vkInstance = VK_createInstance(vulkanFunctions);

		VulkanWindow2 *vulkanWindow = new VulkanWindow2();

		vulkanWindow->setVulkanInstance(vkInstance);
		vulkanWindow->setFunctions(vulkanFunctions);

		vulkanWindow->setBattlefieldModel(battlefieldModel);
		vulkanWindow->setGameCamera(camera);
		vulkanWindow->setBattleInputService(&battleInputServiceImpl);


		widget_vulkan = QWidget::createWindowContainer(vulkanWindow);
		widget_vulkan->setFocusPolicy(Qt::NoFocus);
		//widget_vulkan->setFocus();
		widget_vulkan->setParent(this->ui->widget);

		//widget_vulkan->setGeometry(this->width() / 2, 0, this->width() / 2, this->height());
		widget_vulkan->setGeometry(0, 0, widget_vulkan->width(), widget_vulkan->height());


		battlefieldModel->getBattleScene3D()->setRenderVulkan(vulkanWindow);
	}

	timer->setInterval(16);
	timer->setTimerType(Qt::CoarseTimer);
	timer->start();
}


MainWindow::~MainWindow()
{
	delete timer;
	delete ui;
}


void MainWindow::slotTime()
{
	battlefieldModel->onEnterFrame();
	//widgetPrimitive->repaint();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

	int key = event->nativeScanCode();

	battleInputServiceImpl.onKeyDown(key);
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
		return;

	int key = event->nativeScanCode();

	battleInputServiceImpl.onKeyUp(key);
}


void MainWindow::closeEvent(QCloseEvent*)
{

}


void MainWindow::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();

	(void)size;

	//int width = size.width();
	//int height = size.height();

	int width = ui->widget->width();
	int height = ui->widget->height();

	//widgetPrimitive->setGeometry(0, 0, width / 2, height);

	//widget_vulkan->setGeometry(width / 2, 0, width / 2, height);
	widget_vulkan->setGeometry(0, 0, width, height);
}


void MainWindow::showEvent(QShowEvent *event)
{
	(void)event;
	int width = ui->widget->width();
	int height = ui->widget->height();
	widget_vulkan->setGeometry(0, 0, width, height);
}

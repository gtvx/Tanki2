#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class GameCamera;
class QTimer;
class BattlefieldModel;
class Tank;
class _init;
class WidgetPrimitive;
class VulkanFunctions;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	_init *init;
	GameCamera *camera;
	WidgetPrimitive *widgetPrimitive;
	QWidget *widget_vulkan;
	VulkanFunctions *vulkanFunctions;
	QTimer *timer;
	Tank *localTank;
	BattlefieldModel *battlefieldModel;

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
	void slotTime();
	void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

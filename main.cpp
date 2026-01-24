#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <fenv.h>



#if 0
#include <QDir>
#include "path.h"
#include <QFileSystemWatcher>
#include <QProcess>

class FileShader
{
	QString filepath_source;
	QString filepath_spv;

public:
	FileShader(const QString &filepath_source, const QString &filepath_spv)
	{
		this->filepath_source = filepath_source;
		this->filepath_spv = filepath_spv;
	}

	QString source()
	{
		return filepath_source;
	}

	QString spv()
	{
		return filepath_spv;
	}
};

class DynamicChangeShaderCompiller : public QObject
{
	QFileSystemWatcher fileWatcher;

	QHash<QString, FileShader*> hash;

	void fileChanged(const QString &path)
	{
		//qDebug() << "fileChanged" << path;

		FileShader *fileShader = hash[path];

		if (fileShader == nullptr)
			return;

		QStringList arguments;

		arguments << "-V";
		arguments << fileShader->source();
		arguments << "-o";
		arguments << fileShader->spv();

		QProcess::execute("glslangValidator", arguments);


		fileWatcher.addPath(path);
	}


	void add(const QString &source, const QString &spriv)
	{
		if (spriv.endsWith(".vert") || spriv.endsWith(".frag"))
		{
			qDebug("error DynamicChangeShaderCompiller::add");
			return;
		}

		QDir dir(path::getShaders() + "../" + source);
		QString path_source = dir.absolutePath();
		hash[path_source] = new FileShader(path_source, path::getShaders() + spriv);

		fileWatcher.addPath(path_source);
	}


public:
	DynamicChangeShaderCompiller()
	{
		add("PaintVertex.vert", "PaintVertex.spv");
		add("AnimatedPaintFragment.frag", "AnimatedPaintFragment.spv");
		QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &DynamicChangeShaderCompiller::fileChanged);
	}
};
#endif

int main(int argc, char *argv[])
{
	//feenableexcept(FE_INVALID);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
#endif

	QApplication a(argc, argv);
	QDir::setCurrent(a.applicationDirPath());

	MainWindow w;
	w.show();

	//DynamicChangeShaderCompiller dynamicChangeShaderCompiller;

	return a.exec();
}

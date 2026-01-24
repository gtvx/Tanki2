#include "path.h"
#include <QDir>


namespace
{

	QString remove_last_dir(QString dir)
	{
		int v1 = dir.lastIndexOf(QChar('/'));
		int v2 = dir.lastIndexOf(QChar('\\'));

		if (v1 == v2)
			return QString();

		int v = std::max(v1, v2);

		return dir.mid(0, v);
	}

	QString getProjectDir()
	{
		QString current = QDir::currentPath();

		for (int i = 0; i < 5; i++)
		{
			current = remove_last_dir(current);
			if (QFile::exists(current + "/main.cpp"))
			{
				return current + "/";
			}
		}

		return {};
	}



	QString getShaderBuildDir()
	{
		auto project_dir = getProjectDir();

		if (project_dir.isEmpty())
			return {};

		const auto build_dir = project_dir + "shaders/build/";

		if (QDir(build_dir).exists())
		{
			return build_dir;
		}

		return {};
	}


	QString getShaderDir()
	{
		QFile file(QDir::currentPath() + "/shaders.ini");

		if (file.open(QIODevice::ReadOnly))
		{
			return QString::fromUtf8(file.readAll().trimmed());
		}

		QString current_shader = QDir::currentPath() + "/resources/shaders/build/";

		if (QDir(current_shader).exists())
		{
			return current_shader;
		}

		QString project_shader_dir = getShaderBuildDir();

		if (project_shader_dir.isEmpty())
			return {};

		if (file.open(QIODevice::WriteOnly))
		{
			file.write(project_shader_dir.toUtf8());
		}

		return project_shader_dir;
	}


	QString getResourcesDir()
	{
		QFile file(QDir::currentPath() + "/resources.ini");

		if (file.open(QIODevice::ReadOnly))
		{
			return QString::fromUtf8(file.readAll().trimmed());
		}

		QString current_resources = QDir::currentPath() + "/resources/";

		if (QDir(current_resources).exists())
		{
			return current_resources;
		}

		auto project_dir = getProjectDir();

		if (project_dir.isEmpty())
			return {};

		QString resources_dir = project_dir + "resources/";

		if (!QDir(resources_dir).exists())
		{
			return {};
		}

		if (file.open(QIODevice::WriteOnly))
		{
			file.write(resources_dir.toUtf8());
		}

		return resources_dir;
	}


	QString path_resources;
	QString path_shaders;
}



QString path::getResources()
{
	if (path_resources.isEmpty())
	{
		path_resources = getResourcesDir();
	}

	return path_resources;
}


QString path::getShaders()
{
	if (path_shaders.isEmpty())
	{
		path_shaders = getShaderDir();
	}

	return path_shaders;
}

#ifndef PARSER3DS_H
#define PARSER3DS_H

#include <QByteArray>
#include <QVector>
#include <memory>

class Material;
class Object3D;
class TextureMaterial;

class Parser3DSResult
{
public:
	bool ok;
	QVector<std::shared_ptr<Object3D>> objects;
	QVector<std::shared_ptr<Object3D>> parents;
	QVector<std::shared_ptr<Material>> materials;
	QVector<std::shared_ptr<TextureMaterial>> textureMaterials;
};


Parser3DSResult Parser3DS(const QString &name,
						  const QByteArray &data,
						  const QString &texturesBaseURL = "",
						  double scale = 1.0);

#endif // PARSER3DS_H

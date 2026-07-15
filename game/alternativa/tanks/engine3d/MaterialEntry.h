#ifndef MATERIALENTRY_H
#define MATERIALENTRY_H

#include <memory>
#include <QString>

class BitmapData;
class TextureMaterial;

class MaterialEntry
{
public:
	std::shared_ptr<BitmapData> keyData_bitmapData;
	QString keyData_string;

	BitmapData *texture;
	std::shared_ptr<TextureMaterial> material;
	int referenceCount;

	MaterialEntry(std::shared_ptr<BitmapData> keyData, std::shared_ptr<TextureMaterial> material);
	MaterialEntry(const QString &keyData, std::shared_ptr<TextureMaterial> material);
};

#endif // MATERIALENTRY_H

#ifndef NORMALSCALCULATOR_H
#define NORMALSCALCULATOR_H

#include <QVector>
#include <memory>

class Object3D;

class NormalsCalculator
{
public:
	NormalsCalculator();
	void calculateNormals(QVector<std::shared_ptr<Object3D>> &list);

private:
	void clearSmoothingGroups(QVector<std::shared_ptr<Object3D> > &list);
};

#endif // NORMALSCALCULATOR_H

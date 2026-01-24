#ifndef KDNODE_H
#define KDNODE_H

#include <memory>
#include <QVector>

class Object3D;
class Receiver;
class Shadow;
class Decal;

class KDNode
{
	std::shared_ptr<KDNode> m_negative;
	std::shared_ptr<KDNode> m_positive;

	KDNode *raw_negative;
	KDNode *raw_positive;

public:
	KDNode();


	void setNegative(std::shared_ptr<KDNode> node)
	{
		this->m_negative = node;
		this->raw_negative = node.get();
	}

	KDNode* negative()
	{
		return this->raw_negative;
	}


	void setPositive(std::shared_ptr<KDNode> node)
	{
		this->m_positive = node;
		this->raw_positive = node.get();
	}

	KDNode* positive()
	{
		return this->raw_positive;
	}


	void clip(Decal*, double, double, Object3D*);
	void collectPolygons(Decal*, double, double, double, double, double, double, double, double);
	void collectReceivers(Shadow*);
	void createReceivers(QVector<QVector<float>*> &param1, QVector<QVector<uint>*> &param2);

	double coord;

	double minCoord;
	double maxCoord;

	double boundMinX;
	double boundMinY;
	double boundMinZ;
	double boundMaxX;
	double boundMaxY;
	double boundMaxZ;

	int axis;

	std::shared_ptr<Object3D> objectList;
	std::shared_ptr<Object3D> objectBoundList;
	std::shared_ptr<Object3D> occluderList;
	std::shared_ptr<Object3D> occluderBoundList;
	std::shared_ptr<Receiver> receiverList;
};

#endif // KDNODE_H

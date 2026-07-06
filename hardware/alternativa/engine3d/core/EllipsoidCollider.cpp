#include "EllipsoidCollider.h"
#include "flash/geom/Vector3D.h"
#include "hardware/alternativa/engine3d/core/Object3D.h"
#include "MyMath.h"
#include "Wrapper.h"
#include "Vertex.h"
#include "Face.h"
#include "Faces.h"


EllipsoidCollider::EllipsoidCollider(double radiusX, double radiusY, double radiusZ)
{
	this->matrix = new_Object3D();
	this->src = new Vector3D();
	this->displ = new Vector3D();
	this->dest = new Vector3D();
	this->collisionPoint = new Vector3D();
	this->collisionPlane = new Vector3D();
	this->vCenter = new Vector3D();
	this->vA = new Vector3D();
	this->vB = new Vector3D();
	this->vC = new Vector3D();
	this->vD = new Vector3D();
	this->radiusX = radiusX;
	this->radiusY = radiusY;
	this->radiusZ = radiusZ;
	this->threshold = 0.001;
	this->facesLength = 0;
	this->radius = 0;

	this->src->reset();
	this->displ->reset();
	this->dest->reset();
	this->collisionPoint->reset();
	this->collisionPlane->reset();
	this->vCenter->reset();
	this->vA->reset();
	this->vB->reset();
	this->vC ->reset();
	this->vD->reset();
}


bool EllipsoidCollider::getCollision(const Vector3D *source,
									 const Vector3D *displacement,
									 Vector3D *resCollisionPoint,
									 Vector3D *resCollisionPlane,
									 Object3D *object)
{
	double abx;
	double aby;
	double abz;
	double acx;
	double acy;
	double acz;
	double abx2;
	double aby2;
	double abz2;
	double acx2;
	double acy2;
	double acz2;

	if(displacement->getLength() <= this->threshold)
	{
		return false;
	}

	this->prepare(source, displacement, object, nullptr, true);

	if(this->facesLength > 0)
	{
		if(this->checkCollision())
		{
			this->matrix->composeMatrix();
			resCollisionPoint->x = this->matrix->matrix4.ma * this->collisionPoint->x + this->matrix->matrix4.mb * this->collisionPoint->y + this->matrix->matrix4.mc * this->collisionPoint->z + this->matrix->matrix4.md;
			resCollisionPoint->y = this->matrix->matrix4.me * this->collisionPoint->x + this->matrix->matrix4.mf * this->collisionPoint->y + this->matrix->matrix4.mg * this->collisionPoint->z + this->matrix->matrix4.mh;
			resCollisionPoint->z = this->matrix->matrix4.mi * this->collisionPoint->x + this->matrix->matrix4.mj * this->collisionPoint->y + this->matrix->matrix4.mk * this->collisionPoint->z + this->matrix->matrix4.ml;
			if(this->collisionPlane->x < this->collisionPlane->y)
			{
				if(this->collisionPlane->x < this->collisionPlane->z)
				{
					abx = 0;
					aby = -this->collisionPlane->z;
					abz = this->collisionPlane->y;
				}
				else
				{
					abx = -this->collisionPlane->y;
					aby = this->collisionPlane->x;
					abz = 0;
				}
			}
			else if(this->collisionPlane->y < this->collisionPlane->z)
			{
				abx = this->collisionPlane->z;
				aby = 0;
				abz = -this->collisionPlane->x;
			}
			else
			{
				abx = -this->collisionPlane->y;
				aby = this->collisionPlane->x;
				abz = 0;
			}
			acx = this->collisionPlane->z * aby - this->collisionPlane->y * abz;
			acy = this->collisionPlane->x * abz - this->collisionPlane->z * abx;
			acz = this->collisionPlane->y * abx - this->collisionPlane->x * aby;
			abx2 = this->matrix->matrix4.ma * abx + this->matrix->matrix4.mb * aby + this->matrix->matrix4.mc * abz;
			aby2 = this->matrix->matrix4.me * abx + this->matrix->matrix4.mf * aby + this->matrix->matrix4.mg * abz;
			abz2 = this->matrix->matrix4.mi * abx + this->matrix->matrix4.mj * aby + this->matrix->matrix4.mk * abz;
			acx2 = this->matrix->matrix4.ma * acx + this->matrix->matrix4.mb * acy + this->matrix->matrix4.mc * acz;
			acy2 = this->matrix->matrix4.me * acx + this->matrix->matrix4.mf * acy + this->matrix->matrix4.mg * acz;
			acz2 = this->matrix->matrix4.mi * acx + this->matrix->matrix4.mj * acy + this->matrix->matrix4.mk * acz;
			resCollisionPlane->x = abz2 * acy2 - aby2 * acz2;
			resCollisionPlane->y = abx2 * acz2 - abz2 * acx2;
			resCollisionPlane->z = aby2 * acx2 - abx2 * acy2;
			resCollisionPlane->normalize();
			resCollisionPlane->w = resCollisionPoint->x * resCollisionPlane->x + resCollisionPoint->y * resCollisionPlane->y + resCollisionPoint->z * resCollisionPlane->z;
			this->faces.clear();
			return true;
		}
		this->faces.clear();
		return false;
	}

	return false;
}


void EllipsoidCollider::prepare(const Vector3D *source, const Vector3D *displacement, Object3D *object, void *excludedObjects, bool param5)
{
	(void)param5;

	this->radius = this->radiusX;
	if(this->radiusY > this->radius)
	{
		this->radius = this->radiusY;
	}
	if(this->radiusZ > this->radius)
	{
		this->radius = this->radiusZ;
	}
	this->matrix->scaleX = this->radiusX / this->radius;
	this->matrix->scaleY = this->radiusY / this->radius;
	this->matrix->scaleZ = this->radiusZ / this->radius;
	this->matrix->x = source->x;
	this->matrix->y = source->y;
	this->matrix->z = source->z;
	this->matrix->composeMatrix();
	this->matrix->invertMatrix();
	this->src->x = 0;
	this->src->y = 0;
	this->src->z = 0;
	this->displ->x = this->matrix->matrix4.ma * displacement->x + this->matrix->matrix4.mb * displacement->y + this->matrix->matrix4.mc * displacement->z;
	this->displ->y = this->matrix->matrix4.me * displacement->x + this->matrix->matrix4.mf * displacement->y + this->matrix->matrix4.mg * displacement->z;
	this->displ->z = this->matrix->matrix4.mi * displacement->x + this->matrix->matrix4.mj * displacement->y + this->matrix->matrix4.mk * displacement->z;
	this->dest->x = this->src->x + this->displ->x;
	this->dest->y = this->src->y + this->displ->y;
	this->dest->z = this->src->z + this->displ->z;
	double rad = this->radius + this->displ->getLength();
	object->composeAndAppend(this->matrix.get());
	this->vA->x = -rad;
	this->vA->y = -rad;
	this->vA->z = -rad;
	this->vB->x = rad;
	this->vB->y = -rad;
	this->vB->z = -rad;
	this->vC->x = rad;
	this->vC->y = rad;
	this->vC->z = -rad;
	this->vD->x = -rad;
	this->vD->y = rad;
	this->vD->z = -rad;

	Faces p;
	p.faces = &this->faces;


	object->collectPlanes(this->vCenter,
						  this->vA,
						  this->vB,
						  this->vC,
						  this->vD,
						  &p,
						  excludedObjects);

	this->facesLength = this->faces.length();

}


bool EllipsoidCollider::checkCollision()
{
	Face *face = nullptr;
	Wrapper *w = nullptr;
	Vertex *a = nullptr;
	Vertex *b = nullptr;
	Vertex *c = nullptr;

	double abx = MyMath::nan();
	double aby = MyMath::nan();
	double abz = MyMath::nan();
	double acx = MyMath::nan();
	double acy = MyMath::nan();
	double acz = MyMath::nan();
	double normalX = MyMath::nan();
	double normalY = MyMath::nan();
	double normalZ = MyMath::nan();
	double len = MyMath::nan();
	double offset = MyMath::nan();
	double distance = MyMath::nan();
	double pointX = MyMath::nan();
	double pointY = MyMath::nan();
	double pointZ = MyMath::nan();
	double faceX = MyMath::nan();
	double faceY = MyMath::nan();
	double faceZ = MyMath::nan();
	double min = MyMath::nan();
	bool inside = false;
	Wrapper *wrapper = nullptr;
	double deltaX = MyMath::nan();
	double deltaY = MyMath::nan();
	double deltaZ = MyMath::nan();
	double t = MyMath::nan();
	double crx = MyMath::nan();
	double cry = MyMath::nan();
	double crz = MyMath::nan();
	double edgeLength = MyMath::nan();
	double edgeDistanceSqr = MyMath::nan();
	double acLen = MyMath::nan();
	double backX = MyMath::nan();
	double backY = MyMath::nan();
	double backZ = MyMath::nan();
	double deltaLength = MyMath::nan();
	double projectionLength = MyMath::nan();
	double projectionInsideLength = MyMath::nan();
	double time = MyMath::nan();
	double minTime = 1;

	double displacementLength = this->displ->getLength();

	for(int i = 0; i < this->facesLength; i++)
	{
		face = this->faces[i].get();
		w = face->wrapper.get();
		a = w->vertex.get();
		w = w->next.get();
		b = w->vertex.get();
		w = w->next.get();
		c = w->vertex.get();
		abx = b->cameraX - a->cameraX;
		aby = b->cameraY - a->cameraY;
		abz = b->cameraZ - a->cameraZ;
		acx = c->cameraX - a->cameraX;
		acy = c->cameraY - a->cameraY;
		acz = c->cameraZ - a->cameraZ;
		normalX = acz * aby - acy * abz;
		normalY = acx * abz - acz * abx;
		normalZ = acy * abx - acx * aby;
		len = normalX * normalX + normalY * normalY + normalZ * normalZ;
		if(len > 0.001)
		{
			len = 1 / MyMath::sqrt(len);
			normalX *= len;
			normalY *= len;
			normalZ *= len;
			offset = a->cameraX * normalX + a->cameraY * normalY + a->cameraZ * normalZ;
			distance = this->src->x * normalX + this->src->y * normalY + this->src->z * normalZ - offset;
			if(distance < this->radius)
			{
				pointX = this->src->x - normalX * distance;
				pointY = this->src->y - normalY * distance;
				pointZ = this->src->z - normalZ * distance;
			}
			else
			{
				t = (distance - this->radius) / (distance - this->dest->x * normalX - this->dest->y * normalY - this->dest->z * normalZ + offset);
				pointX = this->src->x + this->displ->x * t - normalX * this->radius;
				pointY = this->src->y + this->displ->y * t - normalY * this->radius;
				pointZ = this->src->z + this->displ->z * t - normalZ * this->radius;
			}
			min = 1e+22;
			inside = true;
			for(wrapper = face->wrapper.get(); wrapper != nullptr; wrapper = wrapper->next.get())
			{
				a = wrapper->vertex.get();
				b = wrapper->next != nullptr ? wrapper->next->vertex.get() : face->wrapper->vertex.get();
				abx = b->cameraX - a->cameraX;
				aby = b->cameraY - a->cameraY;
				abz = b->cameraZ - a->cameraZ;
				acx = pointX - a->cameraX;
				acy = pointY - a->cameraY;
				acz = pointZ - a->cameraZ;
				crx = acz * aby - acy * abz;
				cry = acx * abz - acz * abx;
				crz = acy * abx - acx * aby;
				if(crx * normalX + cry * normalY + crz * normalZ < 0)
				{
					edgeLength = abx * abx + aby * aby + abz * abz;
					edgeDistanceSqr = (crx * crx + cry * cry + crz * crz) / edgeLength;
					if(edgeDistanceSqr < min)
					{
						edgeLength = MyMath::sqrt(edgeLength);
						abx /= edgeLength;
						aby /= edgeLength;
						abz /= edgeLength;
						t = abx * acx + aby * acy + abz * acz;
						if(t < 0)
						{
							acLen = acx * acx + acy * acy + acz * acz;
							if(acLen < min)
							{
								min = acLen;
								faceX = a->cameraX;
								faceY = a->cameraY;
								faceZ = a->cameraZ;
							}
						}
						else if(t > edgeLength)
						{
							acx = pointX - b->cameraX;
							acy = pointY - b->cameraY;
							acz = pointZ - b->cameraZ;
							acLen = acx * acx + acy * acy + acz * acz;
							if(acLen < min)
							{
								min = acLen;
								faceX = b->cameraX;
								faceY = b->cameraY;
								faceZ = b->cameraZ;
							}
						}
						else
						{
							min = edgeDistanceSqr;
							faceX = a->cameraX + abx * t;
							faceY = a->cameraY + aby * t;
							faceZ = a->cameraZ + abz * t;
						}
					}
					inside = false;
				}
			}
			if(inside)
			{
				faceX = pointX;
				faceY = pointY;
				faceZ = pointZ;
			}
			deltaX = this->src->x - faceX;
			deltaY = this->src->y - faceY;
			deltaZ = this->src->z - faceZ;
			if(deltaX * this->displ->x + deltaY * this->displ->y + deltaZ * this->displ->z <= 0)
			{
				backX = -this->displ->x / displacementLength;
				backY = -this->displ->y / displacementLength;
				backZ = -this->displ->z / displacementLength;
				deltaLength = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
				projectionLength = deltaX * backX + deltaY * backY + deltaZ * backZ;
				projectionInsideLength = this->radius * this->radius - deltaLength + projectionLength * projectionLength;
				if(projectionInsideLength > 0)
				{
					time = (projectionLength - MyMath::sqrt(projectionInsideLength)) / displacementLength;
					if(time < minTime)
					{
						minTime = time;
						this->collisionPoint->x = faceX;
						this->collisionPoint->y = faceY;
						this->collisionPoint->z = faceZ;
						if(inside)
						{
							this->collisionPlane->x = normalX;
							this->collisionPlane->y = normalY;
							this->collisionPlane->z = normalZ;
							this->collisionPlane->w = offset;
						}
						else
						{
							deltaLength = MyMath::sqrt(deltaLength);
							this->collisionPlane->x = deltaX / deltaLength;
							this->collisionPlane->y = deltaY / deltaLength;
							this->collisionPlane->z = deltaZ / deltaLength;
							this->collisionPlane->w = this->collisionPoint->x * this->collisionPlane->x + this->collisionPoint->y * this->collisionPlane->y + this->collisionPoint->z * this->collisionPlane->z;
						}
					}
				}
			}
		}
	}
	return minTime < 1;
}

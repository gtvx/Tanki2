#include "RayCollisionFilter.h"


RayCollisionFilter::RayCollisionFilter(Body *body)
{
	this->body = body;
}


bool RayCollisionFilter::considerBody(Body *body)
{
	return this->body != body;
}


void RayCollisionFilter::setBody(Body *body)
{
	this->body = body;
}


Body* RayCollisionFilter::getBody()
{
	return body;
}

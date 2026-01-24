#include "UVMatrixProvider.h"
#include "flash/geom/Matrix.h"


UVMatrixProvider::UVMatrixProvider()
{
	matrix = new Matrix;
}


UVMatrixProvider::~UVMatrixProvider()
{
	delete matrix;
}


void UVMatrixProvider::getValues(float matrixValues[8])
{
	matrixValues[0] = matrix->a;
	matrixValues[1] = matrix->b;
	matrixValues[2] = matrix->tx;
	matrixValues[3] = 0;
	matrixValues[4] = matrix->c;
	matrixValues[5] = matrix->d;
	matrixValues[6] = matrix->ty;
	matrixValues[7] = 0;
}

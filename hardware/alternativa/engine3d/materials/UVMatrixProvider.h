#ifndef UVMATRIXPROVIDER_H
#define UVMATRIXPROVIDER_H

class Matrix;

class UVMatrixProvider
{
	Matrix *matrix;

public:
	UVMatrixProvider();
	~UVMatrixProvider();

	Matrix* getMatrix()
	{
		return matrix;
	}

	void getValues(float matrixValues[8]);
};

#endif // UVMATRIXPROVIDER_H

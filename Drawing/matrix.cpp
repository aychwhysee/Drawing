#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
	data = new double[3];
	data[0] = data[1] = 0.0;
	data[2] = 1.0;
}

Vector::Vector(const Vector& oldVector) 
{
	data = new double[3];
	for (int i = 0; i < 3; ++i)
	{
		data[i] = oldVector.data[i];
	}
}

Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1.0;
}


Vector::~Vector()
{
	delete[] data;
}

double& Vector::operator[](int index) const
{
	return data[index];
}

Matrix::Matrix() 
{
	data = new double*[3];
	data[0] = new double[3];
	data[1] = new double[3];
	data[2] = new double[3];
	data[0][0] = 1.0;
	data[0][1] = 0.0;
	data[0][2] = 0.0;
	data[1][0] = 0.0;
	data[1][1] = 1.0;
	data[1][2] = 0.0;
	data[2][0] = 0.0;
	data[2][1] = 0.0;
	data[2][2] = 1.0;
}

Matrix::Matrix(const Matrix& oldMatrix) 
{
	data = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		data[i] = new double[3];
		for (int j = 0; j < 3; ++j)
		{
			data[i][j] = oldMatrix.data[i][j];
		}
	}
}

Matrix::~Matrix()
{
	delete[] data[0];
	delete[] data[1];
	delete[] data[2];
	delete[] data;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	Matrix* result = new Matrix();
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			double sum = 0.0;
			for (int k = 0; k < 3; ++k)
			{
				sum += data[i][k] * (*otherMatrix)[k][j];				
			}
			(*result)[i][j] = sum;
		}
	}
	return result;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	Vector* result = new Vector();
	for (int i = 0; i < 3; ++i)
	{
		(*result)[i]= 0;
		for (int j = 0; j < 3; ++j)
		{
			(*result)[i] += (data[i][j] * (*theVector)[j]);						
		}
		 
	}
	return result;
}

double* Matrix::operator[](int index) const
{
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* translateMatrix = new Matrix();
	//Since new Matrix() makes the identity, only need to change values for certain entries
	(*translateMatrix)[0][2] = deltaX;
	(*translateMatrix)[1][2] = deltaY;
	return translateMatrix;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* rotateMatrix = new Matrix();
	//Since new Matrix() makes the identity, only need to change values for certain entries
	(*rotateMatrix)[0][0] = cos(theta);
	(*rotateMatrix)[0][1] = -sin(theta);
	(*rotateMatrix)[1][0] = sin(theta);
	(*rotateMatrix)[1][1] = cos(theta);
	return rotateMatrix;
}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	Matrix* shearMatrix = new Matrix();
	//Since new Matrix() makes the identity, only need to change values for certain entries
	(*shearMatrix)[0][1] = shearXY;
	(*shearMatrix)[1][0] = shearYX;
	return shearMatrix;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* scaleMatrix = new Matrix();
	//Since new Matrix() makes the identity, only need to change values for certain entries
	(*scaleMatrix)[0][0] = scaleX;
	(*scaleMatrix)[1][1] = scaleY;
	return scaleMatrix;
}

Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->data[0][0] = -data[1][2] * data[2][1] + data[1][1] * data[2][2];
	answer->data[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	answer->data[0][2] = -data[0][2] * data[1][1] + data[0][1] * data[1][2];
	answer->data[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	answer->data[1][1] = -data[0][2] * data[2][0] + data[0][0] * data[2][2];
	answer->data[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	answer->data[2][0] = -data[1][1] * data[2][0] + data[1][0] * data[2][1];
	answer->data[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	answer->data[2][2] = -data[0][1] * data[1][0] + data[0][0] * data[1][1];

	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			answer->data[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][2] * data[2][0] +
		data[0][2] * data[1][0] * data[2][1] - data[0][0] * data[1][2] * data[2][1] -
		data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];
}

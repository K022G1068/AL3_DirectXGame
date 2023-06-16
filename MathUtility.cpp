#include "MathUtility.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) 
{ 
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	
	return result; 
}

Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result;
	float determinant = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                    matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                    matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                    matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                    matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                    matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
	                    matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
	                    matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
	                    matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
	                    matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
	                    matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                    matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
	                    matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
	                    matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
	                    matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] -
	                    matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
	                    matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                    matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
	                    matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
	                    matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                    matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
	                    matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
	                    matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                    matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	float determinantRecp = 1.0f / determinant;

	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][1] = (-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
	                  matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
	                  matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
	                  matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
	                  matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] +
	                  matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][3] = (-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] +
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) *
	                 determinantRecp;
	result.m[1][0] = (-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
	                  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
	                  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
	                  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
	                  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
	                  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
	                  matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
	                  matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
	                  matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
	                  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
	                  matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][2] = (-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] +
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] -
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) *
	                 determinantRecp;
	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
	                  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][1] = (-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
	                  matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
	                  matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
	                  matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
	                  matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] +
	                  matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][3] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] +
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) *
	                 determinantRecp;
	result.m[3][0] = (-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
	                  matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
	                  matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
	                  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
	                  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
	                  matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
	                  matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
	                  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
	                  matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
	                  matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
	                  matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][2] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
	                  matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] +
	                  matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
	                  matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
	                  matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) *
	                 determinantRecp;
	return result;
}

float Lerp(float objPos, float targetPos, float time) 
{
	return objPos + (targetPos - objPos) * time;
}

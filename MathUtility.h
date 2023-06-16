#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
Matrix4x4 Inverse(const Matrix4x4& matrix);
float Lerp(float objPos, float targetPos, float time);
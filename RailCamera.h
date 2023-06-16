#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtility.h"
#include "ImGuiManager.h"
#include <vector>
#include "PrimitiveDrawer.h"
#include "Input.h"
#include "MathUtility.h"

class RailCamera {
public:
	~RailCamera();
	void Initialize(Vector3 worldPos, Vector3 worldRot, ViewProjection& viewProjection);
	void Update();
	const ViewProjection& GetViewProjection() { return viewProjection_; };
	const WorldTransform& GetWorldTransform() { return worldTransform_; };
	void Move();
	void Move2(std::vector<Vector3>& points);
	const Vector3 GetsplinePoint(float t); 
	void DrawSpline();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	std::vector<Vector3> points_;
	Input* input_ = nullptr;
	int num = 0;

	Vector3 current = {};
	Vector3 ahead = {};
	Vector3 distance = {};
	int aheadnum = 0;
	float frame = 0;
	float maxFrame = 240.0f;


	// List to keep track on point to draw
};

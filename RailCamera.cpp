#include "RailCamera.h"

RailCamera::~RailCamera() {}

void RailCamera::Initialize(Vector3 worldPos, Vector3 worldRot, ViewProjection& viewProjection) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = worldRot;
	viewProjection_.Initialize();
	points_ = {

	    {0,  0,  0},
	    {0,  0,  0},
        {0, 10, 10},
        {0, 15, 10},
        {0, 15, 20},
        {0, 0,  20},
        {0, 0,  30},
	};
	input_ = Input::GetInstance();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
}

void RailCamera::Update() 
{ 
	Move();
	worldTransform_.UpdateMatrix();
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	float sliderTrans[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	ImGui::DragFloat3(
	    "Translate",
	    sliderTrans,
	    0.01f);
	worldTransform_.translation_ = {sliderTrans[0], sliderTrans[1], sliderTrans[2]};
	float sliderRot[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z};
	ImGui::DragFloat3(
	    "Rotate",
	    sliderRot,
	    0.01f);
	worldTransform_.rotation_ = {sliderRot[0], sliderRot[1], sliderRot[2]};
	ImGui::End();
}

void RailCamera::Move() 
{
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_SPACE))
	{
		//move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_LSHIFT)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}

void RailCamera::Move2(std::vector<Vector3>& points) 
{

	float t = frame/maxFrame;
	aheadnum = num + 1;

	float z, y;
	z = current.z - ahead.z;
	y = current.y - ahead.y;
	float norm = sqrtf(powf(z, 2.0f) + powf(y, 2.0f));
	z = z / norm;
	y = y / norm;
	//float theta = atan2f(y, z);
	//int time = 0;
	if (aheadnum >= points.size() - 1)
	{
		aheadnum = int(points.size() - 1);
	}
	if (num >= points.size() - 1) {
		num = int(points.size() - 1);
	}
	current = points[num];
	ahead = points[aheadnum];


	distance = worldTransform_.translation_ - current;
	//distances = sqrtf(powf(distance.x, 2.0f) + powf(distance.y, 2.0f) + powf(distance.z, 2.0f));
	//worldTransform_.rotation_.x = Lerp(worldTransform_.rotation_.z, theta, t);
	//worldTransform_.rotation_.x = theta;
	worldTransform_.translation_.x = Lerp(worldTransform_.translation_.x, current.x, t);
	worldTransform_.translation_.y = Lerp(worldTransform_.translation_.y, current.y, t);
	if (t >= 1)
	{
		frame = 0;
	}
	num++;
	frame+=1.0f;
}

const Vector3 RailCamera::GetsplinePoint(float t) { 
	int p0, p1, p2, p3;
	p1 = int(t) + 1;
	p2 = p1 + 1;
	p3 = p1 + 1;
	p0 = p1 - 1;

	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.0f * tt - t;
	float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f * tt + t;
	float q4 = ttt - tt;

	float tx = 0.5f * (points_[p0].x * q1 + points_[p1].x * q2 + points_[p2].x * q3 + points_[p3].x * q4);
	float ty = 0.5f * (points_[p0].y * q1 + points_[p1].y * q2 + points_[p2].y * q3 + points_[p3].y * q4);
	float tz = 0.5f * (points_[p0].z * q1 + points_[p1].z * q2 + points_[p2].z * q3 + points_[p3].z * q4);
	
	return {tx, ty, tz};
}

void RailCamera::DrawSpline() 
{ 
	//List to keep track on point to draw
	std::vector<Vector3> pointsDrawing;

	//Line section
	//const float segmentCount = 1000;

	//Draw Spline
	for (float t = 0.0f; t < (float)points_.size() - 2; t += 0.01f){
		//float t = 1.0f / segmentCount * i;
		Vector3 pos = GetsplinePoint(t);
		pointsDrawing.push_back(pos);
	}

	for (int i = 1; i < pointsDrawing.size(); i++) {
		Vector3 point1 = pointsDrawing[i - 1];
		Vector3 point2 = pointsDrawing[i];
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    point1, point2, {1.0f, 0.0f, 0.0f, 1.0f});
	}

	if (input_->PushKey(DIK_TAB)) {
		Move2(pointsDrawing);
	}
}

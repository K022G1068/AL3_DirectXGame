#include "Skydome.h"
#include <assert.h>

Skydome::~Skydome() 
{

}

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};
}

void Skydome::Update() 
{ 
	worldTransform_.rotation_.y += 0.01f;
	worldTransform_.UpdateMatrix(); 
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

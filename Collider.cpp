#include "Collider.h"

void Collider::OnCollision() {}

void Collider::SetBox(Vector3& pos, Vector3& scale) 
{ 
	colliderBox_.min.x = pos.x - (scale.x);
	colliderBox_.min.y = pos.y - (scale.y);
	colliderBox_.min.z = pos.z - (scale.z);

	colliderBox_.max.x = pos.x + (scale.x);
	colliderBox_.max.y = pos.y + (scale.y);
	colliderBox_.max.z = pos.z + (scale.z);
}

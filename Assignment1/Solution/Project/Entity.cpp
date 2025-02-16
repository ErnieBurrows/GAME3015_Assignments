#include "Entity.hpp"

Entity::Entity(Game* game) : SceneNode(game), mVelocity(0, 0, 0)
{
}

void Entity::SetVelocity(XMFLOAT3 velocity)
{
	mVelocity = velocity;
}

void Entity::SetVelocity(float vx, float vy, float vz)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
	mVelocity.z = vz;
}

XMFLOAT3 Entity::GetVelocity() const
{
	return mVelocity;
}

void Entity::UpdateCurrent(const GameTimer& gt) 
{
	XMFLOAT3 velocity;
	velocity.x = mVelocity.x * gt.DeltaTime();
	velocity.y = mVelocity.y * gt.DeltaTime();
	velocity.z = mVelocity.z * gt.DeltaTime();

	Move(velocity.x, velocity.y, velocity.z);

	renderer->World = GetWorldTransform();
	renderer->NumFramesDirty++;
}

void Entity::DrawCurrent() const
{
	if (renderer) 
	{
		renderer->World = GetWorldTransform();
		renderer->NumFramesDirty++;
	}
}

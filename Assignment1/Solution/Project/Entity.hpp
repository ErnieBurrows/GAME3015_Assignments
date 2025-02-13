#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(Game* game);

	void SetVelocity(XMFLOAT3 velocity);
	void SetVelocity(float vx, float vy, float vz);
	XMFLOAT3 GetVelocity() const;

	virtual	void UpdateCurrent(const GameTimer& gt);

public:
	XMFLOAT3 mVelocity;
};


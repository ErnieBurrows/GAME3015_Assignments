#pragma once
#include "Aircraft.hpp"

struct AircraftMover
{
	AircraftMover(float vx, float vy, float vz) : velocity(vx, vy, vz) { }

	void operator() (SceneNode& node, const GameTimer& dt) const
	{
		// This downcast is ONLY SAFE if the command is only sent to Aircraft objects.
		Aircraft& aircraft = static_cast<Aircraft&>(node);
		aircraft.Accelerate(const_cast<XMFLOAT3&>(velocity));
	}

	XMFLOAT3 velocity;
};

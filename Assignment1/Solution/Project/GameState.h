#pragma once

#include "State.h"
#include "World.hpp"
#include <memory>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual ~GameState();

	virtual void Draw();
	virtual bool Update(float dt);
	virtual bool HandleEvent(WPARAM btnState, int x, int y);
	
private:
	std::unique_ptr<World> mWorld;

};
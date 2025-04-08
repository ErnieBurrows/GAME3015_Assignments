#pragma once

#include "State.h"

class MainMenuState : public State
{
public:
	MainMenuState(StateStack& stack, Context context);

	virtual void Draw();
	virtual bool Update(const GameTimer& gt);
	virtual bool HandleEvent(WPARAM btnState, int x, int y);
};

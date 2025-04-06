#pragma once

#include "State.h"
#include <iostream>

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context) : State(stack, context)
	{
		std::cout << "PauseState: Initialized.\n";
	}

	virtual ~PauseState()
	{
		std::cout << "PauseState: Destroyed.\n";
	}

	virtual void Draw()
	{
		std::cout << "PauseState: Drawing...\n";
	}

	virtual bool Update(float dt)
	{
		std::cout << "PauseState: Updating...\n";
		return true;
	}

	virtual bool HandleEvent(WPARAM btnState, int x, int y)
	{
		std::cout << "PauseState: Handling event...\n";
		if (btnState != 0)
		{
			std::cout << "PauseState: Key pressed. Resuming game.\n";
			RequestStackPop();
		}

		return false;
	}
};

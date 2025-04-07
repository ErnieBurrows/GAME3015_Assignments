#pragma once

#include "State.h"
#include <iostream>

class MainMenuState : public State
{
public:
	MainMenuState(StateStack& stack, Context context) : State(stack, context)
	{
		std::cout << "MainMenuState: Initialized.\n";
	}

	virtual void Draw()
	{
		std::cout << "MainMenuState: Drawing...\n";
	}

	virtual bool Update(float dt)
	{
		std::cout << "MainMenuState: Updating...\n";
		return true;
	}

	virtual bool HandleEvent(WPARAM btnState)
	{
		std::cout << "MainMenuState: Handling event...\n";

		// Simulate a key press to transition to the game state
		if (btnState != 0)
		{
			std::cout << "MainMenuState: Key pressed. Transitioning to Game State.\n";
			RequestStackPop();

			// Assuming 2 is the ID for Game state
			RequestStackPush(2);
		}
		return true;
	}
};

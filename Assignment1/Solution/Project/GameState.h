#pragma once

#include "State.h"
#include <iostream>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context) : State(stack, context)
	{
		std::cout << "GameState: Initialized.\n";

		Game* gameInstance = GetContext().game;
		if (gameInstance)
		{
			std::cout << "GameState: Game instance is valid.\n";
		}
		else
		{
			std::cout << "GameState: Game instance is null.\n";
		}
	}

	virtual ~GameState()
	{
		std::cout << "GameState: Destroyed.\n";
	}

	virtual void Draw()
	{
		std::cout << "GameState: Drawing...\n";
	}

	virtual bool Update(float dt)
	{
		std::cout << "GameState: Updating...\n";

		return true;
	}
	virtual bool HandleEvent(WPARAM btnState, int x, int y)
	{
		std::cout << "GameState: Handling event...\n";
		if (btnState != 0)
		{
			std::cout << "GameState: Key pressed. Pause requested\n";

			RequestStackPush(3);
		}
		return true;
	}

private:
	//Todo: reference world
};
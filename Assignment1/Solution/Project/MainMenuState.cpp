#include "MainMenuState.h"
#include <iostream>

MainMenuState::MainMenuState(StateStack& stack, Context context) : State(stack, context) 
{
	std::cout << "MainMenuState: Initialized.\n";
}

void MainMenuState::Draw()
{
	std::cout << "MainMenuState: Drawing...\n";
}

bool MainMenuState::Update(const GameTimer& gt)
{
	std::cout << "MainMenuState: Updating...\n";
	return true;
}

bool MainMenuState::HandleEvent(WPARAM btnState, int x, int y)
{
	std::cout << "MainMenuState: Handling event...\n";

	if (btnState != 0)
	{
		std::cout << "MainMenuState: Key pressed. Transitioning to Game State.\n";
		RequestStackPop();

		RequestStackPush(2);
	}
	return true;
}

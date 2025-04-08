#include "PauseState.h"
#include <iostream>

PauseState::PauseState(StateStack& stack, Context context) : State(stack, context)
{
	std::cout << "PauseState: Initialized.\n";
}

PauseState::~PauseState()
{
	std::cout << "PauseState: Destroyed.\n";
}

void PauseState::Draw()
{
	std::cout << "PauseState: Drawing...\n";
}

bool PauseState::Update(const GameTimer& gt)
{
	std::cout << "PauseState: Updating...\n";
	return false;
}

bool PauseState::HandleEvent(WPARAM btnState, int x, int y)
{
	std::cout << "PauseState: Handling event...\n";
	if (btnState != 0)
	{
		std::cout << "PauseState: Key pressed. Resuming game.\n";
		RequestStackPop();
	}

	return false;
}
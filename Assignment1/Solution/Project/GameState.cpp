#include "GameState.h"
#include "Game.hpp"
#include <iostream>

GameState::GameState(StateStack& stack, Context context) : State(stack, context)
{
	mWorld = std::make_unique<World>(GetContext().game);

	mWorld->BuildScene();

	std::cout << "GameState: Initialized.\n";
}

GameState::~GameState()
{
	std::cout << "GameState: Destroyed.\n";
}

void GameState::Draw()
{
	mWorld->Draw();
}

bool GameState::Update(float dt)
{
	GameTimer gt;
	mWorld->Update(gt);

	return true;
}

bool GameState::HandleEvent(WPARAM btnState)
{
	std::cout << "GameState: Handling event...\n";
	if (btnState != 0)
	{
		std::cout << "GameState: Key pressed. Pause requested\n";

		RequestStackPush(3);
	}
	return true;
}

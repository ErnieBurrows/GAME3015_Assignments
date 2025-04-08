#include "TitleState.h"

TitleState::TitleState(StateStack& stack, Context context) : State(stack, context), mElapsedTime(0.0f)
{
	std::cout << "TitleState: Initialized. (Press any key to continue...)\n";
}

void TitleState::Draw()
{
	std::cout << "TitleState: Drawing...\n";
}

bool TitleState::Update(const GameTimer& gt)
{
	//mElapsedTime += gt.DeltaTime();

	if (mElapsedTime > 2.0f)
	{
		std::cout << "TitleState: 2 seconds elapsed. Transitioning to Main Menu.\n";

		RequestStackPop();

		RequestStackPush(1);
	}
	return true;
}

bool TitleState::HandleEvent(WPARAM btnState, int x, int y)
{

	if (btnState != 0)
	{
		std::cout << "TitleState: Key pressed. Transitioning to Main Menu.\n";
		RequestStackPop();

		RequestStackPush(1);
	}
	return true;
}

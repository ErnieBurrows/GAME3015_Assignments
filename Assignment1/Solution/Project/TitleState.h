#pragma once
#include "State.h"
#include <iostream>

class TitleState : public State
{
public: 
	TitleState(StateStack& stack, Context context) : State(stack, context), mElapsedTime(0.0f)
	{
		std::cout << "TitleState: Initialized. (Press any key to continue...)\n";
	}

	virtual void Draw()
	{
		std::cout << "TitleState: Drawing...\n";
	}

	virtual bool Update(float dt)
	{
		mElapsedTime += dt;

		if (mElapsedTime > 2.0f)
		{
			std::cout << "TitleState: 2 seconds elapsed. Transitioning to Main Menu.\n";

			RequestStackPop();

			// Assuming 1 is the ID for Main Menu state
			RequestStackPush(1); 
		}
		return true;
	}

	virtual bool HandleEvent(WPARAM btnState)
	{
		// If a key is pressed, transition to the main menu
		if (btnState != 0)
		{
			std::cout << "TitleState: Key pressed. Transitioning to Main Menu.\n";
			RequestStackPop();

			// Assuming 1 is the ID for Main Menu state
			RequestStackPush(1);
		}
		return true;
	}

private:
	float mElapsedTime;
};

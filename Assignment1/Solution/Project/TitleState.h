#pragma once
#include "State.h"
#include <iostream>

class TitleState : public State
{
public: 
	TitleState(StateStack& stack, Context context);

	virtual void Draw();
	virtual bool Update(const GameTimer& gt);
	virtual bool HandleEvent(WPARAM btnState, int x, int y);
	
private:
	float mElapsedTime;
};

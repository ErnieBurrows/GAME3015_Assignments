#include "State.h"
#include "StateStack.h"

void State::RequestStackPush(int stateID)
{
	mStack->PushState(stateID);
}

void State::RequestStackPop()
{
	mStack->PopState();
}

void State::RequestStackClear()
{
	mStack->ClearStates();
}

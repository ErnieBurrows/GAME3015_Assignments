#include "State.h"
#include "StateStack.h"

State::Context::Context(Game* _game, Player* _player) : game(_game), player(_player)
{
}

State::State(StateStack& stack, Context context) : mStack(&stack)
, mContext(context), mSceneGraph(std::make_unique<SceneNode>(this)) 
{ 
}

State::~State() { }

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



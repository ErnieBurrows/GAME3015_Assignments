#include "StateStack.h"
#include <cassert>
#include "Game.hpp"

StateStack::StateStack(State::Context context) : mStack()
, mPendingList()
, mContext(context)
, mFactories()
{

}

void StateStack::Update(const GameTimer& timer)
{
	// Upate from the top state downward until one returns false
	for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
	{
		if (!(*i)->Update(timer))
			break;
	}
	ApplyPendingChanges();
}

void StateStack::Draw()
{
	for (State::Ptr& state : mStack)
	{
		state->Draw();
	}
}

void StateStack::HandleEvent(WPARAM btnState)
{
	for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
	{
		if (!(*i)->HandleEvent(btnState))
			break;
	}
}

void StateStack::HandleRealTimeInput()
{
	for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
	{
		if (!(*i)->HandleRealTimeInput())
			break;
	}
}

void StateStack::PushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::PopState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::ClearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::IsEmpty() const
{
	return mStack.empty();
}

State* StateStack::GetCurrentState()
{
	return mStack.back().get();
}

State* StateStack::GetPreviousState()
{
	if (mStack.size() >= 2)
	{
		return mStack[mStack.size() - 2].get();
	}
	return nullptr;  
}

State::Ptr StateStack::CreateState(States::ID stateID)
{
	auto found = mFactories.find(stateID);

	assert(found != mFactories.end());

	return found->second();
}

void StateStack::ApplyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(CreateState(change.stateID));
			break;

		case Pop:
			if (!mStack.empty())
				mStack.pop_back();
			break;

		case Clear:
			mStack.clear();
			break;
		}

	}
	// Clear the pending changes after applying them
	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) : action(action), stateID(stateID)
{
}

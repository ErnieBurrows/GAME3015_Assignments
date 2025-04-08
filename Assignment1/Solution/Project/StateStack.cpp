#include "StateStack.h"

void StateStack::Update(const GameTimer& gt)
{
	// Upate from the top state downward until one returns false
	for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
	{
		if (!(*i)->Update(gt))
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

void StateStack::HandleEvent(WPARAM btnState, int x, int y)
{
	for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
	{
		if (!(*i)->HandleEvent(btnState, x, y))
			break;
	}
	ApplyPendingChanges();
}

void StateStack::PushState(int stateID)
{
	mPendingList.push_back({ Push, stateID });
}

void StateStack::PopState()
{
	mPendingList.push_back({ Pop, 0 });
}

void StateStack::ClearStates()
{
	mPendingList.push_back({ Clear, 0 });
}

bool StateStack::IsEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::CreateState(int stateID)
{
	auto found = mFactories.find(stateID);

	if (found == mFactories.end())
	{
		throw std::runtime_error("State not registered");
	}

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

#pragma once
#include "State.h"

#include <vector>
#include <functional>
#include <map>
#include <stdexcept>

class StateStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear
	};

	explicit StateStack(State::Context context) : mContext(context) {}

	template<typename T>
	void RegisterState(int stateID)
	{
		mFactories[stateID] = [this]() 
		{
			return State::Ptr(new T(*this, mContext));
		};
	}

	void Update(const GameTimer& gt)
	{
		// Upate from the top state downward until one returns false
		for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
		{
			if (!(*i)->Update(gt))
				break;
		}
		ApplyPendingChanges();
	}

	void Draw()
	{
		for (State::Ptr& state : mStack)
		{
			state->Draw();
		}
	}

	void HandleEvent(WPARAM btnState, int x, int y)
	{
		for (auto i = mStack.rbegin(); i != mStack.rend(); ++i)
		{
			if (!(*i)->HandleEvent(btnState, x, y))
				break;
		}
		ApplyPendingChanges();
	}

	void PushState(int stateID)
	{
		mPendingList.push_back({Push, stateID});
	}

	void PopState()
	{
		mPendingList.push_back({Pop, 0});
	}

	void ClearStates()
	{
		mPendingList.push_back({ Clear, 0 });
	}

	bool IsEmpty() const
	{
		return mStack.empty();
	}

private:
	struct PendingChange
	{
		Action action;
		int stateID;
	};

	State::Ptr CreateState(int stateID)
	{
		auto found = mFactories.find(stateID);

		if (found == mFactories.end())
		{
			throw std::runtime_error("State not registered");
		}

		return found->second();
	}

	void ApplyPendingChanges()
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

	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	State::Context mContext;
	std::map<int, std::function<State::Ptr()>> mFactories;
};


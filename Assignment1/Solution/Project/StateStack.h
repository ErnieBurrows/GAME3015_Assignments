#pragma once
#include "State.h"

#include <vector>
#include <functional>
#include <utility>
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

	explicit StateStack(State::Context context);

	template<typename T>
	void RegisterState(int stateID);


	void Update(const GameTimer& timer);
	void Draw();
	void HandleEvent(WPARAM btnState);
	void HandleRealTimeInput();

	void PushState(States::ID stateID);
	void PopState();
	void ClearStates();
	
	bool IsEmpty() const;

	State* GetCurrentState();
	State* GetPreviousState();

private:
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);

		Action action;
		States::ID stateID;
	};

	State::Ptr CreateState(States::ID stateID);
	void ApplyPendingChanges();
	
private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	State::Context mContext;

	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template<typename T>
inline void StateStack::RegisterState(int stateID)
{
	mFactories[stateID] = [this]()
		{
			return State::Ptr(new T(*this, mContext));
		};
}


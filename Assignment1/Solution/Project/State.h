#pragma once
#include <memory>
#include<Windows.h>

class StateStack;

class Game;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	// Context struct to pass information and pointers to each state
	struct Context
	{
		Game* game;
	};

	State(StateStack& stack, Context context) : mStack(&stack), mContext(context) {}

	virtual ~State() {}

	virtual void Draw() = 0;
	virtual bool Update(float dt) = 0;
	virtual bool HandleEvent(WPARAM btnState, int x, int y) = 0;

protected:
	void RequestStackPush(int stateID);
	void RequestStackPop();
	void RequestStackClear();

	Context GetContext() const { return mContext; }

private:
	StateStack* mStack;
	Context mContext;
};


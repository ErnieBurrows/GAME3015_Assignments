#pragma once
#include <queue>
#include "Command.h"

class CommandQueue
{
public: 
	void push(const Command& command) { mQueue.push(command); }

	Command pop() {
		Command cmd = mQueue.front();
		mQueue.pop();
		return cmd;
	}

	bool isEmpty() const { return mQueue.empty(); }
private:
	std::queue<Command> mQueue;
};


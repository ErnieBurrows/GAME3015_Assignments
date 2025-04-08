#pragma once
#include "CommandQueue.hpp"
#include <map>	

class CommandQueue;

class Player
{
public:

	Player();

	void HandleEvent(CommandQueue& commands);

	void HandeRealTimeInput(CommandQueue& commands);

	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};

	void AssignKey(Action action, char key);

	char getAssignedKey(Action action) const;

private:
	
	void InitializeActions();

	static bool isRealTimeAction(Action action);

private:
	std::map<char, Action>		mKeyBinding; 
	std::map<Action, Command>	mActionBinding; 
	
};




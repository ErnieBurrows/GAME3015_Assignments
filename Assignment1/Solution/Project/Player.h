#pragma once
#include <map>
#include "Command.h"
#include "CommandQueue.h"
#include "SceneNode.hpp"

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Shoot
	};

	Player();

	void HandleEvent(CommandQueue& commands);

	void HandleRealtimeInput(CommandQueue& commands);

	void AssignKey(Action action, int key);
	
	int GetAssignedKey(Action action) const;

private:
	static bool isRealtimeAction(Action action);

	// Used for maping a specific key to a specific action i.e VK_LEFT to MoveLeft
	std::map<int, Action> mKeyBinding;

	// Used for mapping a specific action to a specific command i.e MoveLeft to MoveLeftCommand
	std::map<Action, Command> mActionBinding;



};


#pragma once
#include "Command.h"
#include "SceneNode.hpp"
#include "../../Common/GameTimer.h"
#include "Category.h"


namespace Actions
{
	/* ----- PLAYER ACTIONS ----- */
	inline Command CreateMoveLeftCommand(float moveSpeed, Category::Type category)
	{
		Command command;

		command.category = category;

		command.action = [moveSpeed](SceneNode& node, const GameTimer& dt)
			{
				node.Move(-moveSpeed * dt.DeltaTime(), 0.0f, 0.0f);
			};
		return command;
	}

	inline Command CreateMoveRightCommand(float moveSpeed, Category::Type category)
	{
		Command command;

		command.category = category;

		command.action = [moveSpeed](SceneNode& node, const GameTimer& dt)
			{
				node.Move(moveSpeed * dt.DeltaTime(), 0.0f, 0.0f);
			};
		return command;
	}

	inline Command CreateMoveUpCommand(float moveSpeed, Category::Type category)
	{
		Command command;

		command.category = category;

		command.action = [moveSpeed](SceneNode& node, const GameTimer& dt)
			{
				node.Move(0.0f, moveSpeed * dt.DeltaTime(), 0.0f);
			};
		return command;
	}

	inline Command CreateMoveDownCommand(float moveSpeed, Category::Type category)
	{
		Command command;

		command.category = category;

		command.action = [moveSpeed](SceneNode& node, const GameTimer& dt)
			{
				node.Move(0.0f, -moveSpeed * dt.DeltaTime(), 0.0f);
			};
		return command;
	}


}

#include "Player2.h"
#include <Windows.h>
#include "Actions.h"

Player2::Player2()
{
	moveSpeed = 10.0f;

	mKeyBinding[VK_LEFT] = MoveLeft;
	mKeyBinding[VK_RIGHT] = MoveRight;
	mKeyBinding[VK_UP] = MoveUp;
	mKeyBinding[VK_DOWN] = MoveDown;

	mActionBinding[MoveLeft] = Actions::CreateMoveLeftCommand(moveSpeed, Category::Player2Aircraft);
	mActionBinding[MoveRight] = Actions::CreateMoveRightCommand(moveSpeed, Category::Player2Aircraft);
	mActionBinding[MoveUp] = Actions::CreateMoveUpCommand(moveSpeed, Category::Player2Aircraft);
	mActionBinding[MoveDown] = Actions::CreateMoveDownCommand(moveSpeed, Category::Player2Aircraft);

	// Make sure the commands ONLY affect the player aircraft
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::Player2Aircraft;
	}
}

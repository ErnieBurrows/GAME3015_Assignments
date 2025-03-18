#include "Player.h"
#include "Windows.h"
#include "Actions.h"

Player::Player()
{
	moveSpeed = 10.0f;

	mKeyBinding['A'] = MoveLeft;
	mKeyBinding['D'] = MoveRight;
	mKeyBinding['W'] = MoveUp;
	mKeyBinding['S'] = MoveDown;

	mActionBinding[MoveLeft] = Actions::CreateMoveLeftCommand(moveSpeed, Category::PlayerAircraft);
	mActionBinding[MoveRight] = Actions::CreateMoveRightCommand(moveSpeed, Category::PlayerAircraft);
	mActionBinding[MoveUp] = Actions::CreateMoveUpCommand(moveSpeed, Category::PlayerAircraft);
	mActionBinding[MoveDown] = Actions::CreateMoveDownCommand(moveSpeed, Category::PlayerAircraft);

	// Make sure the commands ONLY affect the player aircraft
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerAircraft;
	}
}
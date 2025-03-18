#include "Player.h"
#include "Windows.h"

const float playerMoveSpeed = 30.0f;

Player::Player()
{
	// Set up default key bindings
	mKeyBinding[VK_LEFT] = MoveLeft;
	mKeyBinding[VK_RIGHT] = MoveRight;
	mKeyBinding[VK_UP] = MoveUp;
	mKeyBinding[VK_DOWN] = MoveDown;

	// Associate actions with commands.
	mActionBinding[MoveLeft].action = [](SceneNode& node, const GameTimer& dt)
		{
			node.Move(-playerMoveSpeed * dt.DeltaTime(), 0.0f, 0.0f);
		};
	mActionBinding[MoveRight].action = [](SceneNode& node, const GameTimer& dt)
		{
			node.Move(playerMoveSpeed * dt.DeltaTime(), 0.0f, 0.0f);
		};
	mActionBinding[MoveUp].action = [](SceneNode& node, const GameTimer& dt)
		{
			node.Move(0.0f, playerMoveSpeed * dt.DeltaTime(), 0.0f);
		};
	mActionBinding[MoveDown].action = [](SceneNode& node, const GameTimer& dt)
		{
			node.Move(0.0f, -playerMoveSpeed * dt.DeltaTime(), 0.0f);
		};

	// Make sure the commands ONLY affect the player aircraft
	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerAircraft;
	}
}

/// <summary>
/// Used to handle events that are not real time i.e key presses instead of per frame key checking
/// </summary>
/// <param name="commands"></param>
void Player::HandleEvent(CommandQueue& commands)
{
}

void Player::HandleRealtimeInput(CommandQueue& commands)
{
	// Iterate through all key bindings and push the associated command to the command queue
	for (const auto& pair : mKeyBinding)
	{
		if (GetAsyncKeyState(pair.first) & 0x8000)
		{
			OutputDebugStringA("Key pressed!\n");

			if (isRealtimeAction(pair.second))
			{
				commands.push(mActionBinding[pair.second]);
			}
		}
	}
}

void Player::AssignKey(Action action, int key)
{
	// Remove any existing mapping for this action to avoid duplicates.
	for (auto i = mKeyBinding.begin(); i != mKeyBinding.end();)
	{
		if (i->second == action)
		{
			mKeyBinding.erase(i);
		}
		else
		{
			++i;;
		}
	}

	mKeyBinding[key] = action;
}

int Player::GetAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
		{
			return pair.first;
		}
	}

	return -1;
}

bool Player::isRealtimeAction(Action action)
{
	// Todo: Define which actions are real time
	return true;
}

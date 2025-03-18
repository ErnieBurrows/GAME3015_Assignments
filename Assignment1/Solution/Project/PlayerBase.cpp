#include "PlayerBase.h"
#include "Windows.h"
#include "Actions.h"



PlayerBase::PlayerBase()
{
	
}

/// <summary>
/// Used to handle events that are not real time i.e key presses instead of per frame key checking
/// </summary>
/// <param name="commands"></param>
void PlayerBase::HandleEvent(CommandQueue& commands)
{
}

void PlayerBase::HandleRealtimeInput(CommandQueue& commands)
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

void PlayerBase::AssignKey(Action action, int key)
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

int PlayerBase::GetAssignedKey(Action action) const
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

bool PlayerBase::isRealtimeAction(Action action)
{
	// Todo: Define which actions are real time
	return true;
}

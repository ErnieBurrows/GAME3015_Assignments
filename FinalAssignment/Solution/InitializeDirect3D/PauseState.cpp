#include "PauseState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include "GameState.hpp"
#include <windows.h>

PauseState::PauseState(StateStack* stack, Context* context)
    : State(stack, context)
{

}


PauseState::~PauseState()
{
}

void PauseState::Draw()
{
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->draw();
}


bool PauseState::Update(const GameTimer& gt)
{
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->update(gt);

    return false;
}

bool PauseState::HandleEvent(WPARAM btnState)
{
    if (d3dUtil::IsKeyDown(VK_RETURN))
    {
        RequestStackPop();
    }
    if (d3dUtil::IsKeyDown(VK_ESCAPE))
    {
        RequestStateClear();
        RequestStackPush(States::Menu);
    }
    return false;
}


bool PauseState::HandleRealTimeInput()
{
    return false;
}


void PauseState::CheckPause(const GameTimer& gt)
{
    if (d3dUtil::IsKeyDown('P'))
    {
        showPause = true;
    }
    if (d3dUtil::IsKeyDown('Q'))
    {
        RequestStackPop();
        RequestStackPush(States::Menu);
    }
    if (d3dUtil::IsKeyDown(VK_BACK))
    {
        showPause = false;

    }

}
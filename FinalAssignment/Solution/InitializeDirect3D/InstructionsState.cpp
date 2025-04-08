#include "InstructionsState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

InstructionsState::InstructionsState(StateStack* stack, Context* context)
    : State(stack, context)
{
    mAllRitems.clear();
    mContext->game->ResetFrameResources();
    mContext->game->BuildMaterials();


    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
    backgroundSprite->SetDrawName("Desert", "boxGeo", "box");
    backgroundSprite->setScale(10.0, 1.0, 7.0);
    backgroundSprite->setPosition(0, 0, 0);
    mSceneGraph->attachChild(std::move(backgroundSprite));

    std::unique_ptr<SpriteNode> InstructionsSprite = std::make_unique<SpriteNode>(this);
    InstructionsSprite->SetDrawName("Instructions", "boxGeo", "box");
    InstructionsSprite->setScale(7.0, 1.0, 5.0);
    InstructionsSprite->setPosition(0, 1, 0);
    mSceneGraph->attachChild(std::move(InstructionsSprite));


    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}


InstructionsState::~InstructionsState()
{
}

void InstructionsState::Draw()
{
    mSceneGraph->draw();
}


bool InstructionsState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    return true;
}


bool InstructionsState::HandleEvent(WPARAM btnState)
{
    if (d3dUtil::IsKeyDown(VK_BACK))
    {
        RequestStackPop();
        RequestStackPush(States::Menu);

    }
    return true;
}

bool InstructionsState::HandleRealTimeInput()
{
    return true;
}
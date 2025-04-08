#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

TitleState::TitleState(StateStack* stack, Context* context)
    : State(stack, context)
    , mFlashTimer(0.0f)
    , mIsVisible(true)
{
    mAllRitems.clear();
    mContext->game->ResetFrameResources();
    mContext->game->BuildMaterials();

    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
    backgroundSprite->SetDrawName("Desert", "boxGeo", "box");
    backgroundSprite->setScale(10.0, 1.0, 7.0);
    backgroundSprite->setPosition(0, 0, 0);
    mSceneGraph->attachChild(std::move(backgroundSprite));

    std::unique_ptr<SpriteNode> TSSprite = std::make_unique<SpriteNode>(this);
    TSSprite->SetDrawName("StartPrompt", "boxGeo", "box");
    TSSprite->setScale(5.0, 1.0, 2.0);
    TSSprite->setPosition(0, 1, -0.25);
    mSceneGraph->attachChild(std::move(TSSprite));

    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

TitleState::~TitleState()
{
}

void TitleState::Draw()
{
    mSceneGraph->draw();
}

bool TitleState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    mFlashTimer += gt.DeltaTime();

    if (mFlashTimer >= 0.25f)
    {
        mFlashTimer = 0.0f; 
        mIsVisible = !mIsVisible; 
    }

    for (auto& node : mSceneGraph->getChildren())
    {
        if (auto spriteNode = dynamic_cast<SpriteNode*>(node.get()))
        {
            if (spriteNode->GetDrawName() == "StartPrompt")
            {
                spriteNode->SetVisible(mIsVisible);
                break;  
            }
        }
    }
    return true;
}

bool TitleState::HandleEvent(WPARAM btnState)
{
    RequestStackPop();

    RequestStackPush(States::Menu);

    return true;
}

bool TitleState::HandleRealTimeInput()
{
    return true;
}
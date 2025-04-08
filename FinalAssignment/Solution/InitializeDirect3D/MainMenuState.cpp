#include "MainMenuState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

MainMenuState::MainMenuState(StateStack* stack, Context* context)
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

    std::unique_ptr<SpriteNode> MainMenuPromptSprite = std::make_unique<SpriteNode>(this);
    MainMenuPromptSprite->SetDrawName("MainMenuPrompt", "boxGeo", "box");
    MainMenuPromptSprite->setScale(5, 1.0, 5);
    MainMenuPromptSprite->setPosition(0, 1, -0.5);
    mSceneGraph->attachChild(std::move(MainMenuPromptSprite));

    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Draw()
{
    mSceneGraph->draw();
}

bool MainMenuState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    return true;
}

bool MainMenuState::HandleEvent(WPARAM btnState)
{
    if (d3dUtil::IsKeyDown(VK_RETURN))
    {
        RequestStackPop();
        RequestStackPush(States::Game);
    }
    else if (d3dUtil::IsKeyDown('I'))
    {
        RequestStackPop();
        RequestStackPush(States::Instructions);
    }
    else if (d3dUtil::IsKeyDown(VK_ESCAPE))
    {
        PostQuitMessage(0);
    }
    return true;
}

bool MainMenuState::HandleRealTimeInput()
{
    return true;
}
#include "World.hpp"
#include <iostream>

World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-1.5f, 1.5f, 200.0f, 0.0f) //Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

void World::Update(const GameTimer& gt)
{
	mSceneGraph->Update(gt);
}

void World::Draw()
{
	mSceneGraph->Draw();
}

void World::BuildScene()
{
    std::cout << "Building Scene..." << std::endl;

    std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
    mPlayerAircraft = player.get();
    mPlayerAircraft->SetPosition(0, 0.1, 0.0);
    mSceneGraph->AttachChild(std::move(player));


    //std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
    //auto raptor = enemy1.get();
    //raptor->SetPosition(0.5, 0, 1);
    //mPlayerAircraft->AttachChild(std::move(enemy1));

    //std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
    //auto raptor2 = enemy2.get();
    //raptor2->SetPosition(-0.5, 0, 1);
    //mPlayerAircraft->AttachChild(std::move(enemy2));

    mSceneGraph->Build();
}


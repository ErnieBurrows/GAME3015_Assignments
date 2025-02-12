#include "World.hpp"

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
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->SetPosition(0, 0.1, 0.0);
	mPlayerAircraft->SetScale(0.5, 0.5, 0.5);
	mPlayerAircraft->SetVelocity(mScrollSpeed, 0.0, 0.0);
	mSceneGraph->AttachChild(std::move(player));

	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor = enemy1.get();
	raptor->SetPosition(0.5, 0, 1);
	raptor->SetScale(1.0, 1.0, 1.0);
	raptor->SetWorldRotation(0, XM_PI, 0);
	mPlayerAircraft->AttachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor2 = enemy2.get();
	raptor2->SetPosition(-0.5, 0, 1);
	raptor2->SetScale(1.0, 1.0, 1.0);
	raptor2->SetWorldRotation(0, XM_PI, 0);
	mPlayerAircraft->AttachChild(std::move(enemy2));

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	//mBackground->setPosition(mWorldBounds.left, mWorldBounds.top);
	mBackground->SetPosition(0, 0, 0.0);
	mBackground->SetScale(10.0, 1.0, 200.0);
	//mBackground->setVelocity(0, 0, -mScrollSpeed);
	mSceneGraph->AttachChild(std::move(backgroundSprite));

	mSceneGraph->Build();
}

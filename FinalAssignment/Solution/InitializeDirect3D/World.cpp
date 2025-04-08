#define NOMINMAX
#include "World.hpp"


World::World(State* state)
	: mSceneGraph(new SceneNode(state))
	, mState(state)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-3.25f, 3.25f, -1.5f, 2.5f) 
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

#pragma region Step 14
CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}
#pragma endregion

void World::update(const GameTimer& gt)
{
#pragma region Step 15
	mPlayerAircraft->setVelocity(0.0f, 0.0f, 0.0f);
	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);

	PlayerPosition();
	mSceneGraph->update(gt);
	PlayerVelocity();

#pragma endregion

}

void World::draw()
{
	mSceneGraph->draw();
}


void World::buildScene()
{
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mState));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, 0.0);
	mPlayerAircraft->setScale(0.002, 0.002, 0.002);
	mPlayerAircraft->setWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	mPlayerAircraft->setVelocity(mScrollSpeed, 0.0, 0.0);
	mSceneGraph->attachChild(std::move(player));

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mState));
	backgroundSprite->SetDrawName("Desert", "boxGeo", "box");
	mBackground = backgroundSprite.get();
	mBackground->setPosition(0, 0, 0.0);
	mBackground->setScale(10.0, 1.0, 7.0);
	mBackground->setVelocity(0, 0, -mScrollSpeed); 
	mSceneGraph->attachChild(std::move(backgroundSprite));

	std::unique_ptr<SpriteNode> InstructionSprite(new SpriteNode(mState));
	InstructionSprite->SetDrawName("GameText", "boxGeo", "box");
	//mBackground = backgroundSprite.get();
	//mBackground->setPosition(mWorldBounds.left, mWorldBounds.top);
	InstructionSprite->setPosition(0, 1.0, 2.4);
	InstructionSprite->setScale(2.0, 0, 2.0);
	InstructionSprite->setVelocity(0, 0, 0);
	mSceneGraph->attachChild(std::move(InstructionSprite));


	mSceneGraph->build();
}

/**
 * @brief Adjusts player position to stay within world bounds
 */
void World::PlayerPosition()
{
	const float borderDistance = 10.f;

	XMFLOAT3 position = mPlayerAircraft->getWorldPosition();
	position.x = std::max(position.x, mWorldBounds.x);
	position.x = std::min(position.x, mWorldBounds.y);
	position.z = std::max(position.z, mWorldBounds.z);
	position.z = std::min(position.z, mWorldBounds.w);
	mPlayerAircraft->setPosition(position.x, position.y, position.z);
}


void World::PlayerVelocity()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0.0f && velocity.z != 0.0f)
	{
		mPlayerAircraft->setVelocity(velocity.x / std::sqrt(2.f), velocity.y / std::sqrt(2.f), velocity.z / std::sqrt(2.f));
	}
}

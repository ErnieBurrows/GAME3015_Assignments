#include "SceneNode.hpp"
#include "Game.hpp"

SceneNode::SceneNode(Game* game)
	: mChildren()
	, mParent(nullptr)
	, game(game)
{
	mWorldPosition = XMFLOAT3(0, 0, 0);
	mWorldScaling = XMFLOAT3(1, 1, 1);
	mWorldRotation = XMFLOAT3(0, 0, 0);
}

void SceneNode::AttachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::DetachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::Update(const GameTimer& gt)
{
	UpdateCurrent(gt);
	UpdateChildren(gt);
}

void SceneNode::UpdateCurrent(const GameTimer& gt)
{
	// Do nothing by default
}

void SceneNode::UpdateChildren(const GameTimer& gt)
{
	for (Ptr& child : mChildren)
	{
		child->Update(gt);
	}
}

void SceneNode::Draw() const
{
	DrawCurrent();
	DrawChildren();
}

void SceneNode::DrawCurrent() const
{
	//Empty for now
}

void SceneNode::DrawChildren() const
{
	for (const Ptr& child : mChildren)
	{
		child->Draw();
	}
}

void SceneNode::Build()
{
	BuildCurrent();
	BuildChildren();
}

void SceneNode::BuildCurrent()
{
	//Empty for now
}

void SceneNode::BuildChildren()
{
	for (const Ptr& child : mChildren)
	{
		child->Build();
	}
}

void SceneNode::OnCommand(const Command& command, const GameTimer& dt)
{
	// Check if this node should respond to the command.
	if (command.category & GetCategory())
	{
		command.action(*this, dt);
	}

	// Pass the command to children
	for (const Ptr& child : mChildren)
	{
		child->OnCommand(command, dt);
	}
}

XMFLOAT3 SceneNode::GetWorldPosition() const
{
	return mWorldPosition;
}

void SceneNode::SetPosition(float x, float y, float z)
{
	mWorldPosition = XMFLOAT3(x, y, z);
}

XMFLOAT3 SceneNode::GetWorldRotation() const
{
	return mWorldRotation;
}

void SceneNode::SetWorldRotation(float x, float y, float z)
{
	mWorldRotation = XMFLOAT3(x, y, z);
}

XMFLOAT3 SceneNode::GetWorldScale() const
{
	return mWorldScaling;
}

void SceneNode::SetScale(float x, float y, float z)
{
	mWorldScaling = XMFLOAT3(x, y, z);
}

XMFLOAT4X4 SceneNode::GetWorldTransform() const
{
	XMFLOAT4X4 transform = MathHelper::Identity4x4();
	XMMATRIX T = XMLoadFloat4x4(&transform);

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		auto nodeTransform = node->GetTransform();
		XMMATRIX Tp = XMLoadFloat4x4(&nodeTransform);
		T = Tp * T;
	}
	XMStoreFloat4x4(&transform, T);

	return transform;
}

XMFLOAT4X4 SceneNode::GetTransform() const
{
	XMFLOAT4X4 transform;
	XMStoreFloat4x4(&transform, XMMatrixScaling(mWorldScaling.x, mWorldScaling.y, mWorldScaling.z) *
		XMMatrixRotationX(mWorldRotation.x) *
		XMMatrixRotationY(mWorldRotation.y) *
		XMMatrixRotationZ(mWorldRotation.z) *
		XMMatrixTranslation(mWorldPosition.x, mWorldPosition.y, mWorldPosition.z));
	return transform;
}

void SceneNode::Move(float x, float y, float z)
{
	mWorldPosition.x += x;
	mWorldPosition.y += y;
	mWorldPosition.z += z;
}

void SceneNode::Rotate(float x, float y, float z)
{
	XMFLOAT3 rot = GetWorldRotation();
	SetWorldRotation(rot.x + x, rot.y + y, rot.z + z);
}

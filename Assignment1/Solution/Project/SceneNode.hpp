#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"
#include "Command.h"
#include "Category.h"
#include "FrameResource.h"
#include "RenderItem.h"


using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.



class Game;


class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode(Game* game);

	void AttachChild(Ptr child);
	Ptr	DetachChild(const SceneNode& node);

	void Update(const GameTimer& gt);
	void Draw() const;
	void Build();

	XMFLOAT3 GetWorldPosition() const;
	void SetPosition(float x, float y, float z);
	XMFLOAT3 GetWorldRotation() const;
	void SetWorldRotation(float x, float y, float z);
	XMFLOAT3 GetWorldScale() const;
	void SetScale(float x, float y, float z);

	XMFLOAT4X4 GetWorldTransform() const;
	XMFLOAT4X4 GetTransform() const;

	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);

	const std::vector<Ptr>& GetChildren() const { return mChildren; }

	RenderItem* GetRenderer() const { return renderer; }

private:
	virtual void UpdateCurrent(const GameTimer& gt);
	void UpdateChildren(const GameTimer& gt);

	virtual void DrawCurrent() const;
	void DrawChildren() const;
	virtual void BuildCurrent();
	void BuildChildren();


protected:
	Game* game;
	RenderItem* renderer;

private:
	XMFLOAT3 mWorldPosition;
	XMFLOAT3 mWorldRotation;
	XMFLOAT3 mWorldScaling;
	std::vector<Ptr> mChildren;
	SceneNode* mParent;

	/*--- Command Stuff ---*/
public:
	virtual unsigned int GetCategory() const { return Category::Scene; }

	void OnCommand(const Command& command, const GameTimer& dt);
};


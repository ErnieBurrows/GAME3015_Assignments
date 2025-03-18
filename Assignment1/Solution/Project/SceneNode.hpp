#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"
#include "Command.h"
#include "Category.h"
#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.
struct RenderItem
{
	RenderItem() = default;

	// World matrix of the shape that describes the object's local space
	// relative to the world space, which defines the position, orientation,
	// and scale of the object in the world.
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources;

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

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
	void OnCommand(const Command& command, const GameTimer& gt);
};


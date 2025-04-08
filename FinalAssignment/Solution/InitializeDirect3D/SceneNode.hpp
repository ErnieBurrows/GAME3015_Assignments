#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"
#include "FrameResource.h"

#include "Category.hpp"
#pragma endregion

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

struct RenderItem
{
	RenderItem() = default;

	XMFLOAT4X4 World = MathHelper::Identity4x4(); 
	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
	int NumFramesDirty = gNumFrameResources; 
	UINT ObjCBIndex = -1; 
	Material* Mat = nullptr; 
	MeshGeometry* Geo = nullptr; 

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; 

	
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

class State;
struct Command;

class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	
	SceneNode(State* state);
	void attachChild(Ptr child);
	Ptr	detachChild(const SceneNode& node);
	const std::vector<Ptr>& getChildren() const;

	void update(const GameTimer& gt);
	void draw() const;
	void build();
	XMFLOAT3 getWorldPosition() const;
	void setPosition(float x, float y, float z);
	XMFLOAT3 getWorldRotation() const;
	void setWorldRotation(float x, float y, float z);
	XMFLOAT3 getWorldScale() const;
	void setScale(float x, float y, float z);
	XMFLOAT4X4 getWorldTransform() const;
	XMFLOAT4X4 getTransform() const;
	void move(float x, float y, float z);

	void onCommand(const Command& command, const GameTimer& gt);
	virtual unsigned int getCategory() const;

private:

	virtual void updateCurrent(const GameTimer& gt);
	void updateChildren(const GameTimer& gt);
	virtual void drawCurrent() const;
	void drawChildren() const;
	virtual void buildCurrent();
	void buildChildren();

protected:
	State* mState; 
	RenderItem*	renderer; 
private:
	XMFLOAT3 mWorldPosition;
	XMFLOAT3 mWorldRotation;
	XMFLOAT3 mWorldScaling; 

	std::vector<Ptr> mChildren;
	SceneNode* mParent; 

};


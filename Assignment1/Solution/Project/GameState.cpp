#include "GameState.h"
#include "Game.hpp"
#include "World.hpp"
#include <iostream>

static const int gNumFrameResources = 3;

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context), mCurrFrameResourceIndex(0), mCurrFrameResource(nullptr)
{
	mWorld = std::make_unique<World>(GetContext().game);
	mWorld->BuildScene();
	BuildFrameResources();
	std::cout << "GameState: Initialized.\n";
}

GameState::~GameState()
{
	std::cout << "GameState: Destroyed.\n";
}

void GameState::Draw()
{
	std::cout << "GameState: Drawing...\n";
	mWorld->Draw();

	GetContext().game->UpdateMaterialCBs(mCurrFrameResource->MaterialCB.get());

	std::vector<RenderItem*> renderItems;
	mWorld->CollectRenderItems(renderItems);

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto matCB = mCurrFrameResource->MaterialCB->Resource();

	auto passCB = mCurrFrameResource->PassCB->Resource();
	GetContext().game->getCmdList()->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	for (auto ri : renderItems)
	{
		if (ri->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&ri->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&ri->TexTransform);
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));
			mCurrFrameResource->ObjectCB->CopyData(ri->ObjCBIndex, objConstants);
			ri->NumFramesDirty--;
		}

		auto cmdList = GetContext().game->getCmdList();
		auto vbv = ri->Geo->VertexBufferView();
		auto ibv = ri->Geo->IndexBufferView();

		cmdList->IASetVertexBuffers(0, 1, &vbv);
		cmdList->IASetIndexBuffer(&ibv);
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(GetContext().game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(ri->Mat->DiffuseSrvHeapIndex, GetContext().game->mCbvSrvDescriptorSize);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex * matCBByteSize;

		cmdList->SetGraphicsRootDescriptorTable(0, tex);
		cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
		cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);

		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

bool GameState::Update(float dt)
{
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	if (mCurrFrameResource->Fence != 0 &&
		GetContext().game->GetFence()->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(GetContext().game->GetFence()->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	GameTimer gt;
	UpdateMainPassCB(gt);
	mWorld->Update(gt);
	return true;
}

bool GameState::HandleEvent(WPARAM btnState, int x, int y)
{
	std::cout << "GameState: Handling event...\n";
	if (btnState != 0)
	{
		std::cout << "GameState: Key pressed. Pause requested\n";
		RequestStackPush(3);
	}
	return true;
}

void GameState::UpdateMainPassCB(const GameTimer& gt)
{
	auto& cam = GetContext().game->GetCamera();

	XMMATRIX view = cam.GetView();
	XMMATRIX proj = cam.GetProj();
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	XMMATRIX invView = XMMatrixInverse(nullptr, view);
	XMMATRIX invProj = XMMatrixInverse(nullptr, proj);
	XMMATRIX invViewProj = XMMatrixInverse(nullptr, viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));

	mMainPassCB.EyePosW = cam.GetPosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)GetContext().game->GetClientWidth(), (float)GetContext().game->GetClientHeight());
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / GetContext().game->GetClientWidth(), 1.0f / GetContext().game->GetClientHeight());
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void GameState::BuildFrameResources()
{
	ID3D12Device* device = GetContext().game->GetD3DDevice();
	size_t objectCount = mWorld->GetNumRenderItems();
	size_t materialCount = GetContext().game->GetMaterials().size();

	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(
			std::make_unique<FrameResource>(device, 1, static_cast<UINT>(objectCount), static_cast<UINT>(materialCount))
		);
	}

	mCurrFrameResourceIndex = 0;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();
}
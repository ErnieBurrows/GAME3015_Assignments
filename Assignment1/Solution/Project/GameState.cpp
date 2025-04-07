#include "GameState.h"
#include "Game.hpp"

#include "World.hpp"
#include <iostream>

static const int gNumFrameResources = 3;

GameState::GameState(StateStack& stack, Context context) : State(stack, context), mCurrFrameResourceIndex(0), mCurrFrameResource(nullptr)
{
	mWorld = std::make_unique<World>(GetContext().game);

	mWorld->BuildScene();

	// Create frame resources
	BuildFrameResources();
	std::cout << "GameState: Initialized.\n";
}

GameState::~GameState()
{
	std::cout << "GameState: Destroyed.\n";
}

void GameState::Draw()
{
    std::cout << "GameState: Drawing... \n";
    // Let the world update its nodes(each node updates its RenderItem data).
    mWorld->Draw();

    // Collect render items from the scene graph.
    std::vector<RenderItem*> renderItems;
    mWorld->CollectRenderItems(renderItems); 

    // Update constant buffers for each render item.
    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
    auto objectCB = mCurrFrameResource->ObjectCB->Resource();
    auto matCB = mCurrFrameResource->MaterialCB->Resource();

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
            // You could update material constant buffers similarly.
            ri->NumFramesDirty--;
        }
    }

    // Issue draw calls for each render item.
    ID3D12GraphicsCommandList* cmdList = GetContext().game->getCmdList();
    for (auto ri : renderItems)
    {
        auto vbv = ri->Geo->VertexBufferView();
        auto ibv = ri->Geo->IndexBufferView();
        cmdList->IASetVertexBuffers(0, 1, &vbv);
        cmdList->IASetIndexBuffer(&ibv);
        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

        // Bind descriptor for the object's material texture.
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
	// Cycle frame resource as needed.
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();


	GameTimer gt;
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

void GameState::BuildFrameResources()
{
	// Get the D3D device from the Game.
    ID3D12Device* device = GetContext().game->GetD3DDevice();


    size_t objectCount = mWorld->GetNumRenderItems();

	size_t materialCount = GetContext().game->GetMaterials().size();

    for (int i = 0; i < gNumFrameResources; ++i)
    {
        mFrameResources.push_back(
            std::make_unique<FrameResource>(device,
                1, static_cast<UINT>(objectCount), static_cast<UINT>(materialCount))
        );
    }
    mCurrFrameResourceIndex = 0;
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();
}

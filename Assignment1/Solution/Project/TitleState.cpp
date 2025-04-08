#include "TitleState.h"
#include "Game.hpp"
#include "RenderItem.h"
#include <iostream>

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context), mElapsedTime(0.0f), mCurrFrameResourceIndex(0), mCurrFrameResource(nullptr)
{
    std::cout << "TitleState: Initialized. (Press any key to continue...)\n";

    // Load texture for title screen
    GetContext().game->LoadTextureFromFile("../../Textures/Airplane_Metal.dds", "TitleScreenTex");

    // Create material
    GetContext().game->GetMaterials()["TitleScreen"] = GetContext().game->CreateMaterial("TitleScreen", 3, 3);

    // Geometry (assuming quad already created or loaded in BuildShapeGeometry)
    auto titleItem = std::make_unique<RenderItem>();
    titleItem->World = MathHelper::Identity4x4();
    XMStoreFloat4x4(&titleItem->World, XMMatrixScaling(10.0f, 10.0f, 1.0f));
    titleItem->ObjCBIndex = 0;
    titleItem->Mat = GetContext().game->GetMaterials()["TitleScreen"].get();
    titleItem->Geo = GetContext().game->GetGeometries()["boxGeo"].get(); // or "quadGeo" if available
    titleItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    titleItem->IndexCount = titleItem->Geo->DrawArgs["box"].IndexCount;
    titleItem->StartIndexLocation = titleItem->Geo->DrawArgs["box"].StartIndexLocation;
    titleItem->BaseVertexLocation = titleItem->Geo->DrawArgs["box"].BaseVertexLocation;

    mRenderItems.push_back(std::move(titleItem));

    BuildFrameResources();
}

void TitleState::Draw()
{
    auto game = GetContext().game;
    ID3D12GraphicsCommandList* cmdList = game->getCmdList();

    auto objectCB = mCurrFrameResource->ObjectCB->Resource();
    auto matCB = mCurrFrameResource->MaterialCB->Resource();
    auto passCB = mCurrFrameResource->PassCB->Resource();

    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

    // SET SLOT 2 (required!)
    cmdList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

    for (auto& ri : mRenderItems)
    {
        auto vbv = ri->Geo->VertexBufferView();
        auto ibv = ri->Geo->IndexBufferView();

        cmdList->IASetVertexBuffers(0, 1, &vbv);
        cmdList->IASetIndexBuffer(&ibv);
        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

        CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        tex.Offset(ri->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);

        D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;
        D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex * matCBByteSize;

        cmdList->SetGraphicsRootDescriptorTable(0, tex);
        cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
        cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);

        cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
    }
}


bool TitleState::Update(const GameTimer& gt)
{
    mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % 3;
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

    PassConstants pc;
    XMStoreFloat4x4(&pc.View, XMMatrixIdentity());
    XMStoreFloat4x4(&pc.Proj, XMMatrixIdentity());
    XMStoreFloat4x4(&pc.ViewProj, XMMatrixIdentity());
    XMStoreFloat4x4(&pc.InvView, XMMatrixIdentity());
    XMStoreFloat4x4(&pc.InvProj, XMMatrixIdentity());
    XMStoreFloat4x4(&pc.InvViewProj, XMMatrixIdentity());
    pc.EyePosW = XMFLOAT3(0, 0, -10); // or whatever works for your camera
    pc.RenderTargetSize = XMFLOAT2((float)GetContext().game->GetClientWidth(), (float)GetContext().game->GetClientHeight());
    pc.InvRenderTargetSize = XMFLOAT2(1.0f / pc.RenderTargetSize.x, 1.0f / pc.RenderTargetSize.y);
    pc.NearZ = 1.0f;
    pc.FarZ = 1000.0f;
    pc.TotalTime = gt.TotalTime();
    pc.DeltaTime = gt.DeltaTime();
    pc.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };

    // simple directional light
    pc.Lights[0].Direction = { 0.577f, -0.577f, 0.577f };
    pc.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };

    // write to constant buffer
    mCurrFrameResource->PassCB->CopyData(0, pc);
    
    return true;
}

bool TitleState::HandleEvent(WPARAM btnState, int x, int y)
{
    if (btnState != 0)
    {
        std::cout << "TitleState: Key pressed. Transitioning to Main Menu.\n";
        RequestStackPop();
        RequestStackPush(1);
    }
    return true;
}

void TitleState::BuildFrameResources()
{
    auto device = GetContext().game->GetD3DDevice();
    for (int i = 0; i < 3; ++i)
    {
        mFrameResources.push_back(
            std::make_unique<FrameResource>(device, 1, 1, 1));
    }
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();
}

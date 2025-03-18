#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
	, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "Eagle";
		break;
	case (Raptor):
		mSprite = "Eagle";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

void Aircraft::DrawCurrent() const
{	
	if (renderer)
	{
		renderer->World = GetWorldTransform();
		renderer->NumFramesDirty++;
	}
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mAircraftRitem != nullptr)
	{
		// Vertex buffer setup
		auto vbv = mAircraftRitem->Geo->VertexBufferView();
		auto ibv = mAircraftRitem->Geo->IndexBufferView();

		game->getCmdList()->IASetVertexBuffers(0, 1, &vbv);
		game->getCmdList()->IASetIndexBuffer(&ibv);
		game->getCmdList()->IASetPrimitiveTopology(mAircraftRitem->PrimitiveType);

		// Resource binding
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mAircraftRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);
		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->Mat->MatCBIndex * matCBByteSize;
		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);
		game->getCmdList()->DrawIndexedInstanced(mAircraftRitem->IndexCount, 1, mAircraftRitem->StartIndexLocation, mAircraftRitem->BaseVertexLocation, 0);
	}
}

void Aircraft::BuildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();

	renderer->World = GetTransform();
	renderer->ObjCBIndex = game->GetRenderItems().size();
	renderer->Mat = game->GetMaterials()[mSprite].get();
	renderer->Geo = game->GetGeometries()["planeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	renderer->IndexCount = renderer->Geo->DrawArgs["obj"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["obj"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["obj"].BaseVertexLocation;

	mAircraftRitem = renderer;
	game->GetRenderItems().push_back(std::move(render));
}

unsigned int Aircraft::GetCategory() const
{
	// For now this assumes "Eagle is the player aircraft"
	return (mType == Eagle) ? Category::PlayerAircraft : Category::Player2Aircraft;
}

void Aircraft::StartBarrelRoll()
{
	if (!mIsBarrelRolling)
	{
		mIsBarrelRolling = true;
		mRollAccumulated = 0.0f;
	}
}

void Aircraft::UpdateCurrent(const GameTimer& gt)
{
	if (mIsBarrelRolling)
	{
		const float rollSpeed = XM_PI / 0.5f;
		float dt = gt.DeltaTime();
		float rollStep = rollSpeed * dt;
		mRollAccumulated += rollStep;

		Rotate(0.0f, 0.0f, rollStep);

		if (mRollAccumulated >= XM_PI)
		{
			mIsBarrelRolling = false;
		}

		if (renderer)
		{
			renderer->World = GetWorldTransform();
			renderer->NumFramesDirty++;
		}
	}
}

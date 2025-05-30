#include "World.hpp"
#include "Player.h"
#include "StateStack.h"

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	virtual bool Initialize()override;
private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;
public:
	void OnKeyboardInput(const GameTimer& gt);

	Microsoft::WRL::ComPtr<ID3D12Fence> GetFence() const { return mFence; }

	void UpdateMaterialCBs(UploadBuffer<MaterialConstants>* currMaterialCB);
private:
	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

	//step5
	void LoadTextures();

	void BuildRootSignature();

	//step9
	void BuildDescriptorHeaps();
	void BuildShadersAndInputLayout();
	void BuildShapeGeometry();
	void BuildPSOs();
	void BuildFrameResources();
	void BuildMaterials();
	void BuildRenderItems();
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

public:
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	UINT mCbvSrvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	ID3D12Device* GetD3DDevice() const { return md3dDevice.Get(); }

	Camera& GetCamera() { return mCamera; }

	int GetClientWidth() const { return mClientWidth; }
	int GetClientHeight() const { return mClientHeight; }
private:

	

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	//step11

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	//step7
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	POINT mLastMousePos;
	Camera mCamera;


	//World mWorld;
	

public:
	std::vector<std::unique_ptr<RenderItem>>& GetRenderItems() { return mAllRitems; }
	std::unordered_map<std::string, std::unique_ptr<Material>>& GetMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& GetGeometries() { return mGeometries; }

	void LoadObJModel(const std::string& filename, std::unique_ptr<MeshGeometry>& mesh);

	void LoadTextureFromFile(const std::string& fileName, const std::string& textureName);

	//Function to create custom material
	std::unique_ptr<Material> CreateMaterial(
		const std::string& name,
		int matCBIndex,
		int diffuseSrvHeapIndex,
		const XMFLOAT4& diffuseAlbedo,
		const XMFLOAT3& fresnelR0,
		float roughness);

	//Function to create default material
	std::unique_ptr<Material> CreateMaterial(const std::string& name, int matCBIndex, int diffuseSrvHeapIndex);

	ID3D12GraphicsCommandList* getCmdList() { return mCommandList.Get(); }


/* --- State System ---*/
private:
	StateStack mStateStack;
	enum StateID
	{
		Title = 0,
		MainMenu = 1,
		Play = 2,
		Pause = 3
	};


};

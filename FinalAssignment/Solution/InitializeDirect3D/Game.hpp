#include "World.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include <dwrite.h>
#include <d2d1.h>


class Game : public D3DApp
{
public:

	Game(HINSTANCE hInstance);

	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;

	~Game();

	virtual bool Initialize() override;

public:

	virtual void OnResize() override;

	virtual void Update(const GameTimer& gt) override;

	virtual void Draw(const GameTimer& gt) override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;
	virtual void OnKeyDown(WPARAM btnState) override;

	void RegisterStates();

	void CreateTexture(std::string Name, std::wstring FileName);

	void CreateMaterials(std::string Name, XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float Roughness);

	void LoadTextures();  
	void BuildRootSignature(); 
	void BuildDescriptorHeaps(); 
	void BuildShadersAndInputLayout();  
	void BuildShapeGeometry();  
	void BuildHillGeometry(); 
	void BuildPSOs();
	void BuildFrameResources(int renderItemCount); 
	void BuildMaterials();  
	void ResetFrameResources();


	void LoadObJModel(const std::string& filename, std::unique_ptr<MeshGeometry>& mesh);


	void UpdateCamera(const GameTimer& gt); 
	float GetHillsHeight(float x, float z) const;  
	XMFLOAT3 GetHillsNormal(float x, float z) const; 



	void UpdateObjectCBs(const GameTimer& gt);  
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt); 
	void AnimateMaterials(const GameTimer& gt); 


	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers(); 
	void WaitForGPU(); 

	
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr; 
	int mCurrFrameResourceIndex = 0; 

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries; 
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials; 
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures; 

	int mCurrentMaterialCBIndex = 0;

	int mCurrentDiffuseSrvHeapIndex = 0;

	UINT mCbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	POINT mLastMousePos;

	Camera mCamera;

	Player mPlayer;
	StateStack mStateStack;  

	ID3D12GraphicsCommandList* getCmdList() { return mCommandList.Get(); }
	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }

private:

	ID2D1Factory* pD2DFactory_ = nullptr;  
	ID2D1HwndRenderTarget* pRT_ = nullptr; 
	ID2D1SolidColorBrush* pBlackBrush_ = nullptr;  
	IDWriteFactory* pDWriteFactory_ = nullptr;  
	IDWriteTextFormat* pTextFormat_ = nullptr;  

	const wchar_t* wszText_ = nullptr; 
	UINT32 cTextLength_ = 0; 
};


#include <array>
#include "../common/d3dApp.h"
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../common/d3dUtil.h"
#include "../common/UploadBuffer.h"

#include "FrameResource.h"

class StencilDemo : public D3DApp
{
	struct RenderItem;
public:
	StencilDemo(HINSTANCE hInstance);
	~StencilDemo();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	void OnKeyboardInput(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void UpdateMaterialCB(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void UpdateReflectedPassCB(const GameTimer& gt);

	void BuildShadersAndInputLayout();
	void BuildPSO();

	void BuildRootSignature(); 
	void BuildDescriptorHeaps();

	void BuildFrameResources();
	void LoadTextures();
	void BuildMaterials();
	void BuildRenderItems();
	void BuildRoomGeometry();
	void BuildSkullGeometry();
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

private:
	//static const int gNumFrameResources = 3;

	enum class RenderLayer : int
	{
		Opaque = 0,
		Mirrors,
		Reflected,
		Transparent,
		Shadow,
		Count
	};


	struct RenderItem
	{
		RenderItem() = default;

		// World matrix of the shape that describes the object's local space
		// relative to the world space, which defines the position, orientation,
		// and scale of the object in the world.
		Math::Mat4 World{};
		Math::Mat4 TexTransform{};

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		int NumFramesDirty = 3;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		uint ObjCBIndex = -1;

		Material* Mat = nullptr;
		MeshGeometry* Geo = nullptr;

		// Primitive topology.
		D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// DrawIndexedInstanced parameters.
		uint IndexCount = 0;
		uint StartIndexLocation = 0;
		int BaseVertexLocation = 0;
	};

	// FrameResource
	std::vector<std::unique_ptr<StencilDemoFR::FrameResource>> mFrameResources;
	StencilDemoFR::FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	uint mCbvSrvDescriptorSize = 0;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;	// List of all the render items.
	RenderItem* mSkullRitem = nullptr;
	RenderItem* mReflectedSkullRitem = nullptr;
	RenderItem* mShadowedSkullRitem = nullptr;

	std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];

	StencilDemoFR::PassConstants mMainPassCB;
	StencilDemoFR::PassConstants mReflectedPassCB;

	ID3D12RootSignature* mRootSignature = nullptr;
	ID3D12DescriptorHeap* mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ID3DBlob*> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	std::unordered_map<std::string, ID3D12PipelineState*> mPSOs;

	Math::Mat4 mWorld{};
	Math::Mat4 mView{};
	Math::Mat4 mProj{};

	Math::Vec3 mSkullTranslation = { 0.0f, 1.0f, -5.0f };

	float mTheta = 1.24f * M_PI;
	float mPhi = 0.42f * M_PI;
	float mRadius = 12.0f;

	float mSunTheta = 1.25f * M_PI;
	float mSunPhi = M_PI / 4;

	Math::Vec3 mEyePos = { 0.0f, 0.0f, 0.0f };

	POINT mLastMousePos;

	bool mIsWireframe{ false };

	uint mPassCbvOffset = 0;

};


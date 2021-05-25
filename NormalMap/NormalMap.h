
#include <array>
#include "../common/d3dApp.h"
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../common/d3dUtil.h"
#include "../common/UploadBuffer.h"
#include "../common/Camera.h"

#include "FrameResource.h"

class NormalMap : public D3DApp
{
	struct RenderItem;
public:
	NormalMap(HINSTANCE hInstance);
	~NormalMap();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	void OnKeyboardInput(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void UpdateMaterialBuffer(const GameTimer& gt);

	void LoadTextures();
	void BuildShadersAndInputLayout();
	void BuildPSO();

	void BuildRootSignature();
	void BuildDescriptorHeaps();

	void BuildFrameResources();
	void BuildMaterials();
	void BuildRenderItems();
	void BuildShapeGeometry();
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

private:
	static const int gNumFrameResources = 3;

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
		int NumFramesDirty = gNumFrameResources;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		uint ObjCBIndex = -1;

		Material* Mat = nullptr;
		MeshGeometry* Geo = nullptr;

		// Primitive topology.
		D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// DrawIndexedInstanced parameters.
		uint IndexCount = 0;
		uint StartIndexLocation = 0;
		int BaseVertexLocation = 0;
	};

	enum class RenderLayer : int
	{
		Opaque = 0,
		Sky,
		Count
	};

	// FrameResource
	std::vector<std::unique_ptr<NormalMapFR::FrameResource>> mFrameResources;
	NormalMapFR::FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	uint mCbvSrvDescriptorSize = 0;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;	// List of all the render items.
	std::vector<RenderItem*> mRitemLayer[(int)RenderLayer::Count];

	UINT mSkyTexHeapIndex = 0;

	NormalMapFR::PassConstants mMainPassCB;

	ID3D12RootSignature* mRootSignature = nullptr;
	ID3D12DescriptorHeap* mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ID3DBlob*> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	std::unordered_map<std::string, ID3D12PipelineState*> mPSOs;

	Camera mCamera;

	POINT mLastMousePos;

	bool mIsWireframe{ false };

};


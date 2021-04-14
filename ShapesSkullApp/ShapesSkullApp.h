
#include <array>
#include "../common/d3dApp.h"
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../common/d3dUtil.h"
#include "../common/UploadBuffer.h"

#include "FrameResource.h"

class ShapesSkullApp : public D3DApp
{
	struct RenderItem;
public:
	ShapesSkullApp(HINSTANCE hInstance);
	~ShapesSkullApp();

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
	void UpdateCamera(const GameTimer& gt);

	void BuildShadersAndInputLayout();
	void BuildPSO();

	void BuildRootSignature();

	void BuildFrameResources();
	void BuildRenderItems();
	void BuildShapeGeometry();
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

private:
	static const int gNumFrameResources = 3;

	struct RenderItem
	{
		RenderItem() = default;

		// World matrix of the shape that describes the object's local space
		// relative to the world space, which defines the position, orientation,
		// and scale of the object in the world.
		Math::Mat4 World{};

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		int NumFramesDirty = gNumFrameResources;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		uint ObjCBIndex = -1;

		MeshGeometry* Geo = nullptr;

		// Primitive topology.
		D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// DrawIndexedInstanced parameters.
		uint IndexCount = 0;
		uint StartIndexLocation = 0;
		int BaseVertexLocation = 0;
	};

	// FrameResource
	std::vector<std::unique_ptr<ShapesSkullFR::FrameResource>> mFrameResources;
	ShapesSkullFR::FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;	// List of all the render items.
	std::vector<RenderItem*> mOpaqueRitems;		// Render items divided by PSO.

	ShapesSkullFR::PassConstants mMainPassCB;

	ID3D12RootSignature* mRootSignature = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;

	ID3DBlob* mvsByteCode = nullptr;
	ID3DBlob* mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	std::unordered_map<std::string, ID3D12PipelineState*> mPSOs;

	Math::Mat4 mWorld{};
	Math::Mat4 mView{};
	Math::Mat4 mProj{};

	float mTheta = 1.5f * M_PI;
	float mPhi = M_PI / 4;
	float mRadius = 15.0f;

	Math::Vec3 mEyePos = { 0.0f, 0.0f, 0.0f };

	POINT mLastMousePos;

	bool mIsWireframe{ false };

};


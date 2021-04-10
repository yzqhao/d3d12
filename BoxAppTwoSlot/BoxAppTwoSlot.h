
#include <array>
#include "../common/d3dApp.h"
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../common/d3dUtil.h"
#include "../common/UploadBuffer.h"

class BoxAppTwoSlot : public D3DApp
{
public:
	BoxAppTwoSlot(HINSTANCE hInstance);
	~BoxAppTwoSlot();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	void BuildBoxGeometry();
	void BuildShadersAndInputLayout();
	void BuildPSO();

	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();

private:
	struct VertexP
	{
		Math::Vec3 Pos;
	};
	struct VertexC
	{
		Math::Color Color;
	};

	struct MeshGeometryVB2 : public MeshGeometry
	{
		ID3DBlob* VertexBufferCPU2 = nullptr;
		ID3D12Resource* VertexBufferGPU2 = nullptr;
		ID3D12Resource* VertexBufferUploader2 = nullptr;
		uint VertexByteStride2 = 0;
		uint VertexBufferByteSize2 = 0;

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView2()const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = VertexBufferGPU2->GetGPUVirtualAddress();
			vbv.StrideInBytes = VertexByteStride2;
			vbv.SizeInBytes = VertexBufferByteSize2;

			return vbv;
		}
	};

	struct ObjectConstants
	{
		Math::Mat4 View;
		Math::Mat4 Proj;
	};

	ID3D12RootSignature* mRootSignature = nullptr;
	ID3D12DescriptorHeap* mCbvHeap = nullptr;

	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	std::unique_ptr<MeshGeometryVB2> mBoxGeo = nullptr;

	ID3DBlob* mvsByteCode = nullptr;
	ID3DBlob* mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ID3D12PipelineState* mPSO = nullptr;

	Math::Mat4 mWorld{};
	Math::Mat4 mView{};
	Math::Mat4 mProj{};

	float mTheta = 1.5f * M_PI;
	float mPhi = M_PI / 4;
	float mRadius = 5.0f;

	POINT mLastMousePos;

};


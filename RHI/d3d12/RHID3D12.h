
#pragma once

#include "../RHIGrahpicSystem.h"
#include "../../common/d3dUtil.h"
#include "../../common/UploadBuffer.h"
#include "D3D12Resource.h"
#include "../RHIVertexFormat.h"
#include "../RHIVertexBuffer.h"
#include "../RHIIndexBuffer.h"

class RHID3D12 final : public RHIGrahpicSystem
{
public:
	RHID3D12();
	~RHID3D12();

	static RHID3D12* Instance();

	virtual bool Init(HWND window) override;

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	virtual void OnResize(int w, int h) override;

	virtual void BeginRender() override;
	virtual void EndRender() override;

	virtual bool Get4xMsaaState()const override;
	virtual void Set4xMsaaState(bool value) override;

	virtual bool IsLostDevice() override { return md3dDevice == nullptr; }
	virtual bool BMsaaState() override { return m4xMsaaState; }

	ID3D12Device* GetD3D12Device() const { return md3dDevice; }

	virtual bool OnLoadVShaderProgram(RHIVSShader* pVShaderProgram, RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseVShaderProgram(RHIResourceIdentifier* pID) override;

	virtual bool OnLoadPShaderProgram(RHIPSShader* pPShaderProgram, RHIResourceIdentifier*& pID) override;
	virtual bool OnReleasePShaderProgram(RHIResourceIdentifier* pID) override;

	virtual bool OnLoadVBufferFormat(RHIVertexFormat* pVertexFormat, RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseVBufferFormat(RHIResourceIdentifier* pID) override;

	virtual bool OnLoadVBufferDate(RHIVertexBuffer* pVertexBuffer, RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseVBufferDate(RHIResourceIdentifier* pID) override;

	virtual bool OnLoadIBufferDate(RHIIndexBuffer* pIndexBuffer, RHIResourceIdentifier*& pID) override;
	virtual bool OnReleaseIBufferDate(RHIResourceIdentifier* pID) override;

protected:

	void CreateVShaderFromCache(RHIVSShader* pVShaderProgram, RHIShaderID* pID);
	void CreateVShaderFromString(RHIVSShader* pVShaderProgram, RHIShaderID* pID);
	void OnLoadVShaderFromString(RHIVSShader* pVShaderProgram, RHIShaderID* pVProgramID);

	void CreatePShaderFromCache(RHIPSShader* pPShaderProgram, RHIShaderID* pID);
	void CreatePShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pID);
	void OnLoadPShaderFromString(RHIPSShader* pPShaderProgram, RHIShaderID* pVProgramID);

	bool CreateInputLayout(const std::vector<RHIVertexFormat::VERTEXFORMAT_TYPE>& FormatArray, RHIVBufferFormatID * pVBufferFormatID);

	void OnLoadVertexBufferEx(uint uiOneVextexSize, uint uiTotalSize, RHIVertexBuffer* pVBuffer, RHIVBufferID * pVBufferID);
	void OnLoadVertexBufferEx(uint uiTotalSize, RHIIndexBuffer* pVBuffer, RHIIBufferID * pVBufferID);

private:

	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void FlushCommandQueue();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	HWND      mhMainWnd = nullptr; // main window handle

	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	IDXGIFactory4* mdxgiFactory{};
	IDXGISwapChain* mSwapChain{};
	ID3D12Device* md3dDevice{};

	ID3D12Fence* mFence{};
	UINT64 mCurrentFence = 0;

	ID3D12CommandQueue* mCommandQueue{};
	ID3D12CommandAllocator* mDirectCmdListAlloc{};
	ID3D12GraphicsCommandList* mCommandList{};

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	ID3D12Resource* mSwapChainBuffer[SwapChainBufferCount]{};
	ID3D12Resource* mDepthStencilBuffer{};

	ID3D12DescriptorHeap* mRtvHeap{};
	ID3D12DescriptorHeap* mDsvHeap{};

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ID3D12PipelineState* mPSO = nullptr;

	int mClientWidth = 800;
	int mClientHeight = 600;

};
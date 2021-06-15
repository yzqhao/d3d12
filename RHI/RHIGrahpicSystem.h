
#pragma once

#include <array>
#include "../math/Color.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "../common/d3dUtil.h"
#include "../common/UploadBuffer.h"

class RHIGrahpicSystem
{
public:
	RHIGrahpicSystem();
	~RHIGrahpicSystem();

	bool InitDirect3D(HWND window);
	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void OnResize();

	void BeginRender();
	void EndRender();

	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	bool IsLostDevice() { return md3dDevice == nullptr; }
	bool BMsaaState() { return m4xMsaaState; }

private:

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
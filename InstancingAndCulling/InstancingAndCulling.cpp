
#include "InstancingAndCulling.h"
#include "../InstancingAndCulling/FrameResource.h"
#include "../common/GeometryGenerator.h"
#include "../common/MeshLoader.h"
#include "../common/DDSTextureLoader.h"

InstancingAndCulling::InstancingAndCulling(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

InstancingAndCulling::~InstancingAndCulling()
{
}

bool InstancingAndCulling::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mCamera.SetPosition(0.0f, 2.0f, -15.0f);

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildSkullGeometry();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSO();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();


	return true;
}

void InstancingAndCulling::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void InstancingAndCulling::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void InstancingAndCulling::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = MATH_DEG_TO_RAD(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = MATH_DEG_TO_RAD(0.25f * static_cast<float>(y - mLastMousePos.y));

		mCamera.Pitch(dy);
		mCamera.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void InstancingAndCulling::OnResize()
{
	D3DApp::OnResize();

	mCamera.SetLens(0.25f*M_PI, AspectRatio(), 1.0f, 1000.0f);
}

void InstancingAndCulling::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);

	// Cycle through the circular frame resource array.
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	// Has the GPU finished processing the commands of the current frame resource?
	// If not, wait until the GPU has completed commands up to this fence point.
	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	UpdateInstanceData(gt);
	UpdateMaterialBuffer(gt);
	UpdateMainPassCB(gt);
}

void InstancingAndCulling::LoadTextures()
{
	auto bricksTex = std::make_unique<Texture>();
	bricksTex->Name = "bricksTex";
	bricksTex->Filename = L"Textures/bricks.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, bricksTex->Filename.c_str(),
		bricksTex->Resource, bricksTex->UploadHeap));

	auto stoneTex = std::make_unique<Texture>();
	stoneTex->Name = "stoneTex";
	stoneTex->Filename = L"Textures/stone.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, stoneTex->Filename.c_str(),
		stoneTex->Resource, stoneTex->UploadHeap));

	auto tileTex = std::make_unique<Texture>();
	tileTex->Name = "tileTex";
	tileTex->Filename = L"Textures/tile.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, tileTex->Filename.c_str(),
		tileTex->Resource, tileTex->UploadHeap));

	auto crateTex = std::make_unique<Texture>();
	crateTex->Name = "crateTex";
	crateTex->Filename = L"Textures/WoodCrate01.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, crateTex->Filename.c_str(),
		crateTex->Resource, crateTex->UploadHeap));

	auto iceTex = std::make_unique<Texture>();
	iceTex->Name = "iceTex";
	iceTex->Filename = L"Textures/ice.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, iceTex->Filename.c_str(),
		iceTex->Resource, iceTex->UploadHeap));

	auto grassTex = std::make_unique<Texture>();
	grassTex->Name = "grassTex";
	grassTex->Filename = L"Textures/grass.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, grassTex->Filename.c_str(),
		grassTex->Resource, grassTex->UploadHeap));

	auto defaultTex = std::make_unique<Texture>();
	defaultTex->Name = "defaultTex";
	defaultTex->Filename = L"Textures/white1x1.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, defaultTex->Filename.c_str(),
		defaultTex->Resource, defaultTex->UploadHeap));

	mTextures[bricksTex->Name] = std::move(bricksTex);
	mTextures[stoneTex->Name] = std::move(stoneTex);
	mTextures[tileTex->Name] = std::move(tileTex);
	mTextures[crateTex->Name] = std::move(crateTex);
	mTextures[iceTex->Name] = std::move(iceTex);
	mTextures[grassTex->Name] = std::move(grassTex);
	mTextures[defaultTex->Name] = std::move(defaultTex);
}

void InstancingAndCulling::OnKeyboardInput(const GameTimer& gt)
{
	const float dt = gt.DeltaTime();

	if (GetAsyncKeyState('W') & 0x8000)
		mCamera.Walk(10.0f*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCamera.Walk(-10.0f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCamera.Strafe(-10.0f*dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCamera.Strafe(10.0f*dt);

	mCamera.UpdateViewMatrix();
}

void InstancingAndCulling::UpdateInstanceData(const GameTimer& gt)
{
	auto view = mCamera.GetView();
	auto invView = view.getInversed();

	auto currInstanceBuffer = mCurrFrameResource->InstanceBuffer.get();
	for (auto& e : mAllRitems)
	{
		const auto& instanceData = e->Instances;

		int visibleInstanceCount = 0;

		for (UINT i = 0; i < (UINT)instanceData.size(); ++i)
		{
			Math::Mat4 world = instanceData[i].World;
			Math::Mat4 texTransform = instanceData[i].TexTransform;

			Math::Mat4 invWorld = world.getInversed();

			// View space to the object's local space.
			Math::Mat4 viewToLocal = invView * invWorld;

			// Transform the camera frustum from view space to the object's local space.
			//BoundingFrustum localSpaceFrustum;
			//mCamFrustum.Transform(localSpaceFrustum, viewToLocal);

			// Perform the box/frustum intersection test in local space.
			//if ((localSpaceFrustum.Contains(e->Bounds) != DirectX::DISJOINT) || (mFrustumCullingEnabled == false))
			{
				InstancingAndCullingFR::InstanceData data;
				data.World = world.getTransposed();
				data.TexTransform = texTransform.getTransposed();
				data.MaterialIndex = instanceData[i].MaterialIndex;

				// Write the instance data to structured buffer for the visible objects.
				currInstanceBuffer->CopyData(visibleInstanceCount++, data);
			}
		}

		e->InstanceCount = visibleInstanceCount;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"Instancing and Culling Demo" <<
			L"    " << e->InstanceCount <<
			L" objects visible out of " << e->Instances.size();
		mMainWndCaption = outs.str();
	}
}

void InstancingAndCulling::UpdateMainPassCB(const GameTimer& gt)
{
	mMainPassCB.View = mCamera.GetView();
	mMainPassCB.Proj = mCamera.GetProj();
	mMainPassCB.ViewProj = mMainPassCB.View * mMainPassCB.Proj;
	mMainPassCB.InvView = mMainPassCB.View.getInversed();
	mMainPassCB.InvProj = mMainPassCB.Proj.getInversed();
	mMainPassCB.InvViewProj = mMainPassCB.ViewProj.getInversed();

	mMainPassCB.EyePosW = mCamera.GetPosition();
	mMainPassCB.RenderTargetSize.set((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize.set(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();

	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.2f, 0.2f, 0.2f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void InstancingAndCulling::UpdateMaterialBuffer(const GameTimer& gt)
{
	auto currMaterialBufer = mCurrFrameResource->MaterialBuffer.get();
	for (auto& e : mMaterials)
	{
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)
		{
			InstancingAndCullingFR::MaterialData matData;
			matData.DiffuseAlbedo = mat->DiffuseAlbedo;
			matData.FresnelR0 = mat->FresnelR0;
			matData.Roughness = mat->Roughness;
			matData.MatTransform = mat->MatTransform;
			matData.DiffuseMapIndex = mat->DiffuseSrvHeapIndex;

			currMaterialBufer->CopyData(mat->MatCBIndex, matData);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

void InstancingAndCulling::Draw(const GameTimer& gt)
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, mPSOs["opaque"]));

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Math::Color::WHITE.getPtr(), 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	{	// draw
		ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvDescriptorHeap };
		mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		mCommandList->SetGraphicsRootSignature(mRootSignature);

		// Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
		// set as a root descriptor.
		auto matBuffer = mCurrFrameResource->MaterialBuffer->Resource();
		mCommandList->SetGraphicsRootShaderResourceView(1, matBuffer->GetGPUVirtualAddress());

		auto passCB = mCurrFrameResource->PassCB->Resource();
		mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

		// Bind all the textures used in this scene.  Observe
		// that we only have to specify the first descriptor in the table.  
		// The root signature knows how many descriptors are expected in the table.
		mCommandList->SetGraphicsRootDescriptorTable(3, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

		DrawRenderItems(mCommandList, mOpaqueRitems);
	}

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}

// 
// app define
//

void InstancingAndCulling::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	mShaders["standardVS"] = d3dUtil::CompileShader(L"InstancingAndCulling\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"InstancingAndCulling\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

}

void InstancingAndCulling::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (uint)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));
}

void InstancingAndCulling::BuildRootSignature()
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[4];

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 7, 0, 0);

	// Create root CBVs.
	slotRootParameter[0].InitAsShaderResourceView(0, 1);
	slotRootParameter[1].InitAsShaderResourceView(1, 1);
	slotRootParameter[2].InitAsConstantBufferView(0);
	slotRootParameter[3].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	// A root signature is an array of root parameters.
	auto staticSamplers = GetStaticSamplers();
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter, staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&serializedRootSig, &errorBlob);

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void InstancingAndCulling::BuildDescriptorHeaps()
{
	//
	// Create the SRV heap.
	//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 7;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto bricksTex = mTextures["bricksTex"]->Resource;
	auto stoneTex = mTextures["stoneTex"]->Resource;
	auto tileTex = mTextures["tileTex"]->Resource;
	auto crateTex = mTextures["crateTex"]->Resource;
	auto iceTex = mTextures["iceTex"]->Resource;
	auto grassTex = mTextures["grassTex"]->Resource;
	auto defaultTex = mTextures["defaultTex"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = bricksTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = bricksTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(bricksTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = stoneTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = stoneTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(stoneTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = tileTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = tileTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(tileTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = crateTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = crateTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(crateTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = grassTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = grassTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(grassTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = iceTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = iceTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(iceTex, &srvDesc, hDescriptor);

	// next descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);

	srvDesc.Format = defaultTex->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = defaultTex->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(defaultTex, &srvDesc, hDescriptor);
}

void InstancingAndCulling::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<InstancingAndCullingFR::FrameResource>(md3dDevice,
			1, (uint)mAllRitems.size(), (uint)mMaterials.size()));
	}
}

void InstancingAndCulling::BuildMaterials()
{
	auto bricks0 = std::make_unique<Material>();
	bricks0->Name = "bricks0";
	bricks0->MatCBIndex = 0;
	bricks0->DiffuseSrvHeapIndex = 0;
	bricks0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
    bricks0->FresnelR0.set(0.02f, 0.02f, 0.02f);
    bricks0->Roughness = 0.1f;

	auto stone0 = std::make_unique<Material>();
	stone0->Name = "stone0";
	stone0->MatCBIndex = 1;
	stone0->DiffuseSrvHeapIndex = 1;
	stone0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
    stone0->FresnelR0.set(0.05f, 0.05f, 0.05f);
    stone0->Roughness = 0.3f;
 
	auto tile0 = std::make_unique<Material>();
	tile0->Name = "tile0";
	tile0->MatCBIndex = 2;
	tile0->DiffuseSrvHeapIndex = 2;
	tile0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
    tile0->FresnelR0.set(0.02f, 0.02f, 0.02f);
	tile0->Roughness = 0.3f;

	auto crate0 = std::make_unique<Material>();
	crate0->Name = "checkboard0";
	crate0->MatCBIndex = 3;
	crate0->DiffuseSrvHeapIndex = 3;
	crate0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
	crate0->FresnelR0.set(0.05f, 0.05f, 0.05f);
	crate0->Roughness = 0.2f;

	auto ice0 = std::make_unique<Material>();
	ice0->Name = "ice0";
	ice0->MatCBIndex = 4;
	ice0->DiffuseSrvHeapIndex = 4;
	ice0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
	ice0->FresnelR0.set(0.1f, 0.1f, 0.1f);
	ice0->Roughness = 0.0f;

	auto grass0 = std::make_unique<Material>();
	grass0->Name = "grass0";
	grass0->MatCBIndex = 5;
	grass0->DiffuseSrvHeapIndex = 5;
	grass0->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
	grass0->FresnelR0.set(0.05f, 0.05f, 0.05f);
	grass0->Roughness = 0.2f;

	auto skullMat = std::make_unique<Material>();
	skullMat->Name = "skullMat";
	skullMat->MatCBIndex = 6;
	skullMat->DiffuseSrvHeapIndex = 6;
	skullMat->DiffuseAlbedo.set(1.0f, 1.0f, 1.0f, 1.0f);
	skullMat->FresnelR0.set(0.05f, 0.05f, 0.05f);
	skullMat->Roughness = 0.5f;
	
	mMaterials["bricks0"] = std::move(bricks0);
	mMaterials["stone0"] = std::move(stone0);
	mMaterials["tile0"] = std::move(tile0);
	mMaterials["crate0"] = std::move(crate0);
	mMaterials["ice0"] = std::move(ice0);
	mMaterials["grass0"] = std::move(grass0);
	mMaterials["skullMat"] = std::move(skullMat);
}

void InstancingAndCulling::BuildRenderItems()
{
	auto skullRitem = std::make_unique<RenderItem>();
	skullRitem->ObjCBIndex = 0;
	skullRitem->Mat = mMaterials["tile0"].get();
	skullRitem->Geo = mGeometries["skullGeo"].get();
	skullRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skullRitem->InstanceCount = 0;
	skullRitem->IndexCount = skullRitem->Geo->DrawArgs["skull"].IndexCount;
	skullRitem->StartIndexLocation = skullRitem->Geo->DrawArgs["skull"].StartIndexLocation;
	skullRitem->BaseVertexLocation = skullRitem->Geo->DrawArgs["skull"].BaseVertexLocation;
	skullRitem->Bounds = skullRitem->Geo->DrawArgs["skull"].Bounds;

	// Generate instance data.
	const int n = 5;
	skullRitem->Instances.resize(n * n * n);

	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;

	float x = -0.5f * width;
	float y = -0.5f * height;
	float z = -0.5f * depth;
	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int index = k * n * n + i * n + j;
				// Position instanced along a 3D grid.
				skullRitem->Instances[index].World = Math::Mat4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					x + j * dx, y + i * dy, z + k * dz, 1.0f);

				skullRitem->Instances[index].TexTransform.scale(2.0, 2.0, 1.0);
				skullRitem->Instances[index].MaterialIndex = index % mMaterials.size();
			}
		}
	}

	mAllRitems.push_back(std::move(skullRitem));

	// All the render items are opaque.
	for (auto& e : mAllRitems)
		mOpaqueRitems.push_back(e.get());
}

void InstancingAndCulling::BuildSkullGeometry()
{
	// skull
	TexModel model;
	LoadModleSkull("Models/skull.txt", model);

	std::vector<InstancingAndCullingFR::Vertex> vertices(model.vertex.size());
	for (size_t i = 0; i < model.vertex.size(); ++i)
	{
		vertices[i].Pos = model.vertex[i].Pos;
		vertices[i].Normal = model.vertex[i].Normal;
	}

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(InstancingAndCullingFR::Vertex);

	const UINT ibByteSize = (UINT)model.indices.size() * sizeof(std::int16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "skullGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), model.indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, model.indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(InstancingAndCullingFR::Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)model.indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	for (size_t i = 0; i < model.vertex.size(); ++i)
	{
		submesh.Bounds.merge(vertices[i].Pos);
	}

	geo->DrawArgs["skull"] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void InstancingAndCulling::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		auto instanceBuffer = mCurrFrameResource->InstanceBuffer->Resource();
		mCommandList->SetGraphicsRootShaderResourceView(0, instanceBuffer->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(ri->IndexCount, ri->InstanceCount, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> InstancingAndCulling::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

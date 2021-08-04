
#include "PickApp.h"
#include "../PickApp/FrameResource.h"
#include "../common/GeometryGenerator.h"
#include "../common/MeshLoader.h"
#include "../common/DDSTextureLoader.h"

PickApp::PickApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

PickApp::~PickApp()
{
}

bool PickApp::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mCamera.LookAt(
		Math::Vec3(5.0f, 4.0f, -15.0f),
		Math::Vec3(0.0f, 1.0f, 0.0f),
		Math::Vec3(0.0f, 1.0f, 0.0f));

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildCarGeometry();
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

void PickApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(mhMainWnd);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		Pick(x, y);
	}
}

void PickApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void PickApp::OnMouseMove(WPARAM btnState, int x, int y)
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

void PickApp::OnResize()
{
	D3DApp::OnResize();

	mCamera.SetLens(0.25f*M_PI, AspectRatio(), 1.0f, 1000.0f);
}

void PickApp::Update(const GameTimer& gt)
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

	UpdateObjectCBs(gt);
	UpdateMaterialBuffer(gt);
	UpdateMainPassCB(gt);
}

void PickApp::LoadTextures()
{
	auto defaultDiffuseTex = std::make_unique<Texture>();
	defaultDiffuseTex->Name = "defaultDiffuseTex";
	defaultDiffuseTex->Filename = L"Textures/white1x1.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice,
		mCommandList, defaultDiffuseTex->Filename.c_str(),
		defaultDiffuseTex->Resource, defaultDiffuseTex->UploadHeap));

	mTextures[defaultDiffuseTex->Name] = std::move(defaultDiffuseTex);
}

void PickApp::OnKeyboardInput(const GameTimer& gt)
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

void PickApp::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : mAllRitems)
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if (e->NumFramesDirty > 0)
		{
			PickAppFR::ObjectConstants objConstants;
			objConstants.World = e->World;
			objConstants.TexTransform = e->TexTransform;
			objConstants.MaterialIndex = e->Mat->MatCBIndex;

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

void PickApp::UpdateMainPassCB(const GameTimer& gt)
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

void PickApp::UpdateMaterialBuffer(const GameTimer& gt)
{
	auto currMaterialBufer = mCurrFrameResource->MaterialBuffer.get();
	for (auto& e : mMaterials)
	{
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)
		{
			PickAppFR::MaterialData matData;
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

void PickApp::Draw(const GameTimer& gt)
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

		auto passCB = mCurrFrameResource->PassCB->Resource();
		mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

		// Bind all the materials used in this scene.  For structured buffers, we can bypass the heap and 
		// set as a root descriptor.
		auto matBuffer = mCurrFrameResource->MaterialBuffer->Resource();
		mCommandList->SetGraphicsRootShaderResourceView(2, matBuffer->GetGPUVirtualAddress());

		// Bind all the textures used in this scene.  Observe
		// that we only have to specify the first descriptor in the table.  
		// The root signature knows how many descriptors are expected in the table.
		mCommandList->SetGraphicsRootDescriptorTable(3, mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

		DrawRenderItems(mCommandList, mRitemLayer[(int)RenderLayer::Opaque]);
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

void PickApp::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	mShaders["standardVS"] = d3dUtil::CompileShader(L"PickApp\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"PickApp\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

}

void PickApp::BuildPSO()
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

void PickApp::BuildRootSignature()
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[4];

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0, 0);

	// Create root CBVs.
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsShaderResourceView(0, 1);
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

void PickApp::BuildDescriptorHeaps()
{
	//
	// Create the SRV heap.
	//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto defaultDiffuseTex = mTextures["defaultDiffuseTex"]->Resource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = defaultDiffuseTex->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = defaultDiffuseTex->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(defaultDiffuseTex, &srvDesc, hDescriptor);
}

void PickApp::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<PickAppFR::FrameResource>(md3dDevice,
			1, (uint)mAllRitems.size(), (uint)mMaterials.size()));
	}
}

void PickApp::BuildMaterials()
{
	auto gray0 = std::make_unique<Material>();
	gray0->Name = "gray0";
	gray0->MatCBIndex = 0;
	gray0->DiffuseSrvHeapIndex = 0;
	gray0->DiffuseAlbedo.set(0.7f, 0.7f, 0.7f, 1.0f);
	gray0->FresnelR0.set(0.04f, 0.04f, 0.04f);
	gray0->Roughness = 0.0f;

	auto highlight0 = std::make_unique<Material>();
	highlight0->Name = "highlight0";
	highlight0->MatCBIndex = 1;
	highlight0->DiffuseSrvHeapIndex = 0;
	highlight0->DiffuseAlbedo.set(1.0f, 1.0f, 0.0f, 0.6f);
	highlight0->FresnelR0.set(0.06f, 0.06f, 0.06f);
	highlight0->Roughness = 0.0f;


	mMaterials["gray0"] = std::move(gray0);
	mMaterials["highlight0"] = std::move(highlight0);
}

void PickApp::BuildRenderItems()
{
	auto carRitem = std::make_unique<RenderItem>();
	carRitem->World.translate({ 0.0f, 1.0f, 0.0f });
	carRitem->ObjCBIndex = 0;
	carRitem->Mat = mMaterials["gray0"].get();
	carRitem->Geo = mGeometries["carGeo"].get();
	carRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	carRitem->box = carRitem->Geo->DrawArgs["car"].Bounds;
	carRitem->IndexCount = carRitem->Geo->DrawArgs["car"].IndexCount;
	carRitem->StartIndexLocation = carRitem->Geo->DrawArgs["car"].StartIndexLocation;
	carRitem->BaseVertexLocation = carRitem->Geo->DrawArgs["car"].BaseVertexLocation;
	mRitemLayer[(int)RenderLayer::Opaque].push_back(carRitem.get());

	auto pickedRitem = std::make_unique<RenderItem>();
	pickedRitem->ObjCBIndex = 1;
	pickedRitem->Mat = mMaterials["highlight0"].get();
	pickedRitem->Geo = mGeometries["carGeo"].get();
	pickedRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Picked triangle is not visible until one is picked.
	pickedRitem->Visible = false;

	// DrawCall parameters are filled out when a triangle is picked.
	pickedRitem->IndexCount = 0;
	pickedRitem->StartIndexLocation = 0;
	pickedRitem->BaseVertexLocation = 0;
	mPickedRitem = pickedRitem.get();
	mRitemLayer[(int)RenderLayer::Highlight].push_back(pickedRitem.get());


	mAllRitems.push_back(std::move(carRitem));
	mAllRitems.push_back(std::move(pickedRitem));
}

void PickApp::BuildCarGeometry()
{
	TexModel model;
	LoadModleSkull("Models/car.txt", model);

	auto totalVertexCount =
		model.vertex.size();

	std::vector<PickAppFR::Vertex> vertices(totalVertexCount);

	uint k = 0;
	for (size_t i = 0; i < model.vertex.size(); ++i, ++k)
	{
		vertices[k].Pos = model.vertex[i].Pos;
		vertices[k].Normal = model.vertex[i].Normal;
		vertices[k].TexC = {0.0, 0.0};
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(model.indices), std::end(model.indices));

	const uint vbByteSize = (uint)vertices.size() * sizeof(PickAppFR::Vertex);
	const uint ibByteSize = (uint)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "carGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice,
		mCommandList, indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(PickAppFR::Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	for (size_t i = 0; i < model.vertex.size(); ++i)
	{
		submesh.Bounds.merge(vertices[i].Pos);
	}

	geo->DrawArgs["car"] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void PickApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
	uint objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PickAppFR::ObjectConstants));

	auto objectCB = mCurrFrameResource->ObjectCB->Resource();

	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];

		if (ri->Visible == false)
			continue;

		cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;

		cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);

		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

void PickApp::Pick(int sx, int sy)
{	// 转换到物体的局部空间做相交检测
	Math::Mat4 P = mCamera.GetProj();
#if 0
	// Compute picking ray in view space.
	float vx = (+2.0f * sx / mClientWidth - 1.0f) / P.m[0];
	float vy = (-2.0f * sy / mClientHeight + 1.0f) / P.m[5];

	// Ray definition in view space.
	Math::Vec4 rayOrigin(0.0f, 0.0f, 0.0f, 1.0f);
	Math::Vec4 rayDir(vx, vy, 1.0f, 0.0f);

	Math::Mat4 V = mCamera.GetView();
	Math::Mat4 invView = mCamera.GetView().getInversed();

	// Assume nothing is picked to start, so the picked render-item is invisible.
	mPickedRitem->Visible = false;

	// Check if we picked an opaque render item.  A real app might keep a separate "picking list"
	// of objects that can be selected.   
	for (auto ri : mRitemLayer[(int)RenderLayer::Opaque])
	{
		auto geo = ri->Geo;

		// Skip invisible render-items.
		if (ri->Visible == false)
			continue;

		Math::Mat4 W = ri->World;
		Math::Mat4 invWorld = W.getInversed();

		// Tranform ray to vi space of Mesh.
		Math::Mat4 toLocal = invWorld * invView;

		rayOrigin *= toLocal;
		rayDir *= toLocal;

		// Make the ray direction unit length for the intersection tests.
		Math::Vec3 rayDir3 = Math::Vec3(rayDir.x, rayDir.y, rayDir.z).getNormalized();

		// If we hit the bounding box of the Mesh, then we might have picked a Mesh triangle,
		// so do the ray/triangle tests.
		//
		// If we did not hit the bounding box, then it is impossible that we hit 
		// the Mesh, so do not waste effort doing ray/triangle tests.
		float tmin = 0.0f;
		if (ri->box.Intersects(rayOrigin, rayDir, tmin))
		{
			// NOTE: For the demo, we know what to cast the vertex/index data to.  If we were mixing
			// formats, some metadata would be needed to figure out what to cast it to.
			auto vertices = (PickAppFR::Vertex*)geo->VertexBufferCPU->GetBufferPointer();
			auto indices = (std::uint32_t*)geo->IndexBufferCPU->GetBufferPointer();
			UINT triCount = ri->IndexCount / 3;

			// Find the nearest ray/triangle intersection.
			tmin = 1.0;
			for (UINT i = 0; i < triCount; ++i)
			{
				// Indices for this triangle.
				UINT i0 = indices[i * 3 + 0];
				UINT i1 = indices[i * 3 + 1];
				UINT i2 = indices[i * 3 + 2];

				// Vertices for this triangle.
				const Math::Vec3& v0 = vertices[i0].Pos;
				const Math::Vec3& v1 = vertices[i1].Pos;
				const Math::Vec3& v2 = vertices[i2].Pos;

				// We have to iterate over all the triangles in order to find the nearest intersection.
				float t = 0.0f;
				if (TriangleTests::Intersects(rayOrigin, rayDir, v0, v1, v2, t))
				{
					if (t < tmin)
					{
						// This is the new nearest picked triangle.
						tmin = t;
						UINT pickedTriangle = i;

						mPickedRitem->Visible = true;
						mPickedRitem->IndexCount = 3;
						mPickedRitem->BaseVertexLocation = 0;

						// Picked render item needs same world matrix as object picked.
						mPickedRitem->World = ri->World;
						mPickedRitem->NumFramesDirty = gNumFrameResources;

						// Offset to the picked triangle in the mesh index buffer.
						mPickedRitem->StartIndexLocation = 3 * pickedTriangle;
					}
				}
			}
		}
	}
#endif
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> PickApp::GetStaticSamplers()
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

#pragma once

#include "../common/D3DUtil.h"
#include "../common/UploadBuffer.h"
#include "../math/Mat4.h"

namespace LandAndWavesFR {

struct ObjectConstants
{
    Math::Mat4 World{};
};

struct PassConstants
{
	Math::Mat4 View{};
	Math::Mat4 InvView{};
	Math::Mat4 Proj{};
	Math::Mat4 InvProj{};
	Math::Mat4 ViewProj{};
	Math::Mat4 InvViewProj{};
    Math::Vec3 EyePosW = { 0.0f, 0.0f, 0.0f };
    float cbPerObjectPad1 = 0.0f;
    Math::Vec2 RenderTargetSize = { 0.0f, 0.0f };
    Math::Vec2 InvRenderTargetSize = { 0.0f, 0.0f };
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
};

struct Vertex
{
    Math::Vec3 Pos;
    Math::Color Color;
};

// Stores the resources needed for the CPU to build the command lists
// for a frame.  
struct FrameResource
{
public:
    
	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount);
    FrameResource(const FrameResource& rhs) = delete;
    FrameResource& operator=(const FrameResource& rhs) = delete;
    ~FrameResource();

    // We cannot reset the allocator until the GPU is done processing the commands.
    // So each frame needs their own allocator.
    ID3D12CommandAllocator* CmdListAlloc{};

    // We cannot update a cbuffer until the GPU is done processing the commands
    // that reference it.  So each frame needs their own cbuffers.
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
    std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

	std::unique_ptr<UploadBuffer<Vertex>> WavesVB = nullptr;

    // Fence value to mark commands up to this fence point.  This lets us
    // check if these frame resources are still in use by the GPU.
    UINT64 Fence = 0;
};

}
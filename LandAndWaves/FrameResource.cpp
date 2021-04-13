#include "FrameResource.h"

namespace LandAndWavesFR {

	FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount)
	{
		ThrowIfFailed(device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&CmdListAlloc)));

		PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
		ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);

		WavesVB = std::make_unique<UploadBuffer<Vertex>>(device, waveVertCount, false);
	}

	FrameResource::~FrameResource()
	{

	}

}
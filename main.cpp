

#include "InitDirect3DApp/InitDirect3DApp.h"
#include "BoxApp/BoxApp.h"
#include "BoxAppTwoSlot/BoxAppTwoSlot.h"
#include "ShapesApp/ShapesApp.h"
#include "ShapesSkullApp/ShapesSkullApp.h"
#include "LandAndWaves/LandAndWaves.h"
#include "LitWaves/LitWaves.h"
#include "LitColumns/LitColumns.h"
#include "TexColumns/TexColumns.h"
#include "TexBox/TexBox.h"
#include "TexWaves/TexWaves.h"
#include "BlendWaves/BlendWaves.h"
#include "MultiTexBox/MultiTexBox.h"
#include "StencilDemo/StencilDemo.h"
#include "CameraAndDynamicIndexingApp/CameraAndDynamicIndexingApp.h"
#include "InstancingAndCulling/InstancingAndCulling.h"
#include "CubeMap/CubeMap.h"
#include "DynamicCube/DynamicCube.h"
#include "NormalMap/NormalMap.h"
#include "PickApp/PickApp.h"
#include "ShadowMap/ShadowMapApp.h"
#include "QuatApp/QuatApp.h"
#include "SkinnedMesh/SkinnedMesh.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	try
	{
		//InitDirect3DApp theApp(hInstance);
		//BoxApp theApp(hInstance);
		//BoxAppTwoSlot theApp(hInstance);
		//ShapesApp theApp(hInstance);
		//ShapesSkullApp theApp(hInstance);
		//LandAndWaves theApp(hInstance);
		//LitWaves theApp(hInstance);
		//LitColumns theApp(hInstance);
		//TexBox theApp(hInstance);
		//TexWaves theApp(hInstance);
		//TexColumns theApp(hInstance);
		//BlendWaves theApp(hInstance);
		//MultiTexBox theApp(hInstance);
		//StencilDemo theApp(hInstance);
		//CameraAndDynamicIndexingApp theApp(hInstance);
		//InstancingAndCulling theApp(hInstance);		// TODO : ??׶???޳?
		//CubeMap theApp(hInstance);
		//DynamicCube theApp(hInstance); 
		//NormalMap theApp(hInstance);
		//PickApp theApp(hInstance);
		ShadowMapApp theApp(hInstance);
		//QuatApp theApp(hInstance);
		//SkinnedMesh theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

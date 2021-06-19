
#pragma once

#include "FlagSet.h"

NS_JYE_BEGIN

// Graphics capability support level. Web platform (Emscripten) also uses OpenGL ES, but is considered a desktop platform capability-wise
#ifdef PLATFORM_MOBILE
#define MOBILE_GRAPHICS
#else
#define DESKTOP_GRAPHICS
#endif

enum APIVersion : uint
{
	RHI_None,
	RHI_DirectX12,
	RHI_Vulkan
};

/// Primitive type.
enum class PrimitiveType : uint
{
    TRIANGLE_LIST = 0,
    LINE_LIST,
    POINT_LIST,
    TRIANGLE_STRIP,
    LINE_STRIP,
    TRIANGLE_FAN
};

/// Blending mode.
enum class BlendMode : uint
{
	BL_ZERO = 0,
	BL_ONE,
	BL_SRC_COLOR,
	BL_ONE_MINUS_SRC_COLOR,
	BL_SRC_ALPHA,
	BL_ONE_MINUS_SRC_ALPHA,
	BL_DST_ALPHA,
	BL_ONE_MINUS_DST_ALPHA,
	BL_DST_COLOR,
	BL_ONE_MINUS_DST_COLOR,
	BL_Factor,
	BL_InvFactor,
};

enum class BlendOperator : uint
{
	BO_Add,
	BO_Subtract,
	BO_ReverseSubtract,
	BO_Min,
	BO_Max
};

enum class ColorMask : uint
{
	CM_None = 0,
	CM_Red = 0x1,
	CM_Green = 0x2,
	CM_Blue = 0x4,
	CM_Alpha = 0x8,
	CM_All = CM_Red | CM_Green | CM_Blue | CM_Alpha,
};
JYE_FLAGSET(ColorMask, ColorMaskFlags);

/// Depth or stencil compare mode.
enum class CompareOperator : uint
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};

enum class FrontFace : uint
{
	CounterClockwise,	// 逆时针
	Clockwise			// 顺时针
};

/// Culling mode.
enum class CullMode : uint
{
    CULL_NONE = 0,
    CULL_FRONT,
    CULL_BACK,
};

/// Fill mode.
enum class FillMode : uint
{
    FILL_SOLID = 0,
    FILL_WIREFRAME,
    FILL_POINT
};

/// Stencil operation.
enum class StencilOp : uint
{
    OP_KEEP = 0,
    OP_ZERO,
    OP_REF,
    OP_INCR,
    OP_DECR
};


/// Texture filtering mode.
enum class TextureFilterMode : uint
{
	MinPointMagPointMipPoint = 0,
	MinPointMagPointMipLinear = 1,
	MinPointMagLinearMipPoint = 2,
	MinPointMagLinearMipLinear = 3,
	MinLinearMagPointMipPoint = 4,
	MinLinearMagPointMipLinear = 5,
	MinLinearMagLinearMipPoint = 6,
	MinLinearMagLinearMipLinear = 7,
	Anisotropy = 8
};

/// Texture addressing mode.
enum class TextureAddressMode : uint
{
	TW_REPEAT = 0,
	TW_CLAMP_TO_EDGE,
	TW_MIRRORED_REPEAT,
	TW_CLAMP_TO_BORDER,
};

/// Cube map faces.
enum class CubeMapFace : uint
{
	FACE_POSITIVE_X = 0,
	FACE_NEGATIVE_X,
	FACE_POSITIVE_Y,
	FACE_NEGATIVE_Y,
	FACE_POSITIVE_Z,
	FACE_NEGATIVE_Z,
	MAX_CUBEMAP_FACES
};

enum class TextureType : uint
{
	TT_TEXTURE1D,
	TT_TEXTURE2D,
	TT_TEXTURE3D,
	TT_TEXTURECUBE,
};

enum class TextureRefUsage : uint
{
	Common,
	CubeMap
};

enum class TextureUseage : uint
{
	TU_STATIC = 0,
	TU_READ = 1,//从显存读取到内存
	TU_WRITE = 2,//从内存写到显存
	TU_RT_WRITE_ONLY = 3,//仅显存可见，用于depthrenderbuffer
	TU_COMPUTEWRITE = 4,
};

enum class BorderColor : uint
{
	TransparentBlack,
	OpaqueBlack,
	OpaqueWhite
};

enum class PolygonMode : uint
{
    PM_POINT = 0,
    PM_LINE,
    PM_TRIANGLE,
};

enum class StencilOperator : uint
{
	Keep,
	Zero,
	Replace,
	IncrementAndClamp,
	DecrementAndClamp,
	Invert,
	IncrementAndWrap,
	DecrementAndWrap
};

enum class PixelFormat : int
{
    PF_AUTO = -1,//自动根据纹理格式变换

	PF_A8,
	PF_L8,

	PF_L8A8,
	PF_YUV420P, 
	PF_R8G8B8,//8_8_8
	PF_R5G6B5,//5_6_5
	PF_R8G8B8A8,//8_8_8_8
	PF_R4G4B4A4,//4_4_4_4
	PF_DEPTH16,
	PF_DEPTH32,//not on mobile device;
	PF_DEPTH24_STENCIL8,

	// floating point texture format
	PF_RGBAFLOAT,//32_32_32_32 floating	
	PF_RGBAHALF,//16_16_16_16 floating
	PF_RG11B10FLOAT,//11_11_10 floating

	PF_R16_FLOAT,
	PF_R32_UINT,
	PF_R32_FLOAT,

	PF_ETC1_RGB8,  
	PF_PVRTC_RGB4,
	PF_PVRTC_RGBA4,
	PF_PVRTC2_4BPP,
	PF_PVRTC_RGBA2,
	PF_PVRTC2_2BPP,
	PF_PVRTC_RGB2,
};

struct GraphicsDataType
{
	enum Enum
	{
		DT_FLOAT = 0,
		DT_HALF_FLOAT,
		DT_INT_8_8_8_8,
		DT_UINT_8_8_8_8,
		DT_INT_2_10_10_10,
		DT_UINT_10_10_10_2,
	};
};

enum class IndexType : uint 
{
	UInt32,
	UInt16
};

/// Shader types.
enum class ShaderType : uint
{
	ST_NONE = 0,
    ST_VS,
	ST_PS,
	ST_CS,
};

enum class ClearTarget : uint
{
    CLEAR_COLOR = 0x1,
    CLEAR_DEPTH = 0x2,
    CLEAR_STENCIL = 0x4,
};
JYE_FLAGSET(ClearTarget, ClearTargetFlags);

enum class ResourceType : uint
{
	RT_Buffer,
	RT_Texture,
	RT_GroupBuffer
};

enum class ShaderVisibility : uint
{
	All,
	Vertex,
	Pixel,
};

enum class MultiSample : uint
{
	Count1 = 1,
	Count2 = 2,
	Count4 = 4,
	Count8 = 8,
	Count16 = 16,
	Count32 = 32
};

enum class Dimension : uint
{
	Dimension1D,
	Dimension2D,
	Dimension3D,
};

enum class MemoryHeap : uint
{
	Default,
	Upload
};

enum class ResourceLayout : uint
{
	GeneralRead,
	RenderTarget,
	DepthStencil,
	CopyDestination,
	CopySource,
	Present
};

enum class ResourceUsage : uint
{
	None = 0,
	VertexBuffer = 1 << 0,
	IndexBuffer = 1 << 1,
	ConstantBuffer = 1 << 2,
	RenderTarget = 1 << 3,
	DepthStencil = 1 << 4
};
JYE_FLAGSET(ResourceUsage, ResourceUsageFlags);

enum class AttachmentLoad : uint
{
	Clear,
	Load,
	DontCare
};

enum class AttachmentStore : uint
{
	Store,
	DontCare,
};

enum class PipelineBindPoint : uint
{
	Graphic,
	Computer,
};

struct StencilOperatorInfo
{
	CompareOperator mCompareOperator = CompareOperator::Always;
	StencilOperator FailOperator = StencilOperator::Keep;
	StencilOperator PassOperator = StencilOperator::Keep;
	StencilOperator DepthFailOperator = StencilOperator::Keep;

	StencilOperatorInfo() = default;
};







NS_JYE_END

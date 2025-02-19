#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define DELETE_UNVAILUBLE(key) if(key) { delete key; key = nullptr; }

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define WHEEL_UP CKeyMgr::GetInst()->IsMouseWheelUp()
#define WHEEL_DOWN CKeyMgr::GetInst()->IsMouseWheelDown()

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER	32
#define MAX_MIP		8

#define NaN std::numeric_limits<float>::quiet_NaN()

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

#define xAlloc(size)		PoolAllocator::Alloc(size)
#define xrelease(ptr)		PoolAllocator::Release(ptr)

#include "ObjectPool.h"
#define onew(object) ObjectPool<object>::Pop();
//#define odelete(object) ObjectPool<decltype(object)>::Push(&object);
template<typename T>
void odelete(T * object) {
	ObjectPool<T>::Push(object);
}


enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera
	TEXTCOMP,			// Text

	BEHAVIORTREE,
	FINDPATH,
	
	// UI
	UICOMPONENT,

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	SKYBOX,			// SkyBox
	DECAL,			// 내부 렌더링
		
	END,

	// custom
	SCRIPT,
};

enum Component_Flags_
{
	_NONE = 0,
	_TRANSFORM = 1 << 0,
	_COLLIDER2D = 1 << 1,
	_COLLIDER3D = 1 << 2,
	_ANIMATOR2D = 1 << 3,
	_ANIMATOR3D = 1 << 4,
	_LIGHT2D = 1 << 5,
	_LIGHT3D = 1 << 6,
	_CAMERA = 1 << 7,
	_TEXT = 1 << 8,
	_BEHAVIORTREE = 1 << 9,
	_FINDPATH = 1 << 10,
	_UICOMPONENT = 1 << 11,
	_MESHRENDER = 1 << 12,
	_PARTICLESYSTEM = 1 << 13,
	_TILEMAP = 1 << 14,
	_LANDSCAPE = 1 << 15,
	_SKYBOX = 1 << 16,
	_DECAL = 1 << 17,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];

enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,
	BONE,			// 애니메이션 BONE

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	ITEX_0,
	ITEX_1,
	ITEX_2,
	ITEX_3,
	ITEX_4,
	ITEX_5,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,	

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,	
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합

	DEFEREED_DECAL_BLEND, // 0 타겟은 AlphaBlend, 1 타겟은 ONE-ONE Blend
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,	
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	DOMAIN_DEFERRED,		// 지연 렌더링 오브젝트
	DOMAIN_DEFERRED_DECAL,	// Deferred Decal(광원 적용 가능한 Decal)

	DOMAIN_OPAQUE,			// 불투명 오브젝트
	DOMAIN_MASK,			// 불투명, 완전 투명
	DOMAIN_DECAL,			// 데칼 오브젝트
	DOMAIN_TRANSPARENT,		// 반투명
	DOMAIN_POSTPROCESS,		// 후 처리
	DOMAIN_UI,

	DOMAIN_LIGHT,			// 광원 타입 
	DOMAIN_UNDEFINED,		// 미정
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	CREATE_OBJECT_TO_PARENT, // wParam : ChildObject, lParam : ParentObject
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	LEVEL_CHANGE,	
};


enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	FRUSTUM,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
	END,
};

enum class COLLIDER3D_TYPE
{
	SPHERE,
	CUBE,
	END,
};
extern const char* COLLIDER2D_TYPE_STR[(UINT)COLLIDER2D_TYPE::END];
extern const wchar_t* COLLIDER2D_TYPE_WSTR[(UINT)COLLIDER2D_TYPE::END];

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,	// 파티클 생성
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	DUMMY_3,

	END,
};

enum class MRT_TYPE
{
	SWAPCHAIN,

	DEFERRED,

	DEFERRED_DECAL,

	LIGHT,

	SHADOWMAP,

	_3DANIM_EDIT,

	_UI_EDIT,

	END,
};

enum class FONT
{
	KBIZM,
	Muli_SemiBold,
	Roboto_Regularl,
	SairaSemiCondensed_Regular,
	END,
};
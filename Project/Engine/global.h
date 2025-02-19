#pragma once

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

using std::queue;
using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;
using std::unordered_map;

#include <typeinfo>
#include <assert.h>

using std::type_info;

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;



#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader header
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// Fbx Loader
#include <FBXLoader/fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif


#include "define.h"
#include "struct.h"
#include "func.h"

#include "CSingleton.h"

#include "debug.h"
#include <bitset>
#include <set>
#include <iomanip>
#include <thread>
#include <mutex>

//메모리 매니저 관련
#include "MemoryMgr.h"
#include "CustomContainer.h"
#include "ObjectPool.h"

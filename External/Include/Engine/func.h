#pragma once
#include "MemoryPool.h"
#include "MemoryMgr.h"
#include "CEntity.h"
#include <mutex>

extern mutex m_aFuncMutex;

template <typename T, UINT _Size>
void DeleteArray(T* (&Arr)[_Size])
{
	for (UINT i = 0; i < _Size; ++i)
	{
		if (nullptr != Arr[i])
			delete Arr[i];
	}
}

// 오브젝트 생성
class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, const wstring& _LayerName);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName);
void SpawnGameObjectToParent(CGameObject* _NewObject, CGameObject* _ParentObject);
void SpawnChlidGameObject(CGameObject* _ParentObject, const wstring& _LayerName);

// 오브젝트 삭제
void DestroyObject(CGameObject* _DeletObject);

// 컴포넌트 추가
void AddComponents(CGameObject* _Object, int CreateCompType);

// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCube(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);


void DrawDebugFrustumCube(const Matrix& _matWorld, bool DepthTest);

// GameObject 유효성 체크
bool IsValidObj(CGameObject*& _Target);


const char* ToString(RES_TYPE);
const wchar_t* ToWSTring(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWSTring(COMPONENT_TYPE);

// Relative Path 가져오기
wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath);
const char* ToString(COLLIDER2D_TYPE);
const wchar_t* ToWString(COLLIDER2D_TYPE);

COLLIDER2D_TYPE ToCollider2DType(wstring _wstring);

string ToString(wstring _wstring);
wstring ToWString(string _string);

string ToUpper(string _string);
wstring ToUpper(wstring _wstring);

// Save / Load
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);

int GetSizeofFormat(DXGI_FORMAT _eFormat);

class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>
void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;	
	fread(&i, sizeof(i), 1, _File);
	
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}


void SaveGameObjectPtr(CGameObject* _Obj, FILE* _File);
void LoadGameObjectPtr(wstring& _ObjName, FILE* _File);

const wchar_t* ToWString(COMPONENT_TYPE type);

std::wstring Vec3ToWString(const Vec3& vec);
Vec3 WStringToVec3(const std::wstring& wstr);

std::wstring IntArrayToWString(const std::vector<int>& intArray);
std::vector<int> WStringToIntArray(const std::wstring& str);




//일반적인 벡터 삭제
template<typename T, UINT Size, typename std::enable_if<!std::is_base_of<CEntity, T>::value, T>::type* = nullptr>
void Safe_Del_Array(T* (&arr)[Size]) {
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
			delete arr[i];
	}
}

// CEntity 또는 그 파생 클래스인 경우
template<typename T, UINT Size, typename std::enable_if<std::is_base_of<CEntity, T>::value, T>::type* = nullptr>
void Safe_Del_Array(T* (&arr)[Size]) {
	for (UINT i = 0; i < Size; ++i)
	{
		if (arr[i] == nullptr)
			continue;

		if (static_cast<CEntity*>(arr[i])->GetManagedByMemory()) {
			static_cast<CEntity*>(arr[i])->SetManagedByMemory(false);
			//xdelete<CEntity>(item); // 메모리 풀 버전
			odelete(arr[i]); //오브젝트 풀 버전
		}
		else {
			delete arr[i];
		}
	}
}

// template<typename T, UINT Size>
// void Safe_Del_Array(T* (&arr)[Size])
// {
// 	for (UINT i = 0; i < Size; ++i)
// 	{
// 		if (nullptr != arr[i])
// 			delete arr[i];
// 	}	
// }


//template<typename T>
//void Safe_Del_Vec(vector<T*>& _vec)
//{
//	for (size_t i = 0; i < _vec.size(); ++i)
//	{
//		if (nullptr != _vec[i])
//		{
//			delete _vec[i];
//		}
//	}
//	_vec.clear();
//}

//일반적인 벡터 삭제
template<typename T, typename std::enable_if<!std::is_base_of<CEntity, T>::value, T>::type* = nullptr>
void Safe_Del_Vec(std::vector<T*>& vec) {
	for (auto& item : vec) {
		delete item;
	}
	vec.clear();
}

// CEntity 또는 그 파생 클래스인 경우
template<typename T, typename std::enable_if<std::is_base_of<CEntity, T>::value, T>::type* = nullptr>
void Safe_Del_Vec(std::vector<T*>& vec) {
	for (auto& item : vec) {
		if (static_cast<CEntity*>(item)->GetManagedByMemory()) {
			static_cast<CEntity*>(item)->SetManagedByMemory(false);
			//xdelete<CEntity>(item); // 메모리 풀 버전
			odelete(item); //오브젝트 풀 버전
		}
		else {
			delete item;
		}
	}
	vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}


// math
float Rad2Deg(float _radian); 
float Deg2Rad(float _Degree); 

float roundToDecimal(double value, int decimalPlaces);

float CTruncate(float value, int decimalPlaces);


//MemoryMgr Init
extern class MemoryMgr* GMemory;

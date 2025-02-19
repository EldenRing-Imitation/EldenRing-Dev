#include "pch.h"
#include "CFBXLoader.h"
#include "CMesh.h"

#include "CResMgr.h"
#include "CPathMgr.h"


#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using std::experimental::filesystem::path;

void GenerateUVForMesh(FbxMesh* pMesh) {
	// 메쉬에서 최초의 폴리곤을 대상으로 UV 레이어 생성
	FbxGeometryElementUV* lUVElem = pMesh->CreateElementUV("UVSet_01");

	// 데이터 매핑 모드 설정: 폴리곤-정점(Polygon Vertex) 매핑
	lUVElem->SetMappingMode(FbxGeometryElement::eByControlPoint);

	// 데이터 참조 모드 설정: 직접 참조
	lUVElem->SetReferenceMode(FbxGeometryElement::eDirect);

	// 폴리곤 개수와 각 폴리곤을 구성하는 정점의 개수를 기반으로 UV 좌표 생성
	for (int polyIndex = 0; polyIndex < pMesh->GetPolygonCount(); ++polyIndex) {
		int polySize = pMesh->GetPolygonSize(polyIndex);
	
		for (int vertIndex = 0; vertIndex < polySize; ++vertIndex) {
			// 임시로 UV 좌표를 생성 (예시로서 단순화된 계산)
			FbxVector2 uv(static_cast<double>(vertIndex % 3), static_cast<double>((vertIndex / 3) % 3));

			// UV 세트에 UV 좌표 추가
			lUVElem->GetDirectArray().Add(uv);
		}
	}
}

CFBXLoader::CFBXLoader()
	: m_pManager(NULL)
	, m_pScene(NULL)
	, m_pImporter(NULL)
	, m_iContainerCnt(-1)
{
}

CFBXLoader::~CFBXLoader()
{
	m_pScene->Destroy();
	m_pManager->Destroy();

	for (size_t i = 0; i < m_vecBone.size(); ++i)
	{
		if (nullptr != m_vecBone[i])
			delete m_vecBone[i];
	}

	for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
	{
		if (nullptr != m_vecAnimClip[i])
			delete m_vecAnimClip[i];
	}

	for (int i = 0; i < m_arrAnimName.Size(); ++i)
	{
		if (nullptr != m_arrAnimName[i])
			delete m_arrAnimName[i];
	}
}

void CFBXLoader::init()
{
	m_pManager = FbxManager::Create();
	if (NULL == m_pManager)
		assert(NULL);

	FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(pIOSettings);

	m_pScene = FbxScene::Create(m_pManager, "");
	if (NULL == m_pScene)
		assert(NULL);

	m_iContainerCnt = -1;
}

void CFBXLoader::LoadFbx(const wstring& _strPath)
{
	m_vecContainer.clear();

	m_pImporter = FbxImporter::Create(m_pManager, "");

	//wstring str = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(strName.c_str());
	string strPath(_strPath.begin(), _strPath.end());

	if (!m_pImporter->Initialize(strPath.c_str(), -1, m_pManager->GetIOSettings()))
		assert(nullptr);

	m_pImporter->Import(m_pScene); // FBX 폴더에 fbm 폴더를 생성

	/*FbxAxisSystem originAxis = FbxAxisSystem::eMax;
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem DesireAxis = FbxAxisSystem::DirectX;
	DesireAxis.ConvertScene(m_pScene);
	originAxis = m_pScene->GetGlobalSettings().GetAxisSystem();*/

	m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	// Bone 정보 읽기
	LoadSkeleton(m_pScene->GetRootNode());

	// Animation 이름정보 
	m_pScene->FillAnimStackNameArray(m_arrAnimName);

	// Animation Clip 정보
	LoadAnimationClip();

	// 삼각화(Triangulate)
	Triangulate(m_pScene->GetRootNode());

	// 메쉬 데이터 얻기
	LoadMeshDataFromNode(m_pScene->GetRootNode()->GetChild(0));

	m_pImporter->Destroy();

	// 필요한 텍스쳐 로드
	LoadTexture();

	// 필요한 메테리얼 생성
	CreateMaterial();
}

void CFBXLoader::LoadMeshDataFromNode(FbxNode* _pNode, int _iParentIdx)
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();
	int iChildCnt = _pNode->GetChildCount();
	int iThisIdx = 0;
	bool bGroupNode = 0;

	if (pAttr && FbxNodeAttribute::eMesh == pAttr->GetAttributeType())
	{
		FbxAMatrix matGlobal = _pNode->EvaluateGlobalTransform();
		matGlobal.GetR();

		fbxsdk::FbxMesh* pMesh = _pNode->GetMesh();
		if (NULL != pMesh)
		{
			pMesh->SetName(_pNode->GetName());
			LoadMesh(pMesh);
			iThisIdx = m_iContainerCnt;
			
			// 그룹노드 여부
			m_vecContainer.back().bGroupNode = 0;

			// Parent Idx
			m_vecContainer.back().iParentIdx = _iParentIdx;
			
			// 해당 메시의 Transform 정보 받아오기
			LoadTransfrom(_pNode);
		}
	}
	else if (!pAttr && 0 < iChildCnt)
	{
		m_vecContainer.push_back(tContainer{});
		m_iContainerCnt++; // 컨테이너 갯수 추가
		tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

		iThisIdx = m_iContainerCnt;

		// 그룹노드 여부
		m_vecContainer.back().bGroupNode = 1;

		// Parent Idx
		m_vecContainer.back().iParentIdx = _iParentIdx;

		string strName = _pNode->GetName();
		Container.strName = wstring(strName.begin(), strName.end());

		// 해당 메시의 Transform 정보 받아오기
		LoadTransfrom(_pNode);
	}

	// 해당 노드의 재질정보 읽기
	UINT iMtrlCnt = _pNode->GetMaterialCount();
	if (iMtrlCnt > 0)
	{
		for (UINT i = 0; i < iMtrlCnt; ++i)
		{
			FbxSurfaceMaterial* pMtrlSur = _pNode->GetMaterial(i);
			LoadMaterial(pMtrlSur);
		}
	}

	// 자식 노드 정보 읽기
	
	for (int i = 0; i < iChildCnt; ++i)
	{
		LoadMeshDataFromNode(_pNode->GetChild(i), iThisIdx);
	}
}

void CFBXLoader::LoadMesh(FbxMesh* _pFbxMesh)
{
	m_vecContainer.push_back(tContainer{});
	m_iContainerCnt++; // 컨테이너 갯수 추가
	tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

	string strName = _pFbxMesh->GetName();
	Container.strName = wstring(strName.begin(), strName.end());

	int iVtxCnt = _pFbxMesh->GetControlPointsCount();

	// FbxMesh에 있는 기존 정점정보
	vector<Vec3> VtxElement{};
	VtxElement.resize(iVtxCnt);

	FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();

	for (int i = 0; i < iVtxCnt; ++i)
	{
		VtxElement[i].x = (float)pFbxPos[i].mData[0];
		VtxElement[i].y = (float)pFbxPos[i].mData[2];
		VtxElement[i].z = (float)pFbxPos[i].mData[1];
	}

	// 폴리곤 갯수
	int iPolyCnt = _pFbxMesh->GetPolygonCount();

	// 폴리곤을 구성하는 정점 개수
	int iPolySize = _pFbxMesh->GetPolygonSize(0);
	if (3 != iPolySize)
		assert(NULL); // Polygon 구성 정점이 3개가 아닌 경우
	
	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
	Container.vecIdx.resize(iMtrlCnt);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

	// 정점과 UV인덱스 동기화 여부
	bool VtxSynchronization = false;

	// 다각형의 각 정점마다 텍스쳐좌표지정. 정점당 다수의 UV를 가질 수 있어, 정점 갯수를 UV인덱스 갯수와 일치시킨다.
	if (_pFbxMesh->GetElementUV()->GetMappingMode() == fbxsdk::FbxLayerElement::eByPolygonVertex)
	{
		iVtxCnt = iPolyCnt * iPolySize;
		VtxSynchronization = true;
	}
	else
	{
		Container.Resize(iVtxCnt);
		for (int i = 0; i < iVtxCnt; ++i)
			Container.vecPos[i] = VtxElement[i];
	}

	Container.Resize(iVtxCnt);

	UINT arrIdx[3] = {};
	UINT iVtxOrder = 0; // 폴리곤 순서로 접근하는 순번
		
	// 정점과 UV인덱스를 동기화함
	if (VtxSynchronization)
	{
		for (int i = 0; i < iPolyCnt; ++i)
		{
			for (int j = 0; j < iPolySize; ++j)
			{
				// i 번째 폴리곤에, j 번째 정점
				int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
				int _iUVIndex = _pFbxMesh->GetTextureUVIndex(i, j);

				// 정점 생성
				Container.vecPos[iVtxOrder] = VtxElement[iIdx]; // 정점위치정보
				Container.vecTangent[iVtxOrder] = GetTangent(_pFbxMesh, nullptr, iIdx, iVtxOrder);
				Container.vecNormal[iVtxOrder] = GetNormal(_pFbxMesh, nullptr, iIdx, iVtxOrder);
				Container.vecBinormal[iVtxOrder] = GetBinormal(_pFbxMesh, nullptr, iIdx, iVtxOrder);
				Container.vecUV[iVtxOrder] = GetUV(_pFbxMesh, nullptr, _iUVIndex, iVtxOrder, iIdx);

				// 인덱스는 지금 추가한 정점의 순서
				arrIdx[j] = iVtxOrder;
				++iVtxOrder;
			}
			UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
		}
	}
	else
	{
		for (int i = 0; i < iPolyCnt; ++i)
		{
			for (int j = 0; j < iPolySize; ++j)
			{
				// i 번째 폴리곤에, j 번째 정점
				int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
				int _iUVIndex = _pFbxMesh->GetTextureUVIndex(i, j);
				arrIdx[j] = iIdx;

				GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder);
				GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
				GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
				GetUV(_pFbxMesh, &Container, _iUVIndex, iVtxOrder, iIdx);

				++iVtxOrder;
			}
			UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
			Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
		}
	}
	LoadAnimationData(_pFbxMesh, &Container);
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* _pMtrlSur)
{
	tFbxMaterial tMtrlInfo{};

	string str = _pMtrlSur->GetName();
	tMtrlInfo.strMtrlName = wstring(str.begin(), str.end());

	// Diff
	tMtrlInfo.tMtrl.vDiff = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sDiffuse
		, FbxSurfaceMaterial::sDiffuseFactor);

	// Amb
	tMtrlInfo.tMtrl.vAmb = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sAmbient
		, FbxSurfaceMaterial::sAmbientFactor);

	// Spec
	tMtrlInfo.tMtrl.vSpec = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sSpecular
		, FbxSurfaceMaterial::sSpecularFactor);

	// Emisv
	tMtrlInfo.tMtrl.vEmv = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sEmissive
		, FbxSurfaceMaterial::sEmissiveFactor);

	// Texture Name
	tMtrlInfo.strDiff = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);
	tMtrlInfo.strNormal = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
	tMtrlInfo.strSpec = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sSpecular);
	tMtrlInfo.strEmis = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sEmissive);


	m_vecContainer.back().vecMtrl.push_back(tMtrlInfo);
}

void CFBXLoader::LoadTransfrom(FbxNode* _pNode)
{
	FbxDouble3 translation = _pNode->LclTranslation;
	FbxDouble3 rotation = _pNode->LclRotation;
	FbxDouble3 scaling = _pNode->LclScaling;

	Vec3 lclTranslation = {};
	Vec3 lclRotation= {};
	Vec3 lclScaling= {};

	wstring NodeName = m_vecContainer.back().strName;

	// unity : translation.x -> -1.f;
	translation.mData[0] *= -1.f;

	// euler -> quaternion


	// // unity :: rotation.z -> -1.f;
	rotation.mData[2] += 180.f;
	// 
	// // unity rotation-> -1.f;
	rotation.mData[1] += 180.f;
	
	// unity tranlation -> / 100.f;
	translation.mData[0] /= 100.f;
	translation.mData[1] /= 100.f;
	translation.mData[2] /= 100.f;


	// Local translation
	lclTranslation.x = roundToDecimal(translation.mData[0], 3);
	lclTranslation.y = roundToDecimal(translation.mData[1], 3);
	lclTranslation.z = roundToDecimal(translation.mData[2], 3);

	// Local rotation
	lclRotation.x = roundToDecimal(rotation.mData[0],3);
	lclRotation.y = roundToDecimal(rotation.mData[1],3);
	lclRotation.z = roundToDecimal(rotation.mData[2],3);

	// Local scaling
	lclScaling.x = roundToDecimal(scaling.mData[0],3);
	lclScaling.y = roundToDecimal(scaling.mData[1],3);
	lclScaling.z = roundToDecimal(scaling.mData[2],3);

	m_vecContainer.back().tLocalTransform.translation = lclTranslation;
	m_vecContainer.back().tLocalTransform.rotation = lclRotation;
	m_vecContainer.back().tLocalTransform.scaling = lclScaling;
}

Vec3 CFBXLoader::GetTangent(FbxMesh* _pMesh
	, tContainer* _pContainer
	, int _iIdx		 /*해당 정점의 인덱스*/
	, int _iVtxOrder /*폴리곤 단위로 접근하는 순서*/)
{
	int iTangentCnt = _pMesh->GetElementTangentCount();
	
	//if (1 != iTangentCnt)
	//	assert(NULL); // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	FbxVector4 avgTangent(0, 0, 0, 0);
	for (int i = 0; i < iTangentCnt; ++i) {
		// 탄젠트 data 의 시작 주소
		FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
		UINT iTangentIdx = 0;

		if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iVtxOrder;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
		}
		else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				iTangentIdx = _iIdx;
			else
				iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
		}
		FbxVector4 vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);
		avgTangent += vTangent;
	}

	// 탄젠트 평균 계산
	if (iTangentCnt > 0) {
		avgTangent /= static_cast<double>(iTangentCnt);
	}

	// 정규화
	avgTangent.Normalize();

	if (_pContainer) {
		// 컨테이너에 저장
		_pContainer->vecTangent[_iIdx].x = static_cast<float>(avgTangent.mData[0]);
		_pContainer->vecTangent[_iIdx].y = static_cast<float>(avgTangent.mData[1]);
		_pContainer->vecTangent[_iIdx].z = static_cast<float>(avgTangent.mData[2]);
	}	

	return Vec3((float)avgTangent.mData[0], (float)avgTangent.mData[2], (float)avgTangent.mData[1]);
}

Vec3 CFBXLoader::GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iBinormalCnt = _pMesh->GetElementBinormalCount();
	if (1 != iBinormalCnt)
	{		
		if (_pContainer)
		{
			DirectX::XMVECTOR tangentVec = _pContainer->vecTangent[_iIdx];
			DirectX::XMVECTOR normalVec = _pContainer->vecNormal[_iIdx];

			DirectX::XMVECTOR binormalVec = DirectX::XMVector3Cross(tangentVec, normalVec);
			DirectX::XMStoreFloat3(&_pContainer->vecBinormal[_iIdx], binormalVec);
			return Vec3();
		}
		else {
			int iBinormalCnt = _pMesh->GetElementBinormalCount();
			FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
			DirectX::XMVECTOR binormalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			UINT iBinormalIdx = 0;

			for (int i = 0; i < iBinormalCnt; ++i)
			{
				// 인덱스 결정 로직
				if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
						iBinormalIdx = _iVtxOrder;
					else
						iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
				}
				else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
						iBinormalIdx = _iIdx;
					else
						iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
				}

				FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);
				DirectX::XMVECTOR currentBinormal = DirectX::XMVectorSet((float)vBinormal.mData[0], (float)vBinormal.mData[1], (float)vBinormal.mData[2], 0.0f);
				binormalSum = DirectX::XMVectorAdd(binormalSum, currentBinormal);
			}
			// 평균 계산
			binormalSum = DirectX::XMVectorScale(binormalSum, 1.0f / iBinormalCnt);
			DirectX::XMFLOAT3 result;
			DirectX::XMStoreFloat3(&result, binormalSum);
			return Vec3(result.x, result.y, result.z);
		}
	}

	// 종법선 data 의 시작 주소
	FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
	UINT iBinormalIdx = 0;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iVtxOrder;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iIdx;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);

	if (_pContainer)
	{
		_pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
		_pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
		_pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
	}

	return Vec3((float)vBinormal.mData[0], (float)vBinormal.mData[2], (float)vBinormal.mData[1]);
}

Vec3 CFBXLoader::GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iNormalCnt = _pMesh->GetElementNormalCount();
	if (1 != iNormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
	UINT iNormalIdx = 0;

	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);
	
	if (_pContainer)
	{
		_pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
		_pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
		_pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
	}

	return Vec3((float)vNormal.mData[0], (float)vNormal.mData[2], (float)vNormal.mData[1]);
}



Vec2 CFBXLoader::GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iVtxId, int _iVtxCnt, int _iIdx)
{
	// UV 레이어의 수 확인
	int uvLayerCount = _pMesh->GetElementUVCount();

	if (uvLayerCount == 0) {
		GenerateUVForMesh(_pMesh);
		//assert(false);
	}

	FbxGeometryElementUV* pUV = _pMesh->GetElementUV();
	FbxGeometryElement::EMappingMode MappingMode = pUV->GetMappingMode();
	FbxGeometryElement::EReferenceMode RefMode = FbxGeometryElement::eDirect;

	UINT iUVIdx = 0;
	
	switch (MappingMode)
	{
	case fbxsdk::FbxLayerElement::eNone:
		// 모드 설정 되어있지 않음
		break;
	case fbxsdk::FbxLayerElement::eByControlPoint:
	{
		if (RefMode == FbxGeometryElement::eDirect)
			iUVIdx = _iIdx;
		else if (RefMode == FbxGeometryElement::eIndexToDirect)
			iUVIdx = pUV->GetIndexArray().GetAt(_iIdx);
		break;
	}
	case fbxsdk::FbxLayerElement::eByPolygonVertex:
	{
		if (RefMode == FbxGeometryElement::eDirect)
			iUVIdx = _iVtxId;
		else if (RefMode == FbxGeometryElement::eIndexToDirect)
			iUVIdx = pUV->GetIndexArray().GetAt(_iVtxCnt);
		break;
	}
	case fbxsdk::FbxLayerElement::eByPolygon:
		break;
	case fbxsdk::FbxLayerElement::eByEdge:
		break;
	case fbxsdk::FbxLayerElement::eAllSame:
		break;
	}


	FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);

	if (_pContainer)
	{
		_pContainer->vecUV[_iIdx].x = (float)vUV.mData[0];
		_pContainer->vecUV[_iIdx].y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
	}

	return Vec2((float)vUV.mData[0], 1.f - (float)vUV.mData[1]);
}

Vec4 CFBXLoader::GetMtrlData(FbxSurfaceMaterial* _pSurface
	, const char* _pMtrlName
	, const char* _pMtrlFactorName)
{
	FbxDouble3  vMtrl;
	FbxDouble	dFactor = 0.;

	FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
	FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

	if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
	{
		vMtrl = tMtrlProperty.Get<FbxDouble3>();
		dFactor = tMtrlFactorProperty.Get<FbxDouble>();
	}

	Vec4 vRetVal = Vec4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
	return vRetVal;
}

wstring CFBXLoader::GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
{
	string strName;

	FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
	if (TextureProperty.IsValid())
	{
		UINT iCnt = TextureProperty.GetSrcObjectCount();

		if (1 <= iCnt)
		{
			FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
			if (NULL != pFbxTex)
				strName = pFbxTex->GetFileName();
		}
	}

	return wstring(strName.begin(), strName.end());
}

void CFBXLoader::LoadTexture()
{
	path path_content = CPathMgr::GetInst()->GetContentPath();

	path path_fbx_texture = path_content.wstring() + L"texture\\FBXTexture\\";
	if (false == exists(path_fbx_texture))
	{
		create_directory(path_fbx_texture);
	}

	path path_origin;
	path path_filename;
	path path_dest;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{
			vector<path> vecPath;
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strDiff.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strSpec.c_str());
			vecPath.push_back(m_vecContainer[i].vecMtrl[j].strEmis.c_str());

			for (size_t k = 0; k < vecPath.size(); ++k)
			{
				if (vecPath[k].filename().empty())
					continue;

				path_origin = vecPath[k];
				path_filename = vecPath[k].filename();
				path_dest = path_fbx_texture.wstring() + path_filename.wstring();

				if (false == exists(path_dest))
				{
					copy(path_origin, path_dest);
				}

				path_dest = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), path_dest);
				CResMgr::GetInst()->Load<CTexture>(path_filename, path_dest);

				switch (k)
				{
				case 0: m_vecContainer[i].vecMtrl[j].strDiff = path_filename; break;
				case 1: m_vecContainer[i].vecMtrl[j].strNormal = path_filename; break;
				case 2: m_vecContainer[i].vecMtrl[j].strSpec = path_filename; break;
				case 3: m_vecContainer[i].vecMtrl[j].strEmis = path_filename; break;
				}
			}
		}
	}

	path_origin = path_origin.parent_path();
	remove_all(path_origin);
}

void CFBXLoader::CreateMaterial()
{
	wstring strMtrlName;
	wstring strPath;
	wstring strKey;

	for (UINT i = 0; i < m_vecContainer.size(); ++i)
	{
		for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
		{
			// Material 이름짓기
			strMtrlName = m_vecContainer[i].vecMtrl[j].strMtrlName;
			if (strMtrlName.empty())
				strMtrlName = path(m_vecContainer[i].vecMtrl[j].strDiff).stem();

			strPath = L"material\\";
			strKey = strMtrlName + L".mtrl";
			strPath += strKey;

			// 재질 이름
			m_vecContainer[i].vecMtrl[j].strMtrlName = strKey;

			// 이미 로딩된 재질이면 로딩된 것을 사용
			Ptr<CMaterial> pMaterial = CResMgr::GetInst()->FindRes<CMaterial>(strKey);
			if (nullptr != pMaterial)
				continue;

			pMaterial = new CMaterial;

			pMaterial->SetKey(strKey);
			pMaterial->SetRelativePath(strPath);

			pMaterial->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));

			wstring strTexKey = m_vecContainer[i].vecMtrl[j].strDiff;
			Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_0, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strNormal;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_1, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strSpec;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_2, pTex);

			strTexKey = m_vecContainer[i].vecMtrl[j].strEmis;
			pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexKey);
			if (NULL != pTex)
				pMaterial->SetTexParam(TEX_PARAM::TEX_3, pTex);


			pMaterial->SetMaterialCoefficient(
				m_vecContainer[i].vecMtrl[j].tMtrl.vDiff
				, m_vecContainer[i].vecMtrl[j].tMtrl.vSpec
				, m_vecContainer[i].vecMtrl[j].tMtrl.vAmb
				, m_vecContainer[i].vecMtrl[j].tMtrl.vEmv);
		
			CResMgr::GetInst()->AddRes<CMaterial>(pMaterial->GetKey(), pMaterial.Get());
			pMaterial->Save(strPath);
		}
	}
}

void CFBXLoader::LoadSkeleton(FbxNode* _pNode)
{
	int iChildCount = _pNode->GetChildCount();

	LoadSkeleton_Re(_pNode, 0, 0, -1);
}

void CFBXLoader::LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		tBone* pBone = new tBone;

		string strBoneName = _pNode->GetName();

		pBone->strBoneName = wstring(strBoneName.begin(), strBoneName.end());
		pBone->iDepth = _iDepth++;
		pBone->iParentIndx = _iParentIdx;

		m_vecBone.push_back(pBone);
	}

	int iChildCount = _pNode->GetChildCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		LoadSkeleton_Re(_pNode->GetChild(i), _iDepth, (int)m_vecBone.size(), _iIdx);
	}
}

void CFBXLoader::LoadAnimationClip()
{
	int iAnimCount = m_arrAnimName.GetCount();

	for (int i = 0; i < iAnimCount; ++i)
	{
		FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());


		//FbxAnimEvaluator* pevaluator = m_pScene->GetAnimationEvaluator();
		//m_pScene->SetCurrentAnimationStack();


		if (!pAnimStack)
			continue;

		tAnimClip* pAnimClip = new tAnimClip;

		string strClipName = pAnimStack->GetName();
		pAnimClip->strName = wstring(strClipName.begin(), strClipName.end());

		FbxTakeInfo* pTakeInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());
		pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

		pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) - pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);



		m_vecAnimClip.push_back(pAnimClip);
	}
}

void CFBXLoader::Triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr &&
		(pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(m_pManager);
		converter.Triangulate(pAttr, true);
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		Triangulate(_pNode->GetChild(i));
	}
}

void CFBXLoader::LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer)
{
	// Animation Data 로드할 필요가 없음
	int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iSkinCount <= 0 || m_vecAnimClip.empty())
		return;

	_pContainer->bAnimation = true;

	// Skin 개수만큼 반복을하며 읽는다.	
	for (int i = 0; i < iSkinCount; ++i)
	{
		FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

		if (pSkin)
		{
			FbxSkin::EType eType = pSkin->GetSkinningType();
			if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
			{
				// Cluster 를 얻어온다
				// Cluster == Joint == 관절
				int iClusterCount = pSkin->GetClusterCount();

				for (int j = 0; j < iClusterCount; ++j)
				{
					FbxCluster* pCluster = pSkin->GetCluster(j);

					if (!pCluster->GetLink())
						continue;

					// 현재 본 인덱스를 얻어온다.
					int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());
					if (-1 == iBoneIdx)
						assert(NULL);

					FbxAMatrix matNodeTransform = GetTransform(_pMesh->GetNode());

					// Weights And Indices 정보를 읽는다.
					LoadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);

					// Bone 의 OffSet 행렬 구한다.
					LoadOffsetMatrix(pCluster, matNodeTransform, iBoneIdx, _pContainer);

					// Bone KeyFrame 별 행렬을 구한다.
					LoadKeyframeTransform(_pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx, _pContainer);
				}
			}
		}
	}
	CheckWeightAndIndices(_pMesh, _pContainer);
}


void CFBXLoader::CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer)
{
	vector<vector<tWeightsAndIndices>>::iterator iter = _pContainer->vecWI.begin();

	int iVtxIdx = 0;
	for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
	{
		if ((*iter).size() > 1)
		{
			// 가중치 값 순으로 내림차순 정렬
			sort((*iter).begin(), (*iter).end()
				, [](const tWeightsAndIndices& left, const tWeightsAndIndices& right)
				{
					return left.dWeight > right.dWeight;
				}
			);

			double dWeight = 0.f;
			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				dWeight += (*iter)[i].dWeight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*iter)[0].dWeight += revision;
			}

			if ((*iter).size() >= 4)
			{
				(*iter).erase((*iter).begin() + 4, (*iter).end());
			}
		}

		// 정점 정보로 변환, 
		float fWeights[4] = {};
		float fIndices[4] = {};

		for (UINT i = 0; i < (*iter).size(); ++i)
		{
			fWeights[i] = (float)(*iter)[i].dWeight;
			fIndices[i] = (float)(*iter)[i].iBoneIdx;
		}

		memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(Vec4));
		memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(Vec4));
	}
}

FbxQuaternion CFBXLoader::EulerToQuaternion(const FbxVector4& euler)
{
	double cy = cos(euler[2] * 0.5);
	double sy = sin(euler[2] * 0.5);
	double cr = cos(euler[0] * 0.5);
	double sr = sin(euler[0] * 0.5);
	double cp = cos(euler[1] * 0.5);
	double sp = sin(euler[1] * 0.5);

	FbxQuaternion quaternion;
	quaternion[0] = cy * cr * cp + sy * sr * sp;
	quaternion[1] = cy * sr * cp - sy * cr * sp;
	quaternion[2] = cy * cr * sp + sy * sr * cp;
	quaternion[3] = sy * cr * cp - cy * sr * sp;

	return quaternion;
}

void CFBXLoader::LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster
	, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer)
{
	if (m_vecAnimClip.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	m_vecBone[_iBoneIdx]->matBone = _matNodeTransform;

	FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

	FbxLongLong llStartFrame = m_vecAnimClip[0]->tStartTime.GetFrameCount(eTimeMode);
	FbxLongLong llEndFrame = m_vecAnimClip[0]->tEndTime.GetFrameCount(eTimeMode);

	for (FbxLongLong i = llStartFrame; i < llEndFrame; ++i)
	{
		tKeyFrame tFrame = {};
		FbxTime   tTime = 0;

		tTime.SetFrame(i, eTimeMode);

		FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
		FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
		matCurTrans = matReflect * matCurTrans * matReflect;

		tFrame.dTime = tTime.GetSecondDouble();
		tFrame.matTransform = matCurTrans;

		m_vecBone[_iBoneIdx]->vecKeyFrame.push_back(tFrame);
	}
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* _pCluster
	, const FbxAMatrix& _matNodeTransform
	, int _iBoneIdx, tContainer* _pContainer)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;

	_pCluster->GetTransformMatrix(matClusterTrans);
	_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;
	matOffset = matReflect * matOffset * matReflect;

	m_vecBone[_iBoneIdx]->matOffset = matOffset;
}


void CFBXLoader::LoadWeightsAndIndices(FbxCluster* _pCluster
	, int _iBoneIdx
	, tContainer* _pContainer)
{
	int iIndicesCount = _pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iIndicesCount; ++i)
	{
		tWeightsAndIndices tWI = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		tWI.iBoneIdx = _iBoneIdx;
		tWI.dWeight = _pCluster->GetControlPointWeights()[i];

		int iVtxIdx = _pCluster->GetControlPointIndices()[i];

		_pContainer->vecWI[iVtxIdx].push_back(tWI);
	}
}



int CFBXLoader::FindBoneIndex(string _strBoneName)
{
	wstring strBoneName = wstring(_strBoneName.begin(), _strBoneName.end());

	for (UINT i = 0; i < m_vecBone.size(); ++i)
	{
		if (m_vecBone[i]->strBoneName == strBoneName)
			return i;
	}

	return -1;
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* _pNode)
{
	const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(vT, vR, vS);
}

#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CResMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CFBXLoader.h"


CMeshData::CMeshData(bool _bEngine)
	: CRes(RES_TYPE::MESHDATA, _bEngine)
{
}

CMeshData::~CMeshData()
{
	Safe_Del_Vec(m_vecMeshData);
}


CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = nullptr;

	int iMeshDataCnt = (int)m_vecMeshData.size();
	
	// 단일메시
	if (1 == iMeshDataCnt)
	{
		pNewObj = onew(CGameObject);
		AddComponents(pNewObj, _TRANSFORM | _MESHRENDER);
		pNewObj->MeshRender()->SetMesh(m_vecMeshData[0]->m_pMesh);

		//vector<tIndexInfo> idxInfos = pNewObj->MeshRender()->GetMesh()->GetIdxInfo();
		//idxInfos[0].iIdxCount /= 3;
		//
		//pNewObj->MeshRender()->GetMesh()->SetIdxInfo(idxInfos);

		for (UINT i = 0; i < m_vecMeshData[0]->m_vecMtrl.size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(m_vecMeshData[0]->m_vecMtrl[i], i);
		}
	}

	// 복수메시
	else if (1 < iMeshDataCnt)
	{
		vector<CGameObject*> vecObj;
		
		for (size_t i = 0; i < iMeshDataCnt; ++i)
		{
			CGameObject* pNewChildObj = onew(CGameObject);

			pNewChildObj->SetName(m_vecMeshData[i]->m_strObjName);
			AddComponents(pNewChildObj, _TRANSFORM);
			pNewChildObj->Transform()->SetAbsolute(false);

			// 렌더 오브젝트
			if (!(m_vecMeshData[i]->m_bGroupObj))
			{
				AddComponents(pNewChildObj, _MESHRENDER);
				pNewChildObj->MeshRender()->SetMesh(m_vecMeshData[i]->m_pMesh);

				for (UINT j = 0; j < m_vecMeshData[i]->m_vecMtrl.size(); ++j)
				{
					pNewChildObj->MeshRender()->SetMaterial(m_vecMeshData[i]->m_vecMtrl[j], j);
				}
			}

			Vec3 translation = m_vecMeshData[i]->m_tLocalTransform.translation;
			Vec3 rotation = m_vecMeshData[i]->m_tLocalTransform.rotation;
			Vec3 scaling = m_vecMeshData[i]->m_tLocalTransform.scaling;

			pNewChildObj->Transform()->SetRelativePos(translation);
			pNewChildObj->Transform()->SetRelativeRot(rotation);
			pNewChildObj->Transform()->SetRelativeScale(scaling);

			if (0 == i) // 가장 첫번째 노드
				pNewObj = pNewChildObj;
			else
				vecObj[m_vecMeshData[i]->m_ParentIdx]->AddChild(pNewChildObj);

			vecObj.emplace_back(pNewChildObj);
		}
	}

	// Animation 파트 추가
	InstantiateAnimation(pNewObj);


	return pNewObj;
}

void CMeshData::Instantiate(CGameObject* _obj)
{
	CGameObject* pNewObj = _obj;

	int iMeshDataCnt = (int)m_vecMeshData.size();

	// 단일메시
	if (1 == iMeshDataCnt)
	{
		AddComponents(pNewObj, _TRANSFORM | _MESHRENDER);
		pNewObj->MeshRender()->SetMesh(m_vecMeshData[0]->m_pMesh);

		//vector<tIndexInfo> idxInfos = pNewObj->MeshRender()->GetMesh()->GetIdxInfo();
		//idxInfos[0].iIdxCount /= 3;
		//
		//pNewObj->MeshRender()->GetMesh()->SetIdxInfo(idxInfos);

		for (UINT i = 0; i < m_vecMeshData[0]->m_vecMtrl.size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(m_vecMeshData[0]->m_vecMtrl[i], i);
		}
	}

	// 복수메시
	else if (1 < iMeshDataCnt)
	{
		vector<CGameObject*> vecObj;

		for (size_t i = 0; i < iMeshDataCnt; ++i)
		{
			CGameObject* pNewChildObj = onew(CGameObject);

			pNewChildObj->SetName(m_vecMeshData[i]->m_strObjName);
			AddComponents(pNewChildObj, _TRANSFORM);
			pNewChildObj->Transform()->SetAbsolute(false);

			// 렌더 오브젝트
			if (!(m_vecMeshData[i]->m_bGroupObj))
			{
				AddComponents(pNewChildObj, _MESHRENDER);
				pNewChildObj->MeshRender()->SetMesh(m_vecMeshData[i]->m_pMesh);

				for (UINT j = 0; j < m_vecMeshData[i]->m_vecMtrl.size(); ++j)
				{
					pNewChildObj->MeshRender()->SetMaterial(m_vecMeshData[i]->m_vecMtrl[j], j);
				}
			}

			Vec3 translation = m_vecMeshData[i]->m_tLocalTransform.translation;
			Vec3 rotation = m_vecMeshData[i]->m_tLocalTransform.rotation;
			Vec3 scaling = m_vecMeshData[i]->m_tLocalTransform.scaling;

			pNewChildObj->Transform()->SetRelativePos(translation);
			pNewChildObj->Transform()->SetRelativeRot(rotation);
			pNewChildObj->Transform()->SetRelativeScale(scaling);

			if (0 == i) // 가장 첫번째 노드
				pNewObj = pNewChildObj;
			else
				vecObj[m_vecMeshData[i]->m_ParentIdx]->AddChild(pNewChildObj);

			vecObj.emplace_back(pNewChildObj);
		}
	}

	// Animation 파트 추가
	InstantiateAnimation(pNewObj);
}

void CMeshData::InstantiateAnimation(CGameObject* _NewObj)
{
	wstring strMeshName = path(GetKey()).stem();

	int AnimClipCnt = 0;

	map<wstring, Ptr<CRes>> mapBone = CResMgr::GetInst()->GetResources(RES_TYPE::BONE);
	map<wstring, Ptr<CRes>>::iterator iter = mapBone.begin();

	vector<Ptr<CBone>> vectorBone;


	int KeyLen = (int)strMeshName.length();

	if (iter == mapBone.end())
		return;

	for (; iter != mapBone.end(); ++iter)
	{
		wstring strBoneKey = iter->first.substr(0, KeyLen);

		if (strMeshName == strBoneKey)
		{
			vectorBone.emplace_back((CBone*)iter->second.Get());
			AnimClipCnt++;
		}
	}

	if (0 < AnimClipCnt)
	{
		CAnimator3D* pAnimator = onew(CAnimator3D);
		_NewObj->AddComponent(pAnimator);

		vector<Ptr<CBone>>::iterator iter = vectorBone.begin();
		while(iter != vectorBone.end())
			pAnimator->AddAnim(*iter++);
		
		// Message 
		// wstring errMsg = L"Load Compleate Animation : " + std::to_wstring(AnimClipCnt);
		// MessageBox(nullptr, errMsg.c_str(), L"Success!", MB_OK);
			
		return;
	}
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath, int singleMeshData)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	int iContainerCnt = loader.GetContainerCount();	// 컨테이너 갯수 확인

	CMeshData* pMeshData = new CMeshData(true);

	if (singleMeshData)
		iContainerCnt = 1;

	for (int i = 0; i < iContainerCnt; ++i)
	{
		// 컨테이너 받아오기
		const tContainer* container = &loader.GetContainer(i);

		CMesh* pMesh = nullptr;
		vector<Ptr<CMaterial>> vecMtrl;

		tTransformInfo			tlocalTransform = {};
		// Transform 정보 받아오기.
		{
			tlocalTransform = container->tLocalTransform;
			// rotation 정보 degree -> radian으로 변환
			tlocalTransform.rotation.x = Deg2Rad(tlocalTransform.rotation.x);
			tlocalTransform.rotation.y = Deg2Rad(tlocalTransform.rotation.y);
			tlocalTransform.rotation.z = Deg2Rad(tlocalTransform.rotation.z);
		}

		if (!container->bGroupNode)
		{
			// 메쉬 가져오기
			pMesh = CMesh::CreateFromContainer(loader, i);


			// ResMgr 에 메쉬 등록
			if (nullptr != pMesh)
			{
				wstring strMeshKey;
				// strMeshKey += path(strFullPath).stem();
				strMeshKey = pMesh->GetName();
				strMeshKey += L".mesh";

				if (nullptr == CResMgr::GetInst()->FindRes<CMesh>(strMeshKey))
				{
					CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);

					// 메시를 실제 파일로 저장
					pMesh->Save(strMeshKey);
				}
				else
				{
					pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey).Get();
				}

			}

			// 메테리얼 가져오기
			for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
			{
				// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
				Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
				assert(pMtrl.Get());

				vecMtrl.push_back(pMtrl);
			}
		}

		tMeshData* MeshData = new tMeshData;
		MeshData->m_strObjName = container->strName;
		MeshData->m_pMesh = pMesh;
		MeshData->m_vecMtrl = vecMtrl;
		MeshData->m_ParentIdx = container->iParentIdx;
		MeshData->m_bGroupObj = container->bGroupNode;
		MeshData->m_tLocalTransform = tlocalTransform;
		pMeshData->m_vecMeshData.emplace_back(MeshData);
	}

	return pMeshData;
}

int CMeshData::Save(const wstring& _strRelativePath)
{
	// 상대경로 저장
	wstring RelativePath = L"meshdata\\";
	RelativePath += _strRelativePath;
	SetRelativePath(RelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_meshdata = path_content.wstring() + L"meshdata\\";

	if (false == exists(path_meshdata))
	{
		create_directory(path_meshdata);
	}

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 메시데이터 갯수 저장
	UINT iMeshDataCnt = (UINT)m_vecMeshData.size();
	fwrite(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	// Mesh Key 저장	
	// Mesh Data 저장
	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		// MeshData Name
		SaveWString(m_vecMeshData[i]->m_strObjName, pFile);

		// Group 여부
		fwrite(&m_vecMeshData[i]->m_bGroupObj, sizeof(int), 1, pFile);

		// ParentIndx 정보 저장
		fwrite(&m_vecMeshData[i]->m_ParentIdx, sizeof(int), 1, pFile);
		
		// Transform 정보 저장
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.translation, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.rotation, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.scaling, sizeof(Vec3), 1, pFile);
		
		if (!(m_vecMeshData[i]->m_bGroupObj))
		{
			// Mesh 정보 저장
			SaveResRef(m_vecMeshData[i]->m_pMesh.Get(), pFile);

			// material 정보 저장
			UINT iMtrlCount = (UINT)m_vecMeshData[i]->m_vecMtrl.size();
			fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

			UINT iIdx = 0;
			wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
			strMtrlPath += L"material\\";

			for (; iIdx < iMtrlCount; ++iIdx)
			{
				if (nullptr == m_vecMeshData[i]->m_vecMtrl[iIdx])
					continue;

				// Material 인덱스, Key, Path 저장
				fwrite(&iIdx, sizeof(UINT), 1, pFile);
				SaveResRef(m_vecMeshData[i]->m_vecMtrl[iIdx].Get(), pFile);
			}
		}


		// iIdx = -1; // 마감 값
		// fwrite(&iIdx, sizeof(UINT), 1, pFile);
	}

	fclose(pFile);

	return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	assert(pFile);

	// MeshData 갯수 읽기
	UINT iMeshDataCnt = 0;
	fread(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		tMeshData* MeshData = new tMeshData;

		// MeshData Name
		LoadWString(MeshData->m_strObjName, pFile);

		// Group 여부
		fread(&MeshData->m_bGroupObj, sizeof(int), 1, pFile);

		// ParentIndx Load
		fread(&MeshData->m_ParentIdx, sizeof(int), 1, pFile);

		// Transform Load
		fread(&MeshData->m_tLocalTransform.translation, sizeof(Vec3), 1, pFile);
		fread(&MeshData->m_tLocalTransform.rotation, sizeof(Vec3), 1, pFile);
		fread(&MeshData->m_tLocalTransform.scaling, sizeof(Vec3), 1, pFile);

		if (!(MeshData->m_bGroupObj))
		{

			// Mesh Load
			Ptr<CMesh> pMesh = nullptr;
			LoadResRef<CMesh>(pMesh, pFile);
			assert(pMesh.Get());
			MeshData->m_pMesh = pMesh;


			// material 정보 읽기
			vector<Ptr<CMaterial>> vecMtrl;

			// 재질 갯수
			UINT iMtrlCount = 0;
			fread(&iMtrlCount, sizeof(UINT), 1, pFile);


			vecMtrl.resize(iMtrlCount);

			for (UINT i = 0; i < iMtrlCount; ++i)
			{
				UINT idx = -1;
				fread(&idx, 4, 1, pFile);
				if (idx == -1)
					break;

				wstring strKey, strPath;

				Ptr<CMaterial> pMtrl;
				LoadResRef<CMaterial>(pMtrl, pFile);

				vecMtrl[i] = pMtrl;
			}

			MeshData->m_vecMtrl = vecMtrl;
		}

		m_vecMeshData.emplace_back(MeshData);
	}

	fclose(pFile);

	return S_OK;
}
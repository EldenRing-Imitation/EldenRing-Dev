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
	
	// ���ϸ޽�
	if (1 == iMeshDataCnt)
	{
		pNewObj = new CGameObject;
		pNewObj->AddComponent(new CTransform);
		pNewObj->AddComponent(new CMeshRender);
		pNewObj->MeshRender()->SetMesh(m_vecMeshData[0]->m_pMesh);

		for (UINT i = 0; i < m_vecMeshData[0]->m_vecMtrl.size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(m_vecMeshData[0]->m_vecMtrl[i], i);
		}
	}

	// �����޽�
	else if (1 < iMeshDataCnt)
	{
		vector<CGameObject*> vecObj;
		
		for (size_t i = 0; i < iMeshDataCnt; ++i)
		{
			CGameObject* pNewChildObj = new CGameObject;

			pNewChildObj->SetName(m_vecMeshData[i]->m_strObjName);
			pNewChildObj->AddComponent(new CTransform);

			// ���� ������Ʈ
			if (!(m_vecMeshData[i]->m_bGroupObj))
			{
				pNewChildObj->AddComponent(new CMeshRender);
				pNewChildObj->MeshRender()->SetMesh(m_vecMeshData[i]->m_pMesh);

				for (UINT j = 0; j < m_vecMeshData[i]->m_vecMtrl.size(); ++j)
				{
					pNewChildObj->MeshRender()->SetMaterial(m_vecMeshData[i]->m_vecMtrl[j], j);
				}
			}

			pNewChildObj->Transform()->SetRelativePos(m_vecMeshData[i]->m_tLocalTransform.translation);
			pNewChildObj->Transform()->SetRelativeRot(m_vecMeshData[i]->m_tLocalTransform.rotation);
			pNewChildObj->Transform()->SetRelativeScale(m_vecMeshData[i]->m_tLocalTransform.scaling);

			if (0 == i) // ���� ù��° ���
				pNewObj = pNewChildObj;
			else
				vecObj[m_vecMeshData[i]->m_ParentIdx]->AddChild(pNewChildObj);

			vecObj.emplace_back(pNewChildObj);
		}
	}

	// Animation ��Ʈ �߰�
	// if (false == m_pMesh->IsAnimMesh())
	// 	return pNewObj;

	// CAnimator3D* pAnimator = new CAnimator3D;
	// pNewObj->AddComponent(pAnimator);
	// 
	// pAnimator->SetBones(m_pMesh->GetBones());
	// pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _strPath, int singleMeshData)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	int iContainerCnt = loader.GetContainerCount();	// �����̳� ���� Ȯ��

	CMeshData* pMeshData = new CMeshData(true);

	if (singleMeshData)
		iContainerCnt = 1;

	for (int i = 0; i < iContainerCnt; ++i)
	{
		// �����̳� �޾ƿ���
		const tContainer* container = &loader.GetContainer(i);

		CMesh* pMesh = nullptr;
		vector<Ptr<CMaterial>> vecMtrl;

		tTransformInfo			tlocalTransform = {};
		tTransformInfo			tglobalTransform = {};
		// Transform ���� �޾ƿ���.
		tlocalTransform = container->tLocalTransform;
		tglobalTransform = container->tGlobalTransform;

		if (!container->bGroupNode)
		{
			// �޽� ��������
			pMesh = CMesh::CreateFromContainer(loader, i);


			// ResMgr �� �޽� ���
			if (nullptr != pMesh)
			{
				wstring strMeshKey;
				// strMeshKey += path(strFullPath).stem();
				strMeshKey = pMesh->GetName();
				strMeshKey += L".mesh";

				if (nullptr == CResMgr::GetInst()->FindRes<CMesh>(strMeshKey))
				{
					CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);

					// �޽ø� ���� ���Ϸ� ����
					pMesh->Save(strMeshKey);
				}
				else
				{
					pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey).Get();
				}

			}

			// ���׸��� ��������
			for (UINT j = 0; j < loader.GetContainer(i).vecMtrl.size(); ++j)
			{
				// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
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
		MeshData->m_tGlobalTransform = tglobalTransform;
		pMeshData->m_vecMeshData.emplace_back(MeshData);
	}

	return pMeshData;
}

int CMeshData::Save(const wstring& _strRelativePath)
{
	// ����� ����
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

	// �޽õ����� ���� ����
	UINT iMeshDataCnt = (UINT)m_vecMeshData.size();
	fwrite(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	// Mesh Key ����	
	// Mesh Data ����
	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		// MeshData Name
		SaveWString(m_vecMeshData[i]->m_strObjName, pFile);

		// Group ����
		fwrite(&m_vecMeshData[i]->m_bGroupObj, sizeof(int), 1, pFile);

		// ParentIndx ���� ����
		fwrite(&m_vecMeshData[i]->m_ParentIdx, sizeof(int), 1, pFile);
		
		// Transform ���� ����
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.translation, sizeof(Vec4), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.rotation, sizeof(Vec4), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tLocalTransform.scaling, sizeof(Vec4), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tGlobalTransform.translation, sizeof(Vec4), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tGlobalTransform.rotation, sizeof(Vec4), 1, pFile);
		fwrite(&m_vecMeshData[i]->m_tGlobalTransform.scaling, sizeof(Vec4), 1, pFile);
		
		if (!(m_vecMeshData[i]->m_bGroupObj))
		{
			// Mesh ���� ����
			SaveResRef(m_vecMeshData[i]->m_pMesh.Get(), pFile);

			// material ���� ����
			UINT iMtrlCount = (UINT)m_vecMeshData[i]->m_vecMtrl.size();
			fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

			UINT iIdx = 0;
			wstring strMtrlPath = CPathMgr::GetInst()->GetContentPath();
			strMtrlPath += L"material\\";

			for (; iIdx < iMtrlCount; ++iIdx)
			{
				if (nullptr == m_vecMeshData[i]->m_vecMtrl[iIdx])
					continue;

				// Material �ε���, Key, Path ����
				fwrite(&iIdx, sizeof(UINT), 1, pFile);
				SaveResRef(m_vecMeshData[i]->m_vecMtrl[iIdx].Get(), pFile);
			}
		}


		// iIdx = -1; // ���� ��
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

	// MeshData ���� �б�
	UINT iMeshDataCnt = 0;
	fread(&iMeshDataCnt, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iMeshDataCnt; ++i)
	{
		tMeshData* MeshData = new tMeshData;

		// MeshData Name
		LoadWString(MeshData->m_strObjName, pFile);

		// Group ����
		fread(&MeshData->m_bGroupObj, sizeof(int), 1, pFile);

		// ParentIndx Load
		fread(&MeshData->m_ParentIdx, sizeof(int), 1, pFile);

		// Transform Load
		fread(&MeshData->m_tLocalTransform.translation, sizeof(Vec4), 1, pFile);
		fread(&MeshData->m_tLocalTransform.rotation, sizeof(Vec4), 1, pFile);
		fread(&MeshData->m_tLocalTransform.scaling, sizeof(Vec4), 1, pFile);
		fread(&MeshData->m_tGlobalTransform.translation, sizeof(Vec4), 1, pFile);
		fread(&MeshData->m_tGlobalTransform.rotation, sizeof(Vec4), 1, pFile);
		fread(&MeshData->m_tGlobalTransform.scaling, sizeof(Vec4), 1, pFile);

		if (!(MeshData->m_bGroupObj))
		{

			// Mesh Load
			Ptr<CMesh> pMesh = nullptr;
			LoadResRef<CMesh>(pMesh, pFile);
			assert(pMesh.Get());
			MeshData->m_pMesh = pMesh;


			// material ���� �б�
			vector<Ptr<CMaterial>> vecMtrl;

			// ���� ����
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
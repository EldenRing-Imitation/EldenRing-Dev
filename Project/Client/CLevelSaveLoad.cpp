#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine\CPathMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>

#include <Script\CScriptMgr.h>
#include <Engine/CSQLMgr.h>

int CLevelSaveLoad::m_LevelID = 0;

int CLevelSaveLoad::SaveLevel(const wstring& _LevelPath, CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP)
		return E_FAIL;

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)	
		return E_FAIL;

	// ���� �̸� ����
	SaveWString(_Level->GetName(), pFile);


	// ������ ���̾���� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// ���̾� �̸� ����
		SaveWString(pLayer->GetName(), pFile);

		// ���̾��� ���ӿ�����Ʈ�� ����
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// ������Ʈ ���� ����
		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);
		
		// �� ���ӿ�����Ʈ
		for (size_t i = 0; i < objCount; ++i)
		{
			SaveGameObject(vecParent[i], pFile);
		}
	}

	fclose(pFile);


	return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// �̸�
	SaveWString(_Object->GetName(), _File);
	
	// ������Ʈ
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{		
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// ������Ʈ Ÿ�� ����
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ Ÿ�� ����
		fwrite(&i, sizeof(UINT), 1, _File);

		// ������Ʈ ���� ����
		Com->SaveToLevelFile(_File);
	}

	// ��ũ��Ʈ	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// �ڽ� ������Ʈ
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);		
	}

	return 0;
}

int CLevelSaveLoad::SaveLevelToDB(CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP)
		return E_FAIL;

	//���� ������ ����
	CSQLMgr::GetInst()->DeleteAllRecordToAllTable();

	//���� ����
	int levelId = CSQLMgr::GetInst()->InsertToLevel(_Level->GetName());

	m_LevelID = levelId;

	// ������ ���̾� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// ���̾� ����
		int layerId = CSQLMgr::GetInst()->InsertToLayer(levelId, pLayer->GetName(), i);

		// ���̾��� ���ӿ�����Ʈ�� ����
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// �� ���ӿ�����Ʈ
		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			SaveGameObjectToDB(layerId, vecParent[i], -1);
		}
	}

	return 0;
}

int CLevelSaveLoad::SaveGameObjectToDB(int _layerID, CGameObject* _Object, int _parentID)
{
	//���ӿ�����Ʈ ����
	int gameObjectId = CSQLMgr::GetInst()->InsertToGameObject(_layerID, _Object->GetName(), _parentID);

	vector<int> comType;
	// ������Ʈ
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ ���� ����
		Com->SaveToDB(gameObjectId, (COMPONENT_TYPE)i);

		comType.push_back(i);
	}
	wstring wComType = IntArrayToWString(comType);
	CSQLMgr::GetInst()->UpdateToGameObject(gameObjectId, wComType);

	// ��ũ��Ʈ	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		//��ũ��Ʈ �̸� �������ֱ�.

		vecScript[i]->SaveToDB(gameObjectId,(COMPONENT_TYPE)i);
	}


	// �ڽ� ������Ʈ
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObjectToDB(-1, vecChild[i], gameObjectId);
	}
	return 0;
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _LevelPath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;

	CLevel* NewLevel = new CLevel;

	// ���� �̸�
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// ���̾� �̸�
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// ���� ������Ʈ ����
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// �� ���ӿ�����Ʈ
		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// �̸�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// ������Ʈ
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// ������Ʈ ������ ���� Ȯ��
		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			Component = nullptr;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			Component = new CDecal;
			break;
		}

		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}


	// ��ũ��Ʈ	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// �ڽ� ������Ʈ		
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}

CLevel* CLevelSaveLoad::LoadLevelByDB()
{
	CLevel* NewLevel = new CLevel;

	CSQLMgr::GetInst()->SelectFromLevel(m_LevelID, NewLevel);

	CSQLMgr::GetInst()->SelectFromLayer(m_LevelID, NewLevel);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObjectByDB(int _layerID, int _parentID)
{
	return nullptr;
}

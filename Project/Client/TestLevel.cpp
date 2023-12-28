#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CCameraMoveScript.h>

#include "CLevelSaveLoad.h"


#include <Engine/CSetColorShader.h>


void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"SkyBox");
	pCurLevel->GetLayer(2)->SetName(L"Planet");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object ����
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera �� ����
	pMainCam->Camera()->SetLayerMaskAll(true);	// ��� ���̾� üũ
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer �� ���������� �ʴ´�.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI cameara
	// CGameObject* pUICam = new CGameObject;
	// pUICam->SetName(L"UICamera");
	// 
	// pUICam->AddComponent(new CTransform);
	// pUICam->AddComponent(new CCamera);
	// 
	// pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	// pUICam->Camera()->SetCameraIndex(1);		// Sub ī�޶�� ����
	// pUICam->Camera()->SetLayerMask(31, true);	// 31�� ���̾ üũ
	// 
	// SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);
	 
	 
	// SkyBox �߰�
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");
	
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100));
	pSkyBox->SkyBox()->SetSkyBoxType(SKYBOX_TYPE::SPHERE);
	pSkyBox->SkyBox()->SetSkyBoxTexture(CResMgr::GetInst()->FindRes<CTexture>(L"stars_milky_way.jpg"));
	
	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), L"SkyBox");
	 
	// ���� �߰�
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"SunLight");
	
	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);
	
	pLightObj->Transform()->SetRelativeRot(0.f, 0.f, 0.f);
	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetRadius(500.f);
	pLightObj->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));	
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));
	
	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);
	
	// �༺
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Earth");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 500.f));
	pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EarthMtrl.mtrl"));
	
	SpawnGameObject(pObject, Vec3(0.f, 0.f, 500.f), L"Planet");

	CGameObject* pCloud = new CGameObject;
	pCloud->SetName(L"EarthCloud");
	pCloud->AddComponent(new CTransform);
	pCloud->AddComponent(new CMeshRender);
	pCloud->Transform()->SetRelativeScale(Vec3(1.05f, 1.05f, 1.05f));
	pCloud->Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
	
	pCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EarthCloudMtrl.mtrl"));
	
	pObject->AddChild(pCloud);
	
}

#include "pch.h"
#include "CameraUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCamera.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

CameraUI::CameraUI()
    : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
    , m_bLayerCheckWindow(false)
{
    SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    CCamera* pCamComponent = GetTarget()->Camera();

    float       fScale = pCamComponent->GetScale();             // Orthograpic ���� ����ϴ� ī�޶� ����

    float       fNear = pCamComponent->GetNear();               // Near ��
    float       fFar = pCamComponent->GetFar();                 // Far ��

    float       fFOV = pCamComponent->GetFOV();                 // ȭ��
    bool        bMainCamera = pCamComponent->IsMainCamera();    // ����ī�޶� ����

    float       fOrthoWidth = pCamComponent->GetOrthoWidth();   // OrthoGraphic ������ ���� ���� ����
    float       fOrthoHeight = pCamComponent->GetOrthoHeight(); // OrthoGraphic ������ ���� ���� ����

    PROJ_TYPE   eProjType = pCamComponent->GetProjType();       // ���� Ÿ��

    int         iCamIdx= pCamComponent->GetCamIdx();            // ī�޶� �ε���

    bool        bDebugView = pCamComponent->IsDebugView();         // ī�޶� �����ť��

    
    ImGui::Text("Near       ");
    ImGui::SameLine();
    ImGui::SliderFloat("##CameraNear", &fNear, 1.f, fFar - 10.f);
    
    ImGui::Text("Far        ");
    ImGui::SameLine();
    ImGui::SliderFloat("##CameraFar", &fFar, fNear + 10.f, 10000.f);

    ImGui::Text("FOV        ");
    ImGui::SameLine();
    ImGui::SliderAngle("##CameraFOV", &fFOV, 1, 179);

    ImGui::Text("LayerCheck ");
    ImGui::SameLine();
    if (ImGui::Button("OpenWindow##LayerCheckWindow"))
        m_bLayerCheckWindow = true;

    if (m_bLayerCheckWindow)
        render_LayerCheck();

    fScale = pCamComponent->GetScale();             // Orthograpic ���� ����ϴ� ī�޶� ����

    pCamComponent->SetNear(fNear);               // Near ��
    pCamComponent->SetFar(fFar);                 // Far ��

    pCamComponent->SetFOV(fFOV);                 // ȭ��
    bMainCamera = pCamComponent->IsMainCamera();    // ����ī�޶� ����

    pCamComponent->SetOrthoWidth(fOrthoWidth);   // OrthoGraphic ������ ���� ���� ����
    pCamComponent->SetOrthoHeight(fOrthoHeight); // OrthoGraphic ������ ���� ���� ����

    pCamComponent->SetProjType(eProjType);       // ���� Ÿ��

    pCamComponent->ViewDebugCube(bDebugView);         // ī�޶� �����ť��

    return TRUE;
}

void CameraUI::render_LayerCheck()
{
    CLevel*     CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    UINT        iLayerMask = GetTarget()->Camera()->GetLayerMask();         // �Կ����̾� üũ
    wstring     WLayerName;


    if (ImGui::Begin("Camera_LayerCheckWindow", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
    {
        ImGui::BeginGroup();
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            WLayerName = CurLevel->GetLayer(i)->GetName();

            // ���̾��̸��� �����Ǿ����� ���� ���
            if (L"" == WLayerName)
                continue;

            string LayerNumb = "##Layer " + std::to_string(i);
            string LayerName;
            LayerName.assign(WLayerName.begin(), WLayerName.end());

            bool bChecked = iLayerMask & (1 << i);
            
            ImGui::Button(LayerName.c_str());
            ImGui::SameLine();
            ImGui::Checkbox(LayerNumb.c_str(), &bChecked);

            if(bChecked)
                iLayerMask |= 1 << i;
            else
                iLayerMask &= ~(1 << i);
        }
        ImGui::EndGroup();

        if (ImGui::Button("Close##LayerCheckWinCloseBtn"))
        {
            m_bLayerCheckWindow = false;
            ImGui::CloseCurrentPopup();
        };

        GetTarget()->Camera()->SetLayerMask(iLayerMask);
        
        ImGui::End();
    }
}
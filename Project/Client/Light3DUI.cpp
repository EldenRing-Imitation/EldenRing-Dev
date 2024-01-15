#include "pch.h"
#include "Light3DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CLight3D.h>
#include <Engine\CCamera.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

Light3DUI::Light3DUI()
    : ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
    , vMemLightAmbient{}
    , fMemRadius(0.f)
    , fMemAngle(0.f)
    , bDebug(false)
{
    SetName("Light3D");
}

Light3DUI::~Light3DUI()
{
}

int Light3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;


    LIGHT_TYPE eLightType = GetTarget()->Light3D()->GetLightType();
    Vec3 vLightColor = GetTarget()->Light3D()->GetLightColor();
    Vec3 vLightAmbient = GetTarget()->Light3D()->GetLightAmbient();
    float fRadius = GetTarget()->Light3D()->GetRadius();
    float fAngle = GetTarget()->Light3D()->GetAngle();
    bool bDebug = GetTarget()->Light3D()->IsDebug();

    const char* lighttype[] = { "DIRECTIONAL", "POINT", "SPOT" };
    int CurrentType = (UINT)eLightType;

    CCamera* pCamComponent = GetTarget()->Light3D()->GetLightRenderCam();

    float       fFar = pCamComponent->GetFar();                 // Far ��
    float       fOrthoWidth = pCamComponent->GetOrthoWidth();   // OrthoGraphic ������ ���� ���� ����
    float       fOrthoHeight = pCamComponent->GetOrthoHeight(); // OrthoGraphic ������ ���� ���� ����

    static bool bLightRenderLayerCheckWin = false;
   

    ImGui::Text("LightType   ");
    ImGui::SameLine();
    ImGui::Combo("##LightType", &CurrentType, lighttype, IM_ARRAYSIZE(lighttype));

    ImGui::Text("LightColor  ");
    ImGui::SameLine();
    ImGui::DragFloat3("##LightColor", vLightColor, 0.001f, 0.f, 1.f);

    if (CurrentType == (UINT)LIGHT_TYPE::DIRECTIONAL)
    {
        ImGui::Text("LightAmbient");
        ImGui::SameLine();
        ImGui::DragFloat3("##LightAmbient", vLightAmbient, 0.001f, 0.f, 1.f);
    }
    
    if (CurrentType == (UINT)LIGHT_TYPE::POINT)
    {
        ImGui::Text("Radius      ");
        ImGui::SameLine();
        ImGui::DragFloat("##Radius", &fRadius, 1.f, 0.f, 10000.f);
    }

    if (CurrentType == (UINT)LIGHT_TYPE::SPOT)
    {
        ImGui::Text("Angle       ");
        ImGui::SameLine();
        ImGui::DragFloat("##Angle", &fAngle, 1.f, 0.f, 359.f);
    }

    if (CurrentType == (UINT)LIGHT_TYPE::POINT
        || CurrentType == (UINT)LIGHT_TYPE::SPOT)
    {
        ImGui::Text("ShowDebug   ");
        ImGui::Checkbox("##LightDebug", &bDebug);
    }
    else if (CurrentType == (UINT)LIGHT_TYPE::DIRECTIONAL)
    {
        ImGui::Text("LightRender Camera Setting");
        
        ImGui::Text("Far          ");
        ImGui::SameLine();
        ImGui::SliderFloat("##LightRenderCamera_Far", &fFar, 10.f, 100000.f);

        ImGui::Text("OrthoWidth   ");
        ImGui::SameLine();
        ImGui::SliderFloat("##LightRenderCamera_OrthoWidth", &fOrthoWidth, 10.f, 10000.f);

        ImGui::Text("OrthoHeight  ");
        ImGui::SameLine();
        ImGui::SliderFloat("##LightRenderCamera_OrthoHeight", &fOrthoHeight, 10.f, 10000.f);

        if (ImGui::Button("Layer Check##LightRenderLayerCheckWinOpen"))
            bLightRenderLayerCheckWin = true;
            
        render_LightRenderLayer(&bLightRenderLayerCheckWin, pCamComponent);

    }

    GetTarget()->Light3D()->SetLightType((LIGHT_TYPE)CurrentType);
    GetTarget()->Light3D()->SetLightColor(vLightColor);
    GetTarget()->Light3D()->SetLightAmbient(vLightAmbient);
    GetTarget()->Light3D()->SetRadius(fRadius);
    GetTarget()->Light3D()->SetAngle(fAngle);
    GetTarget()->Light3D()->SetDebug(bDebug);

    pCamComponent->SetOrthoWidth(fOrthoWidth);      // OrthoGraphic ������ ���� ���� ����
    pCamComponent->SetOrthoHeight(fOrthoHeight);    // OrthoGraphic ������ ���� ���� ����
    pCamComponent->SetFar(fFar);                    // Far ��

    return TRUE;
}

void Light3DUI::render_LightRenderLayer(bool* _bLightRenderLayerCheckWin, CCamera* _LightRenderCam)
{
    if ((*_bLightRenderLayerCheckWin)  == false)
        return;

    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
    UINT        iLayerMask = _LightRenderCam->GetLayerMask();         // �Կ����̾� üũ
    wstring     WLayerName;

    if (ImGui::Begin("Cam_LayerCheck", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) 
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

            if (bChecked)
                iLayerMask |= 1 << i;
            else
                iLayerMask &= ~(1 << i);
        }
        ImGui::EndGroup();

        if (ImGui::Button("Close##LightRenderLayerCheckWinCloseBtn"))
        {
            (*_bLightRenderLayerCheckWin) = false;
            ImGui::CloseCurrentPopup();
        };

        _LightRenderCam->SetLayerMask(iLayerMask);

        ImGui::End();
    }
}

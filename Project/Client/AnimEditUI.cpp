#include "pch.h"
#include "AnimEditUI.h"

#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CResMgr.h>
#include <Engine/CDevice.h>


#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>

#include <Engine/CTexture.h>
#include <Engine/CStructuredBuffer.h>
#include <Script/CCameraMoveScript.h>

#include "CEditorObjMgr.h"
#include "CAnimEditObj.h"


AnimEditUI::AnimEditUI()
    : UI("##AnimEditUI")
    , m_pRenderObj(nullptr)
{
    SetName("AnimEditUI");


    // 에디터 윈도우 세팅
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        SetFlags(window_flags);
        SetModal(false);
    }
}

AnimEditUI::~AnimEditUI()
{
}

void AnimEditUI::render_cliplistwindow()
{
    ImGui::Button("AnimClipList", ImVec2(200, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    ImVec2 ItemSize = ImGui::GetItemRectSize();
    
    int ClipCount = 200;
    static int SelectClipIdx = 0;

    ImGui::BeginChild("##AnimClipList", ImVec2(200.f, 600.f - ItemSize.y), false, window_flags);;

    // 클립이 한개도 없는 경우 ( No clip )

    // 클립이 1개 이상인 경우 리스트 출력
    for (int i = 0; i < ClipCount; i++)
    {
        if (ImGui::MenuItem("ItemTest"))
        {
            SelectClipIdx = i;
        }
    }
    ImGui::EndChild();

    char buf[10];
    sprintf_s(buf, "%d / %d", SelectClipIdx, ClipCount);

    ImGui::Text(buf);

    ImGui::SameLine();
    ImGui::Button("+");
    ImGui::SameLine();
    ImGui::Button("-");
}

void AnimEditUI::render_previewwindow()
{
    ImGui::Button("Preview Window", ImVec2(600, 0));

    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"3DAnimEditTex");

    int width = (int)pTex->Width();
    int height = (int)pTex->Height();


    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    ImGui::Image((ImTextureID)pTex->GetSRV().Get(), ImVec2(600, 600), uv_min, uv_max, tint_col, border_col);
}

void AnimEditUI::render_infowindow()
{
    // 전체 애니메이션 정보
    // ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    // ImGui::BeginChild("##MeshInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::BeginGroup();
    ImGui::Button("MeshInfo", ImVec2(0, 0));
    ImGui::Text("frame value1");
    ImGui::Text("frame value2");
    ImGui::Text("frame value3");
    ImGui::Text("frame value4");
    ImGui::Text("frame value5");
    ImGui::EndGroup();
    //ImGui::EndChild();

    // 현재 애니메이션 정보
    //ImGui::BeginChild("##CurAnimInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::Button("CurAnimInfo", ImVec2(0, 0));
    ImGui::BeginGroup();
    ImGui::Text("frame value1");
    ImGui::Text("frame value2");
    ImGui::Text("frame value3");
    ImGui::Text("frame value4");
    ImGui::Text("frame value5");
    ImGui::EndGroup();
    //ImGui::EndChild();

    if (ImGui::Button("SetMesh"))
    {
        if (nullptr == m_pRenderObj)
        {
            Ptr<CMeshData> pMeshData = nullptr;
            pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");

            m_pRenderObj = new CAnimEditObj;
            m_pRenderObj->setobject(pMeshData);

            CEditorObjMgr::GetInst()->SetTexRender(m_pRenderObj);
            m_pRenderObj->GetAnimator3D()->GetCurAnimClip(m_tMTCurAnimClip);
        }
    }
}

void AnimEditUI::render_TimeLine()
{
    ImGuiStyle& style = ImGui::GetStyle();
    float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
    ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("##AnimEditTimeLine", ImVec2(0, child_height), true, child_flags);
    
    int MaxFrame = 0; 
    int iCurFrame = 0; 
    
    if (m_pRenderObj)
    {
        MaxFrame = m_tMTCurAnimClip.iFrameLength;
        iCurFrame = m_pRenderObj->GetAnimator3D()->GetCurFrameIdx();
        
    }

    static float test = 1.f;
    for (int frame = 0; frame < MaxFrame; frame++)
    {
        if (frame > 0)
            ImGui::SameLine();
        
        if (frame == iCurFrame)
        {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), " ");
            //   //  ImGui::SetScrollHereX(0.5f);    // half position
            ImGui::SetScrollHereX((float)(iCurFrame / MaxFrame));
        }
        else
        {
            ImGui::Text("               ");
        }
    }
    float SliderWidth = ImGui::GetWindowWidth();
    ImGui::EndChild();
    
    ImGui::SetNextItemWidth(SliderWidth);
    ImGui::SliderInt("Lines", &iCurFrame, 0, MaxFrame);
}

void AnimEditUI::render_CamController()
{
    ImGui::Button("Cam Contorller", ImVec2(0, 0));
}

void AnimEditUI::tick()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    SetPopupPos(viewport->WorkPos);
    SetSize(viewport->WorkSize.x, viewport->WorkSize.y);
}

void AnimEditUI::finaltick()
{
    // 반투명 배경
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar();

    UI::finaltick();
}

int AnimEditUI::render_update()
{
    ImGui::BeginGroup();
    {
        // Mesh가 보유한 Clip Tree
        {
            ImGui::BeginGroup();

            render_cliplistwindow();

            ImGui::EndGroup();
        }
    }

    ImGui::SameLine();
    // Render Preview Window
    {
        ImGui::BeginGroup();
        {
            render_previewwindow();
        }
        ImGui::EndGroup();
    }

    ImGui::SameLine();
    // Anim Info Window
    {
        ImGui::BeginGroup();
        {
            render_infowindow();

            // Preview Cam Postion Controller
            render_CamController();
        }
        ImGui::EndGroup();
    }

    ImGui::EndGroup();

    // 타임라인 스크롤
    render_TimeLine();
    

    return 0;
}
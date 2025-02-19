#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"

#include "CStructuredBuffer.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CTexture.h"

void CRenderMgr::init()
{
    // Light2DBuffer 구조화 버퍼 생성
    m_Light2DBuffer = new CStructuredBuffer;
    m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // Light3DBuffer 구조화 버퍼 생성
    m_Light3DBuffer = new CStructuredBuffer;
    m_Light3DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // MRT 생성
    CreateMRT();
}

void CRenderMgr::CreateMRT()
{
    // ====================
    // SwapChain MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrRTTex, 1, pDSTex);
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(Vec4(0.3f, 0.3f, 0.3f, 1.f), 0);
    }

    // ====================
    // Deferred MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"ColorTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R8G8B8A8_UNORM
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"NormalTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[2] = CResMgr::GetInst()->CreateTexture(L"PositionTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[3] = CResMgr::GetInst()->CreateTexture(L"EmissiveTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R8G8B8A8_UNORM
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[4] = CResMgr::GetInst()->CreateTexture(L"DataTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R32G32B32A32_FLOAT
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrRTTex, 5, nullptr);
    }

    // ====================
    // Deferred Decal 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex");
        arrRTTex[1] = CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex");

        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->Create(arrRTTex, 2, nullptr);
    }



    // ====================
    // Light MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                , DXGI_FORMAT_R8G8B8A8_UNORM
                                , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex", (UINT)vResol.x, (UINT)vResol.y
                                , DXGI_FORMAT_R8G8B8A8_UNORM
                                , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[2] = CResMgr::GetInst()->CreateTexture(L"ShadowTargetTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_R32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::LIGHT]->Create(arrRTTex, 3, nullptr);
    }

    // ====================
    // ShadowMap MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::SHADOWMAP] = new CMRT;

        Vec2 vResol = Vec2(8192, 8192);

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"DynamicShadowMapTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_R32_FLOAT
            , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"DynamicShadowMapDepthTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_D32_FLOAT
            , D3D11_BIND_DEPTH_STENCIL);

        m_MRT[(UINT)MRT_TYPE::SHADOWMAP]->Create(arrRTTex, 1, pDSTex);
    }

    // ====================
    // 3DAnim_EDIT
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::_3DANIM_EDIT] = new CMRT;

        Vec2 vResol(800.f, 800.f);

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"3DAnimEditTex", (UINT)vResol.x, (UINT)vResol.y
                                        , DXGI_FORMAT_R8G8B8A8_UNORM
                                        , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"3DAnimEditDepthTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_D32_FLOAT
            , D3D11_BIND_DEPTH_STENCIL);
        
        m_MRT[(UINT)MRT_TYPE::_3DANIM_EDIT]->Create(arrRTTex, 1, pDSTex);
        m_MRT[(UINT)MRT_TYPE::_3DANIM_EDIT]->SetClearColor(Vec4(0.3f, 0.3f, 0.3f, 1.f), 0);
    }

    // ====================
    // UI_EDIT
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::_UI_EDIT] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"UIEditorTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"UIEditorDepthTex", (UINT)vResol.x, (UINT)vResol.y
            , DXGI_FORMAT_D32_FLOAT
            , D3D11_BIND_DEPTH_STENCIL);

        m_MRT[(UINT)MRT_TYPE::_UI_EDIT]->Create(arrRTTex, 1, pDSTex);
        m_MRT[(UINT)MRT_TYPE::_UI_EDIT]->SetClearColor(Vec4(0.1f, 0.1f, 0.1f, 1.f), 0);
    }
}

void CRenderMgr::ClearMRT()
{
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
    {
        if (nullptr != m_MRT[i])
        {
            m_MRT[i]->ClearTarget();
        }
    }
}
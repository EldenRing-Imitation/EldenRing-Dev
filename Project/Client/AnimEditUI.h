#pragma once
#include "UI.h"

#include <Engine/ptr.h>

class CMesh;
class CAnimEditObj;

class AnimEditUI :
    public UI
{
private:
    Ptr<CMesh> m_pSelectedMesh;  // ���� �ҷ��� mesh
    CAnimEditObj* m_pRenderObj;  // Ŭ�� ����Ʈ
    tMTAnimClip m_tMTCurAnimClip;

    int m_iFrameCount;  // ������ �ʴ� ��� ������

public:
    void MeshLoad() {};

private:
    void render_cliplistwindow();
    void render_previewwindow();
    void render_infowindow();
    void render_TimeLine();
    void render_CamController();

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    AnimEditUI();
    virtual ~AnimEditUI();
};


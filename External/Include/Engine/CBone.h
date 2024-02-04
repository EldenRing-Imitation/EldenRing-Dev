#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

class CBone :
    public CRes
{
private:
    vector<tMTAnimClip> m_vecAnimClip;
    vector<tMTBone>     m_vecBones;
    CStructuredBuffer*  m_pBoneFrameData;    // ��ü �� ������ ���� ( ũ��, �̵�, ȸ�� ) (������ ������ŭ)
    CStructuredBuffer*  m_pBoneOffset;       // �� ���� offset ��� ( �� ���� ��ġ�� �ǵ����� ���) (1��¥��)

    UINT    m_iFrmCnt;

public:
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    UINT GetFrameCount() { return m_iFrmCnt; }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }    // ��ü �� ������ ����
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }  // �� ���� offset ���

public:
    static CBone* CreateFromContatiner(CFBXLoader& _loader);
    static CBone* LoadFbx(const wstring& _strPath);

private:
    virtual int Load(const wstring& _strFilePath);

public:
    virtual int Save(const wstring& _strRelativePath);

public:
    CBone(bool _bEngine = false);
    ~CBone();
};


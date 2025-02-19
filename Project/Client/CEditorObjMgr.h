#pragma once

class CGameObjectEx;
class CAnimEditObj;

class CEditorObjMgr
	: public CSingleton<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);
private:
	vector<CGameObjectEx*>	m_vecEditorObj;

	bool					m_bRenderTex;

	CAnimEditObj*			m_pTexRenderObj;

	CGameObjectEx*			m_DebugShape[(UINT)SHAPE_TYPE::END];
	vector<tDebugShapeInfo> m_DebugShapeInfo;

	bool					m_bDebugRender;

public:
	void init();
	void progress();

private:
	void tick();
	void render();

	void DebugRender();
	void EditorRender();


public:
	void SetTexRender(CAnimEditObj* _pObj);
};


#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CRaycastShader
	: public CComputeShader
{
private:
	UINT                m_iXFace;
	UINT                m_iZFace;

	// ī�޶� Ray ����
	tRay                m_ray;

	//�˻��� �ﰢ�� ������ ��ġ��
	vector<Vector3>			m_vVtx;

	//�ﰢ���� ��ġ�� ������ ���� ����ȭ����
	CStructuredBuffer*		m_pVtx;

	// ������ġ ��� ����
	CStructuredBuffer* m_pOutput;

public:
	void SetVtx(vector<Vector3> _vtx) { m_vVtx = _vtx; }
	void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
	void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
	void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }

protected:

	virtual void UpdateData();
	virtual void Clear();

public:
	CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	~CRaycastShader();
};


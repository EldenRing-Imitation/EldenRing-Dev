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
	vector<Vector4>			m_vVtx;

	//�ﰢ���� ��ġ�� ������ ���� ����ȭ����
	CStructuredBuffer* m_pVtx;

	// ������ġ ��� ����
	CStructuredBuffer* m_pOutput;

	Vector3 m_vMaxScale;

public:
	void SetVtx(vector<Vector4> _vtx);
	void SetFaceCount(UINT _x, UINT _z) { m_iXFace = _x; m_iZFace = _z; }
	void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
	void SetOuputBuffer(CStructuredBuffer* _pOutputBuffer) { m_pOutput = _pOutputBuffer; }

	void SetMaxScale(Vec3 _scale) { m_vMaxScale = _scale; }

	tRay GetRayInfo() { return m_ray; }
	vector<Vector4> GetVertex() { return m_vVtx; }

protected:

	virtual void UpdateData();
	virtual void Clear();

public:
	CRaycastShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	~CRaycastShader();
};


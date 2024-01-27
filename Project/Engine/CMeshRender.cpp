#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"

#include "CResMgr.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (UINT i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;

			GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}

	// ����
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (UINT i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetMaterial(i))
		{
			// ����� ���� ������Ʈ
			GetMaterial(i)->UpdateData();

			// ����� �޽� ������Ʈ �� ������
			GetMesh()->render(i);
		}
	}

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();

	if (GetOwner()->GetName() == L"Monster")
		int a = 0;
}

void CMeshRender::render(UINT _iSubset)
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(_iSubset))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		GetMaterial(_iSubset)->SetAnim3D(true); // Animation Mesh �˸���
		GetMaterial(_iSubset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	// ����� ���� ������Ʈ
	GetMaterial(_iSubset)->UpdateData();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(_iSubset);

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();

}

void CMeshRender::render_shadowmap()
{
	//CRenderComponent::render_shadowmap();

	Ptr<CMaterial> pShadowMapMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	Transform()->UpdateData();

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		pShadowMapMtrl->SetAnim3D(true); // Animation Mesh �˸���
		pShadowMapMtrl->SetBoneCount(Animator3D()->GetBoneCount());
		int a = 0;
	}

	pShadowMapMtrl->UpdateData();

	GetMesh()->render(0);

	if (Animator3D())
		Animator3D()->ClearData();

	if (GetOwner()->GetName() == L"Monster")
		int a = 0;
}

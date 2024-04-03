#pragma once
#include <Engine/CScript.h>
#include "ER_define.h"
#include "ER_struct.h"

class ER_DataScript_Item :
    public CScript
{
private:
	wstring			m_strItemName;	// ������ �̸�
	Ptr<CTexture>	m_ItemIcon;		// ������ �ؽ���

	UINT			m_eItemCode;	// ������ ��ȣ
	UINT			m_eGrade;		// ������ ���		0 : �Ϲ�, 1 : ���, 2 : ���, 3 : ����
	UINT			m_eType;		// ������ Ÿ��		
	UINT			m_eSlot;		// ������ ��������	0 : ����, 1 : ��, 2 : �Ӹ�, 3 : ��, 4 : �ٸ�, 5 : �����Ұ�
	UINT			m_eWeapon_type;	// ���� Ÿ��			0 : ������, 1 : ����, 2 : ��հ�, 3 : ����, 4 : �۷���, 5 : Ȱ

	ER_RECIPE		m_uniRecipe;	// ��� ������

	ER_ItemStats	m_tItemStats;	// ������ ����

	// ĳ���� ���� ���� Ȯ��
	bool			m_bEquiped;	// 0 : ����, 1 : ���

private:
	void SetItemName(const wchar_t* _strName) { m_strItemName = _strName; }
	void SetItemName(wstring _strName) { m_strItemName = _strName; }
	void SetItemTex(Ptr<CTexture> _pTex) { m_ItemIcon = _pTex; }
	void SetItemInfo(UINT _code, UINT _grade, UINT _type, UINT _slot)
	{
		m_eItemCode = _code;
		m_eGrade = _grade;
		m_eType = _type;
		m_eSlot = _slot;
	}
	void SetItemCode(UINT _code) { m_eItemCode = _code; }
	void SetItemGrade(UINT _grade) { m_eGrade = _grade; }
	void SetItemType(UINT _type) { m_eType = _type; }
	void SetItemSlot(UINT _slot) { m_eSlot = _slot; }
	void SetWPType(UINT _WPType) { m_eWeapon_type = _WPType; }
	void SetItemIngr_1(UINT _ingr_1) { m_uniRecipe.ingredient_1 = _ingr_1; }
	void SetItemIngr_2(UINT _ingr_2) { m_uniRecipe.ingredient_2 = _ingr_2; }
	void SetItemRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }

	void GetRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }
	void SetItemStats(ER_ItemStats& _stats) { m_tItemStats = _stats; }

public:
	const wstring& GetItemName() { return m_strItemName; }
	Ptr<CTexture> GetItemTex() { return m_ItemIcon; }

	UINT GetCode() { return (UINT)m_eItemCode; }
	UINT GetGrade() { return (UINT)m_eGrade; }
	UINT GetType() { return (UINT)m_eType; }
	UINT GetSlot() { return (UINT)m_eSlot; }
	UINT GetWPType() { return (UINT)m_eWeapon_type; }

	ER_RECIPE GetRecipe() { return m_uniRecipe; }

	const ER_ItemStats& GetStats() { return m_tItemStats; }

	bool IsEquiped() { return m_bEquiped; }

	CLONE(ER_DataScript_Item);
public:
	ER_DataScript_Item();
	~ER_DataScript_Item();

	friend class ER_ItemMgr;
	friend class ItemDataUI;
	friend class ER_DataScript_Character;
};


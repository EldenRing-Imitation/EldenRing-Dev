#pragma once
#include <Engine\ptr.h>
#include <Engine\CTexture.h>

struct ER_tStats
{
	short iAttackPower;					// ���ݷ�
	short iAttackPowerPerLevel;			// ������ ���ݷ�
	short iSkillAmplification;			// ��ų ����
	short iSkillAmplificationPerLevel;	// ������ ��ų ����
	short iMaxHP;						// ü��
	short iMaxHPPerLevel;				// ������ ü��
	float fHPRegen;						// ü�� ���
	float fHPRegenPerLevel;				// ������ ü�� ���
	short iMaxSP;						// ���׹̳�
	short iMaxSPPerLevel;				// ������ ���׹̳�
	float fSPRegen;						// ���׹̳� ���
	float fSPRegenPerLevel;				// ������ ���׹̳� ���
	short iDefense;						// ����
	float fAttackSpeed;					// ���� �ӵ� %
	float fCriticalStrikeChance;		// ġ��Ÿ Ȯ��
	float fCriticalStrikeDamage;		// ġ��Ÿ �߰� ������
	float fMovementSpeed;				// �̵� �ӵ�
	float fVisionRange;					// �þ�
	float fCooldownReduction;			// ��Ÿ�� ����
	float fOmnisyphon;					// ��� ���� ����
	short padding;						// 2byte �е�
};

class ER_Item
{
private:
	wstring			m_strItemName;	// ������ �̸�
	Ptr<CTexture>	m_ItemIcon;		// ������ �ؽ���
	
	UINT			m_eItemCode;	// ������ ��ȣ
	UINT			m_eGrade;		// ������ ���
	UINT			m_eType;		// ������ Ÿ��
	UINT			m_eSlot;		// ������ ��������

	ER_RECIPE		m_uniRecipe;	// ��� ������
	
	ER_tStats		m_tItemStats;	// ������ ����

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

	void GetRecipe(ER_RECIPE _recipe) { m_uniRecipe = _recipe; }
	void SetItemStats(ER_tStats& _stats) { m_tItemStats = _stats; }

public:
	const wstring& GetItemName() { return m_strItemName; }
	Ptr<CTexture> GetItemTex() { return m_ItemIcon; }

	UINT GetCode() { return (UINT)m_eItemCode; }
	UINT GetGrade() { return (UINT)m_eGrade; }
	UINT GetType() { return (UINT)m_eType; }
	UINT GetSlot() { return (UINT)m_eSlot; }

	ER_RECIPE GetRecipe() { return m_uniRecipe; }

	const ER_tStats& GetStats() { return m_tItemStats; }

public:
	ER_Item();
	~ER_Item();

	friend class ER_ItemMgr;
	friend class ItemDataUI;
};
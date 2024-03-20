#pragma once

struct tInitial_Character_Stats
{
	// �⺻ �ɷ�ġ
	short iAttackPower;					// ���ݷ�
	short iAttackPowerPerLevel;			// ������ ���ݷ�
	short iDefense;						// ����
	short iDefensePerLevel;				// ������ ����
	short iMaxHP;						// ü��
	short iMaxHPPerLevel;				// ������ ü��
	float fHPRegen;						// ü�� ���
	float fHPRegenPerLevel;				// ������ ü�� ���
	short iMaxSP;						// ���׹̳�
	short iMaxSPPerLevel;				// ������ ���׹̳�
	float fSPRegen;						// ���׹̳� ���
	float fSPRegenPerLevel;				// ������ ���׹̳� ���
	float fAttackSpeed;					// ���� �ӵ� %
	float fCriticalStrikeChance;		// ġ��Ÿ Ȯ��
	float fMovementSpeed;				// �̵� �ӵ�
	float fVisionRange;					// �þ�

	UINT	eWPType;					// �ֹ��� Ÿ��
	float	fWpAtkSpd;					// �ֹ��� ���� �ӵ�
	float	fWpAtkRange;				// �ֹ��� ��Ÿ�
}typedef ER_Initial_Stats;

struct tIngame_Stats
{
	int		iLevel;					// ����
	int		iExp;					// ����ġ
	int		iNeedExpForNextLevel;	// �������� ����ġ

	int		iAttackPower;			// ���ݷ�
	int		iDefense;				// ����
	int		iMaxHP;					// �ִ� ü��
	int		iHP;					// ���� ü��
	float	fHPRegen;				// ü�� ���
	int		iMaxSP;					// �ִ� ���¹̳�
	int		iSP;					// ���� ���¹̳�
	float	fSPRegen;				// ���¹̳� ���
	float	fAttackSpeed;			// ���ݼӵ�
	float	fCriticalStrikeChance;	// ġ��Ÿ Ȯ��
	float	fMovementSpeed;			// �̵��ӵ�
	float	fVisionRange;			// �þ�
	float	fAtakRange;				// �⺻���� ��Ÿ�

	float	fCriticalStrikeDamage;
	float	fCooldownReduction;			// ��Ÿ�� ����
	float	fOmnisyphon;					// ��� ���� ����
	int		iSkillAmplification;	// ��ų ����

	void Init_To_LevelOne(const ER_Initial_Stats& _StdStats)
	{
		iLevel = 1;
		iExp = 1;
		iNeedExpForNextLevel = 50;	// ���� ����

		iAttackPower = _StdStats.iAttackPower;
		iDefense = _StdStats.iDefense;
		iMaxHP = _StdStats.iMaxHP;
		iHP = _StdStats.iMaxHP;
		fHPRegen = _StdStats.fHPRegen;
		iMaxSP = _StdStats.iMaxSP;
		iSP = _StdStats.iMaxHP;
		fSPRegen = _StdStats.fSPRegen;
		fAttackSpeed = _StdStats.fAttackSpeed;
		fCriticalStrikeChance = _StdStats.fCriticalStrikeChance;
		fMovementSpeed = _StdStats.fMovementSpeed;
		fVisionRange = _StdStats.fVisionRange;
		fAtakRange = _StdStats.fWpAtkRange;

		fCriticalStrikeDamage = 0;
		fCooldownReduction = 0;
		fOmnisyphon = 0;
		iSkillAmplification = 0;
	};
}typedef ER_Ingame_Stats;

#include <Engine\ptr.h>
#include <Engine\CTexture.h>

struct tItem_Stats
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

}typedef ER_ItemStats;

struct tSkill_Info
{
	wstring			strName;	// ��ų �̸�
	Ptr<CTexture>	TexSkill;	// ��ų ������

	int		iMaxSkillLevel;		// �ִ� ��ų ����

	// �⺻ ��ų ����
	int		iValue1[5];			// Int�� ����
	int		iValue2[5];			// Int�� ����
	float	fValue1[5];			// float�� ����
	float	fValue2[5];			// float�� ����
	
	float	fRange[5];			// ��ų ���� ����
	float	fMaxCoolDown[5];	// ���� ���ð�

	// �ΰ��� ����
	int		iSkillLevel;		// ��ų ����	: value Idx
	float	fCoolDown;			// ���� ���� ���ð�

	tSkill_Info()
		: iMaxSkillLevel(5)	// �ִ� ��ų ����
		, iValue1{}
		, iValue2{}
		, fValue1{}
		, fValue2{}
		, fRange{}
		, fMaxCoolDown{}
		, iSkillLevel(1)
		, fCoolDown(0.f)
	{
		strName = L"NULL";
	}

	void Save(FILE* _File)
	{
		SaveWString(strName, _File);
		fwrite(&iMaxSkillLevel, sizeof(int), 1, _File);
		fwrite(&iValue1, sizeof(int), 5, _File);
		fwrite(&iValue2, sizeof(int), 5, _File);
		fwrite(&fValue1, sizeof(float), 5, _File);
		fwrite(&fValue2, sizeof(float), 5, _File);
		fwrite(&fRange, sizeof(float), 5, _File);
		fwrite(&fMaxCoolDown, sizeof(float), 5, _File);
	}

	void Load(FILE* _File)
	{
		LoadWString(strName, _File);
		fread(&iMaxSkillLevel, sizeof(int), 1, _File);
		fread(&iValue1, sizeof(int), 5, _File);
		fread(&iValue2, sizeof(int), 5, _File);
		fread(&fValue1, sizeof(float), 5, _File);
		fread(&fValue2, sizeof(float), 5, _File);
		fread(&fRange, sizeof(float), 5, _File);
		fread(&fMaxCoolDown, sizeof(float), 5, _File);
	}

}typedef ER_SKILL;

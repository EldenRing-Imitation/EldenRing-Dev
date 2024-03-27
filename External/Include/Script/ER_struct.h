#pragma once
#include "ER_define.h"

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
	float	fAtkRange;				// �⺻���� ��Ÿ�

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
		iSP = _StdStats.iMaxSP;
		fSPRegen = _StdStats.fSPRegen;
		fAttackSpeed = _StdStats.fAttackSpeed;
		fCriticalStrikeChance = _StdStats.fCriticalStrikeChance;
		fMovementSpeed = _StdStats.fMovementSpeed;
		fVisionRange = _StdStats.fVisionRange;
		fAtkRange = _StdStats.fWpAtkRange;

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
	int		iValue1[6];			// Int�� ����
	int		iValue2[6];			// Int�� ����
	float	fValue1[6];			// float�� ����
	float	fValue2[6];			// float�� ����
	
	float	fRange[6];			// ��ų ����
	int		iUseSP[6];			// ��뽺�׹̳�
	float	fMaxCoolDown[6];	// ���� ���ð�
	float	fMaxActionTime[6];	// �ִ� ���ӽð�

	// �ΰ��� ����
	int		iSkillLevel;		// ��ų ����	: value Idx
	int		iCurUseSP;			// ���� ��뿡 �ʿ��� SP
	float	fCoolDown;			// ���� ���� ���ð�
	bool	IsUsable;			// ��� ���� ����;
	bool	IsAction;			// �۵������� ����;
	float	fActionTime;		// ���ӽð�


	tSkill_Info()
		: iMaxSkillLevel(5)	// �ִ� ��ų ����
		, iValue1{}
		, iValue2{}
		, fValue1{}
		, fValue2{}
		, fRange{}
		, iUseSP{}
		, fMaxCoolDown{}
		, fMaxActionTime{}
		, iSkillLevel(0)
		, iCurUseSP(0)
		, fCoolDown(0.f)
		, IsUsable(true)
		, IsAction(false)
		, fActionTime(0.f)
	{
		strName = L"NULL";
	}

	virtual ~tSkill_Info() {}

public:
	bool Use(int* _CharacterSP, bool _IsBuf = false)
	{
		int UsedSP = *_CharacterSP - iUseSP[iSkillLevel];
		
		if (UsedSP < 0)
			return false;

		if (IsUsable)
		{
			if (_IsBuf)
			{
				fActionTime = fMaxActionTime[iSkillLevel];
				IsAction = true;
			}

			fCoolDown = fMaxCoolDown[iSkillLevel];
			IsUsable = false;

			*_CharacterSP = UsedSP;
			return true;
		}

		return false;
	}

	const int& Int1() { return iValue1[iSkillLevel]; }
	const int& Int2() { return iValue2[iSkillLevel]; }
	const float& Float1() { return fValue1[iSkillLevel]; } 
	const float& Float2() { return fValue2[iSkillLevel]; } 
	const float& Range() { return fRange[iSkillLevel]; } 
	const float& MaxCooldown() { return fMaxCoolDown[iSkillLevel]; }
	const float& CurCooldown() { return fCoolDown; }
	const float& ActionTime() { return fMaxActionTime[iSkillLevel]; }
	const int& UseSP() { return iUseSP[iSkillLevel]; }
	void ActionOver() { fActionTime = 0.f; }

	void SkillStatusUpdate(float _Ratio)
	{
		iCurUseSP = iUseSP[iSkillLevel];

		if (IsAction)
		{
			fActionTime -= _Ratio;
			if (fActionTime <= 0)
			{
				fActionTime = 0;
				IsAction = false;
			}
		}

		if (IsUsable)
			return;

		fCoolDown -= _Ratio;

		if (fCoolDown <= 0)
		{
			IsUsable = true;
			fCoolDown = fCoolDown = fMaxCoolDown[iSkillLevel];
		}

	}


	void Save(FILE* _File)
	{
		SaveWString(strName, _File);
		fwrite(&iMaxSkillLevel, sizeof(int), 1, _File);
		fwrite(&iValue1, sizeof(int), 6, _File);
		fwrite(&iValue2, sizeof(int), 6, _File);
		fwrite(&fValue1, sizeof(float), 6, _File);
		fwrite(&fValue2, sizeof(float), 6, _File);
		fwrite(&fRange, sizeof(float), 6, _File);
		fwrite(&iUseSP, sizeof(int), 6, _File);
		fwrite(&fMaxCoolDown, sizeof(float), 6, _File);
		fwrite(&fMaxActionTime, sizeof(float), 6, _File);
	}

	void Load(FILE* _File)
	{
		LoadWString(strName, _File);
		fread(&iMaxSkillLevel, sizeof(int), 1, _File);
		fread(&iValue1, sizeof(int), 6, _File);
		fread(&iValue2, sizeof(int), 6, _File);
		fread(&fValue1, sizeof(float), 6, _File);
		fread(&fValue2, sizeof(float), 6, _File);
		fread(&fRange, sizeof(float), 6, _File);
		fread(&iUseSP, sizeof(int), 6, _File);
		fread(&fMaxCoolDown, sizeof(float), 6, _File);
		fread(&fMaxActionTime, sizeof(float), 6, _File);
	}

}typedef ER_SKILL;

struct tStatus_Effect
{
private:
	UINT Effect;
	float effectStats[10];
	float fActionTime[10];

public:
	const UINT& GetCurStatus() { return Effect; }

	int GetIncATK() { return (int)effectStats[0]; }
	int GetIncDEF() { return (int)effectStats[1]; }
	float GetIncSPD() { return effectStats[2]; }
	float GetIncAPD() { return effectStats[3]; }
	int GetDecATK() { return (int)effectStats[4]; }
	int GetDecDEF() { return (int)effectStats[5]; }
	float GetDecSPD() { return effectStats[6]; }
	float GetDecAPD() { return effectStats[7]; }
	bool IsFear(UINT _bufndbuf) { return _bufndbuf & (UINT)eStatus_Effect::FEAR; }
	bool IsStun(UINT _bufndbuf) { return _bufndbuf & (UINT)eStatus_Effect::STUN; }

	bool ActiveEffect(UINT ActiveEffect, float _ActionTime, float value = 0.f)
	{
		for (int i = 0; i < 10; i++)
		{
			if (ActiveEffect & 1 << i)
			{
				Effect |= ActiveEffect;			// �߰��� ȿ�� ����
				effectStats[i] = value;			// ȿ�� ��ġ ����
				fActionTime[i] = _ActionTime;	// �ۿ�ð� ����
				return true;
			}
		}
		// �ƹ��͵� Ȯ�ε��� �ʾ����� ���� ȿ��
		return false;
	}

	void ActionTiemUpdate(float timeratio)
	{
		// ����� ȿ�� ����
		if (0 == Effect)
			return;

		for (int i = 0; i < 10; ++i)
		{
			if (Effect & 1 << i)
			{
				fActionTime[i] -= timeratio;
				
				// ���ӽð� ����
				if (fActionTime[i] <= 0)
				{
					fActionTime[i] = 0;
					effectStats[i] = 0;
					Effect &= ~(1 << i);
				}
			}
		}
	}

	tStatus_Effect()
		: Effect(0)
		, effectStats{}
		, fActionTime{}
	{}

} typedef BUFNDEBUF_STATS;

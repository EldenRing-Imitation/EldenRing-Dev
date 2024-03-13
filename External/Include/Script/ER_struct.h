#pragma once

struct tStandard_Character_Stats
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
}typedef ER_STD_CHAR_STATS;

struct tIngame_Character_Stats
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

	int		iSkillAmplification;	// ��ų ����

}typedef ER_Ingame_Stats;

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
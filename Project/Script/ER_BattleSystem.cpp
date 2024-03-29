#include "pch.h"
#include "ER_BattleSystem.h"

#include "ER_DataScript_Character.h"
#include "ER_ActionScript_Character.h"

//�׽�Ʈ��
#include "ER_GameSystem.h"

ER_BattleSystem::ER_BattleSystem()
{
}

ER_BattleSystem::~ER_BattleSystem()
{
}


void ER_BattleSystem::init()
{
}

void ER_BattleSystem::Battle_Common(CGameObject* AtkObj, CGameObject* HitObj)
{
	tIngame_Stats* AtkInfo = GetStatsInfo(AtkObj);
	tIngame_Stats* HitInfo = GetStatsInfo(HitObj);

	// ���ݷ� ( ���ݷ� *100 / ( 100 + ������) * ( 1+ ġ��Ÿ���� 0.65)
	
	tStatus_Effect* ATKStatusEfc = AtkObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();
	tStatus_Effect* HitStatusEfc = HitObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();

	int AtkPower = AtkInfo->iAttackPower + ATKStatusEfc->GetIncATK() - ATKStatusEfc->GetDecATK();
	int Def = HitInfo->iDefense + HitStatusEfc->GetIncDEF() - HitStatusEfc->GetDecDEF();

	int CriticalDmg = IsCritical(AtkInfo);

	int FinalDmg = (int)(((AtkPower * 100) / (100 + Def)) + (CriticalDmg * 0.65));

	// ���ó��
	if (HitInfo->iHP - FinalDmg <= 0)
	{
		// Ÿ���� HP�� 0���� �����.
		HitInfo->iHP = 0;

		tFSMData deaddata = {};
		HitObj->GetScript<ER_ActionScript_Character>()->Dead(deaddata);
		return;
	}
	HitInfo->iHP -= FinalDmg;

	// ��������Ʈ ���
}

void ER_BattleSystem::Battle_Skill(CGameObject* _Attacker
	, CGameObject* _HitObj
	, CScript* _Inst
	, SKILL_DMG_CALC _calc)
{
	tIngame_Stats* AtkInfo = GetStatsInfo(_Attacker);
	tIngame_Stats* HitInfo = GetStatsInfo(_HitObj);

	// ���ݷ� ( ���ݷ� *100 / ( 100 + ������) * ( 1+ ġ��Ÿ���� 0.65)

	tStatus_Effect* ATKStatusEfc = _Attacker->GetScript<ER_DataScript_Character>()->GetStatusEffect();
	tStatus_Effect* HitStatusEfc = _HitObj->GetScript<ER_DataScript_Character>()->GetStatusEffect();

	int SkillDmg = (_Inst->*_calc)();
	SkillDmg += SkillDmg * AtkInfo->iSkillAmplification;
	int Def = HitInfo->iDefense + HitStatusEfc->GetIncDEF() - HitStatusEfc->GetDecDEF();

	int CriticalDmg = IsCritical(AtkInfo);

	int FinalDmg = (int)(((SkillDmg * 100) / (100 + Def)) + (CriticalDmg * 0.65));

	// ���ó��
	if (HitInfo->iHP - FinalDmg <= 0)
	{
		// Ÿ���� HP�� 0���� �����.
		HitInfo->iHP = 0;

		tFSMData deaddata = {};
		_HitObj->GetScript<ER_ActionScript_Character>()->Dead(deaddata);
		return;
	}
	HitInfo->iHP -= FinalDmg;

	// ��������Ʈ ���
}


int ER_BattleSystem::IsCritical(tIngame_Stats* _info)
{
	// �������� �ʱ�ȭ
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int CriChance	= (int)_info->fCriticalStrikeChance * 100;
	int Probability = std::rand() % 100;

	// ũ��Ƽ�� �߻�
	if (Probability < CriChance)
		return 1;

	return 0;
}

tIngame_Stats* ER_BattleSystem::GetStatsInfo(CGameObject* _obj)
{
	int ObjLayer = _obj->GetLayerIndex();

	tIngame_Stats* Info = nullptr;

	switch (ObjLayer)
	{
	case 11:	// Monster
	{
		// Monster Data Script���� ������
		break;
	}
	case 12:	// Character
	case 13:	// Player
	{
		Info = _obj->GetScript<ER_DataScript_Character>()->GetStatus();
		break;
	}
	}
	
	return Info;
}
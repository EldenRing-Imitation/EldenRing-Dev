#include "pch.h"
#include "ER_BattleSystem.h"

#include "ER_DataScript_Character.h"

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
	
	int AtkPower = AtkInfo->iAttackPower;
	int Def = HitInfo->iDefense;

	int CriticalDmg = IsCritical(AtkInfo);

	int FinalDmg = (AtkPower * 100) / (100 + Def) * (CriticalDmg * 0.65);
}

void ER_BattleSystem::Battle_Skill(CGameObject* _Attacker
	, CGameObject* _HitObj
	, CScript* _Inst
	, SKILL_DMG_CALC _calc
	, const tSkill_Info* _SkillData)
{
	int a = (_Inst->*_calc)(_SkillData);
}


int ER_BattleSystem::IsCritical(tIngame_Stats* _info)
{
	// �������� �ʱ�ȭ
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	int CriChance	= _info->fCriticalStrikeChance * 100;
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
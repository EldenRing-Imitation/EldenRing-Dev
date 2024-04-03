#pragma once

#define STATEDELEGATE_ENTER(obj, CharacterName, func) obj->SetStateEnter((SCRIPT_DELEGATE)&##ER_ActionScript_##CharacterName::func##Enter)
#define STATEDELEGATE_UPDATE(obj, CharacterName, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&ER_ActionScript_##CharacterName::func##Update)
#define STATEDELEGATE_EXIT(obj, CharacterName, func) obj->SetStateExit((SCRIPT_DELEGATE)&##ER_ActionScript_##CharacterName::func##Exit)

#define STATEDATA_SET(State, FSMData) StateList[(UINT)ER_CHAR_ACT::State]->SetData(FSMData)
#define STATEDATA_GET(State) StateList[(UINT)ER_CHAR_ACT::State]->GetData()

#define BATTLE_SKILL(AttackObj, HittedObj, className, CalcFunc, skilldata, hitstep) ER_BattleSystem::GetInst()->Battle_Skill(AttackObj, HittedObj, this, (SKILL_DMG_CALC)&className::CalcFunc, skilldata, hitstep)
#define BATTLE_COMMON(AttackObj, HittedObj) ER_BattleSystem::GetInst()->Battle_Common(AttackObj, (CGameObject*)HittedObj)

#define GETITEMSTATS(ItemObj) ItemObj->GetScript<ER_DataScript_Item>()->GetStats()

#define GETPROJECTILE(Type) ER_ProjectilePool::GetInst()->GetProjectile(ER_ProjectilePool::eProjType::Type)

#define ERCHARSOUND(Key) PlaySound((UINT)CharacterSound::Key)
#define STOPSOUND(Key) m_pSounds[(UINT)CharacterSound::Key]->Stop()

enum class ER_ITEM_GRADE
{
	NORMAL,		// �Ϲ�
	UNCOMMON,	// ���
	RARE,		// ���
	EPIC,		// ����
};

enum class ER_ITEM_TYPE
{
	EQUIPMENT,			// ���
	CONSUMABLES,		// �Һ�
	INGREDIENT,			// ���
};

enum class ER_ITEM_SLOT
{
	WEAPONS,			// ����
	CHEST,				// ��
	HEAD,				// �Ӹ�
	ARM,				// ��
	LEG,				// �ٸ�
	END,				// �����Ұ�
};

enum class ER_WEAPON_TYPE
{
	AXE,				// ����
	SWORD,				// ��
	PISTOL,				// ����
	GLOVE,				// �۷���
	BOW,				// Ȱ
	END,				// �̼���
};

typedef union item_combination_pair
{
	struct
	{
		UINT ingredient_1;
		UINT ingredient_2;
	};

	DWORD_PTR recipe;
}ER_RECIPE;

typedef struct item_combination_slot
{
	UINT iSlot1;
	UINT iSlot2;
	UINT iAssemblyItem;
}ER_CMB_SLOT;

enum class eExpForLevelUp
{
	LEVEL1 = 0,
	LEVEL2 = 0,
	LEVEL3 = 0,
	LEVEL4 = 0,
	LEVEL5 = 0,
	LEVEL6 = 0,
	LEVEL7 = 0,
	LEVEL8 = 0,
	LEVEL9 = 0,
	LEVEL10 = 0,
	LEVEL11 = 0,
	LEVEL12 = 0,
	LEVEL13 = 0,
	LEVEL14 = 0,
	LEVEL15 = 0,
	LEVEL16 = 0,
	LEVEL17 = 0,
	LEVEL18 = 0,
	LEVEL19 = 0,
	LEVEL20 = 0
};

enum class eSkillindex
{
	Q_1,
	W_1,
	E_1,
	R_1,
	Q_2,
	W_2,
	E_2,
	R_2,
	SKILLMAXSIZE,
}typedef SKILLIDX;

enum class eStatus_Effect
{
	// ����
	DEFALUT			= 0,
	INCREASE_ATK	= 1 << 0,	// ���ݷ� ����
	INCREASE_DEF	= 1 << 1,	// ���� ����
	INCREASE_SPD	= 1 << 2,	// �̵��ӵ� ����
	INCREASE_APD	= 1 << 3,	// ���ݼӵ� ����
	
	// �����
	DECREASE_ATK	= 1 << 4,	// ���ݷ� ����
	DECREASE_DEF	= 1 << 5,	// ���� ����
	DECREASE_SPD	= 1 << 6,	// �̵��ӵ� ����
	DECREASE_APD	= 1 << 7,	// ���ݼӵ� ����
	FEAR			= 1 << 8,	// ����
	STUN			= 1 << 9,	// ����
}typedef BUFNDEBUF_ENUM;

enum class eAreaName
{
	ARCHERY,
	FOREST,
	HOTEL,
	SANDYBEACH,
	SCHOOL,
	UPTOWN,
	END,
}typedef LUMIAISLAND;
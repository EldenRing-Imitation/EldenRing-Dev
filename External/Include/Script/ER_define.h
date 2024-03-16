#pragma once

#define STATEDELEGATE_ENTER(obj, className, func) obj->SetStateEnter((SCRIPT_DELEGATE)&className::func##Enter)
#define STATEDELEGATE_UPDATE(obj, className, func) obj->SetStateUpdate((SCRIPT_DELEGATE)&className::func##Update)
#define STATEDELEGATE_EXIT(obj, className, func) obj->SetStateExit((SCRIPT_DELEGATE)&className::func##Exit)

#define STATEDATA_SET(State, FSMData) StateList[ER_CHAR_ACT::State]->SetData(FSMData)

enum class ER_ITEM_GRADE
{
	COMMON,		// �Ϲ�
	UNCOMMONE,	// ���
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
	NONE,				// �����Ұ�
	WEAPONS,			// ����
	HEAD,				// �Ӹ�
	CHEST,				// ��
	ARM,				// ��
	LEG,				// �ٸ�
};

enum class ER_WEAPON_TYPE
{
	NONE,				// �̼���
	AXE,				// ����
	SWORD,				// ��
	PISTOL,				// ����
	GLOVE,				// �۷���
	BOW					// Ȱ
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
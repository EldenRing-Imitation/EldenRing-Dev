#pragma once

enum class ER_ITEM_GRADE
{
	COMMON,		// �Ϲ�
	UNCOMMONE,	// ����
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
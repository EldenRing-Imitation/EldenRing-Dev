#pragma once


enum class ER_ITEM_CODE
{
	SCISSORS,				// ����	
	LEATHER,				// ����	
	TURTLE_SHELL,			// �ź��� �����		
	RUBBER,					// ��
	SCRAP_METAL,			// ��ö		
	PICKAXE,				// ���	
	FEATHER,				// ����	
	FLOWER,					// ��
	BRANCH,					// ��������
	RUSTY_SWORD,			// �콼 ��		
	SHORT_ROD,				// �ܺ�	
	STONE,					// ������
	LIGHTER,				// ������	
	RIBBON,					// ����
	STALLION_MEDAL,			// ����		
	FOUNTAIN_PEN,			// ������		
	HAMMER,					// ��ġ
	HAIRBAND,				// �Ӹ���	
	COTTON_GLOVES,			// ���尩		
	NAIL,					// ��
	WALTHER_PPK,			// ���� PPK		
	BATTERY,				// ���͸�	
	BANDAGE,				// �ش�	
	HOLY_GRAIL,				// ����	
	SHIRT,					// ����
	WATCH,					// �ո�ð�
	STEEL_CHAIN,			// ��罽		
	CROSS,					// ���ڰ�
	BOW,					// ���
	OIL,					// ����
	CLOTH,					// �ʰ�
	RUNNING_SHOES,			// �ȭ		
	GEMSTONE,				// ����	
	GLASS_BOTTLE,			// ������		
	HELMET,					// ������ ���
	GLUE,					// ������
	PAPER,					// ����
	SPARKLING_WATER,		// ź���			
	TRUMP_CARD,				// Ʈ���� ī��	
	BRACELET,				// ����	
	PIANO_WIRE,				// �ǾƳ뼱	
	GUNPOWDER,				// ȭ��	
	LEATHER_SHIELD,			// ���� ����		
	BROKEN_WATCH,			// ���峭 �ð�		
	MILITARY_SUIT,			// ����		
	OILCLOTH,				// �⸧���� õ	
	REPAIRED_SLIPPERS,		// ���� ������			
	RUBY,					// ���
	MAGNUM_PYTHON,			// �ű׳�-���̼�		
	CIRCLET,				// �Ӹ���	
	FEATHER_DUSTER,			// �������̰�		
	BOOTS,					// ����
	SHAMANS_BRONZE,			// ���Ĵܵ�		
	CHAIN_SCYTHE,			// �罽 ��		
	DEACON_ROBES,			// ������		
	LONG_SWORD,				// ���	
	SAINTS_RELIC,			// ������ ����		
	IRON_KNUCKLES,			// ���̾� ��Ŭ		
	SAFETY_HELMET,			// ������		
	FLOWER_OF_FATE,			// ����� ��		
	GLASS_PIECES,			// ���� ����		
	ION_BATTERY,			// �̿� ����		
	DIVING_SUIT,			// �����		
	LONGBOW,				// ���	
	ASH,					// ��
	ELECTRONIC_PARTS,		// ���� ��ǰ			
	BLUEPRINT,				// ������ ����	
	IRON_SHEET,				// ö��	
	HIGH_HEELS,				// ������	
	GOLD,					// Ȳ��
	STEEL_SHIELD,			// ��ö ����		
	GAUNTLET,				// ��Ʋ��	
	GOLDEN_BRACELET,		// ������			
	MAGNUM_ANACONDA,		// �ű׳�-�Ƴ��ܴ�			
	MOTOR,					// ����
	BALLISTIC_HELMET,		// ��ź��			
	BULLETPROOF_VEST,		// ��ź����			
	MIGHTY_BOW,				// ���±�	
	JEWEL_SWORD,			// ����		
	REAPERS_SCYTHE,			// ����ǳ�		
	AETHER_FRINGE,			// �����׸� ����		
	GLASS_PANEL,			// ������		
	COMBAT_BOOTS,			// ����ȭ		
	SUIT,					// ����
	TIARA,					// Ƽ�ƶ�
	SHEET_METAL_ARMOR,		// �Ǳ� ����			
	FEATHER_BOOTS,			// �淮ȭ ����		
	OPTICAL_CAMOUFLAGE_SUIT,// ���й�ä ��Ʈ					
	VIRTUOUS_OUTLAW,		// �κ�			
	MAGNUM_BOA,				// �ű׳�-����	
	VITAL_SIGN_SENSOR,		// ����Ż ����			
	TUXEDO,					// ���纹
	BUCEPHALUS,				// �����ȷν�	
	BRASIL_GAUNTLET,		// �극���� ��Ʋ��			
	CRUSADER_ARMOR,			// ������� ����		
	CRYSTAL_TIARA,			// ���� Ƽ�ƶ�		
	EXCALIBUR,				// ����Į����	
	MOTORCYCLE_HELMET,		// ������� ���			
	TACTICAL_OPS_HELMET,	// ���� - ops ���				
	JEBES_ALTERED_BOW,		// ������ Ȱ			
	HIGH_PRIEST_ROBES,		// �������� ����			
	WHITE_RHINOS,			// Ŭ���� ����		
	TINDALOS_BAND,			// ź�޷ν��� ����		
	STRAITJACKET_SNEAKERS,	// ǳȭ��				
	PLASMA_ARC,				// �ö�� ��ũ	
	HARPE,					// �ϸ���
	EOD_SUIT,				// EOD ��Ʈ
	END,
};

enum class ER_ITEM_GRADE
{
	NONE,
	COMMON,		// �Ϲ�
	UNCOMMONE,	// ���
	RARE,		// ���
	EPIC,		// ����
};

enum class ER_ITEM_TYPE
{
	NONE,
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
	END

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

extern const wchar_t* ER_ITEM_WSTR[(UINT)ER_ITEM_CODE::END];
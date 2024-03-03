#pragma once
#include <Engine/CSingleton.h>
#include "ER_Item.h"

class ER_ItemMgr
	: public CSingleton<ER_ItemMgr>
{
	SINGLE(ER_ItemMgr);

private:
	vector<ER_Item*> m_vecItem;						// ������ ����Ʈ
	unordered_map<DWORD_PTR, UINT> m_umapRecipe;	// ���ս� ���
	

private:
	int Save();
	int Load();

	int SaveItemData(ER_Item* _Item, FILE* _File);
	ER_Item* LoadItemData(FILE* _File);

	void RecipeUpdate();

public:
	int SearchRecipe(UINT _ingr_1, UINT _ingr_2, int& _res);

public:
	void init();
	void Update();

	friend class ItemDataUI;

};
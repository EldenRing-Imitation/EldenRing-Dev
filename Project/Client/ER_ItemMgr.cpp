#include "pch.h"
#include "ER_ItemMgr.h"

#include "ImGuiMgr.h"
#include "ItemDataUI.h"

ER_ItemMgr::ER_ItemMgr()
{
}

ER_ItemMgr::~ER_ItemMgr()
{
	Safe_Del_Vec(m_vecItem);
}

void ER_ItemMgr::init()
{
	// ������ ������ �ε�
	Load();

	// ������ ������Ʈ
	RecipeUpdate();
	((ItemDataUI*)ImGuiMgr::GetInst()->FindUI("##ItemDataUI"))->RegistItemMgr();
}
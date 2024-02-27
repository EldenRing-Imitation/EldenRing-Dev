#include "pch.h"
#include "ER_ItemMgr.h"

#include <Engine\CPathMgr.h>

extern const wchar_t* ER_ITEM_WSTR[(UINT)ER_ITEM_CODE::END] =
{
	L"����",
	L"����",
	L"�ź��� �����",
	L"��",
	L"��ö",
	L"���",
	L"����",
	L"��",
	L"��������",
	L"�콼 ��",
	L"�ܺ�",
	L"������",
	L"������",
	L"����",
	L"����",
	L"������",
	L"��ġ",
	L"�Ӹ���",
	L"���尩",
	L"��",
	L"���� PPK",
	L"���͸�",
	L"�ش�",
	L"����",
	L"����",
	L"�ո�ð�",
	L"��罽",
	L"���ڰ�",
	L"���",
	L"����",
	L"�ʰ�",
	L"�ȭ",
	L"����",
	L"������",
	L"������ ���",
	L"������",
	L"����",
	L"ź���",
	L"Ʈ���� ī��",
	L"����",
	L"�ǾƳ뼱",
	L"ȭ��",
	L"���� ����",
	L"���峭 �ð�",
	L"����",
	L"�⸧���� õ",
	L"���� ������",
	L"���",
	L"�ű׳� - ���̼�",
	L"�Ӹ���",
	L"�������̰�",
	L"����",
	L"���Ĵܵ�",
	L"�罽 ��",
	L"������",
	L"���",
	L"������ ����",
	L"���̾� ��Ŭ",
	L"������",
	L"����� ��",
	L"���� ����",
	L"�̿� ����",
	L"�����",
	L"���",
	L"��",
	L"���� ��ǰ",
	L"������ ����",
	L"ö��",
	L"������",
	L"Ȳ��",
	L"��ö ����",
	L"��Ʋ��",
	L"������",
	L"�ű׳� - �Ƴ��ܴ�",
	L"����",
	L"��ź��",
	L"��ź����",
	L"���±�",
	L"����",
	L"����ǳ�",
	L"�����׸� ����",
	L"������",
	L"����ȭ",
	L"����",
	L"Ƽ�ƶ�",
	L"�Ǳ� ����",
	L"�淮ȭ ����",
	L"���й�ä ��Ʈ",
	L"�κ�",
	L"�ű׳� - ����",
	L"����Ż ����",
	L"���纹",
	L"�����ȷν�",
	L"�극���� ��Ʋ��",
	L"������� ����",
	L"���� Ƽ�ƶ�",
	L"����Į����",
	L"������� ���",
	L"���� - ops ���",
	L"������ Ȱ",
	L"�������� ����",
	L"Ŭ���� ����",
	L"ź�޷ν��� ����",
	L"ǳȭ��",
	L"�ö�� ��ũ",
	L"�ϸ���",
	L"EOD ��Ʈ"
};

void ER_ItemMgr::RegistItem()
{
	ER_Item* item = new ER_Item();
	item->m_strItemName = L"���";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 0;
	item->m_eGrade = 2;
	item->m_eType = 2;
	item->m_eSlot = 2;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);

	item = new ER_Item();
	item->m_strItemName = L"����Į����";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 1;
	item->m_eGrade = 1;
	item->m_eType = 1;
	item->m_eSlot = 1;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);

	item = new ER_Item();
	item->m_strItemName = L"������ Ȱ";
	item->m_ItemIcon = nullptr;
	item->m_eItemCode = 2;
	item->m_eGrade = 3;
	item->m_eType = 3;
	item->m_eSlot = 3;
	item->m_tItemStats = {};

	m_vecItem.push_back(item);
}

void ER_ItemMgr::RegistRecipe()
{

}

int ER_ItemMgr::Save()
{
	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_item = path_content.wstring() + L"Data";

	if (false == exists(path_item))
	{
		create_directory(path_item);
	}

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"Data\\Item.data";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// Item Cnt
	size_t iItemCnt = m_vecItem.size();
	fwrite(&iItemCnt, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iItemCnt; ++i)
	{
		SaveItemData(m_vecItem[i], pFile);
	}
	
	fclose(pFile);

	return S_OK;
}

int ER_ItemMgr::Load()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += L"Data\\Item.data";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return S_FALSE;

	size_t iItemCnt = 0;
	fread(&iItemCnt, sizeof(size_t), 1, pFile);

	m_vecItem.resize(iItemCnt);

	// Item Data Load
	for (size_t i = 0; i < iItemCnt; ++i)
	{
		m_vecItem[i] = (LoadItemData(pFile));
		
		// recipe�� �ִ� ���
		if (0 != m_vecItem[(UINT)i]->m_uniRecipe.recipe)
			AddRecipe(m_vecItem[(UINT)i]->m_uniRecipe.recipe, (UINT)i);
	}
	
	fclose(pFile);

	return S_OK;
}

int ER_ItemMgr::SaveItemData(ER_Item* _Item, FILE* _File)
{
	// ItemName
	SaveWString(_Item->m_strItemName, _File);

	// Texture
	SaveResRef(_Item->m_ItemIcon.Get(), _File);

	// ItemCode
	fwrite(&_Item->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fwrite(&_Item->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fwrite(&_Item->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fwrite(&_Item->m_eSlot, sizeof(UINT), 1, _File);

	// ItemRecipe
	fwrite(&_Item->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	fwrite(&_Item->m_tItemStats, sizeof(ER_tStats), 1, _File);

	return 0;
}

ER_Item* ER_ItemMgr::LoadItemData(FILE* _File)
{
	ER_Item* pItem = new ER_Item;

	// Item Name
	wstring strName;
	LoadWString(strName, _File);
	pItem->SetItemName(strName.c_str());

	// Texture
	LoadResRef(pItem->m_ItemIcon, _File);

	// ItemCode
	fread(&pItem->m_eItemCode, sizeof(UINT), 1, _File);

	// ItemGrade
	fread(&pItem->m_eGrade, sizeof(UINT), 1, _File);

	// ItemType
	fread(&pItem->m_eType, sizeof(UINT), 1, _File);

	// ItemSlot
	fread(&pItem->m_eSlot, sizeof(UINT), 1, _File);

	// Item Recipe
	fread(&pItem->m_uniRecipe, sizeof(DWORD_PTR), 1, _File);

	// ItemStat
	fread(&pItem->m_tItemStats, sizeof(ER_tStats), 1, _File);

	return pItem;
}

int ER_ItemMgr::AddRecipe(DWORD_PTR _recipe, UINT _ItemCode)
{
	m_umapRecipe.insert(make_pair(_recipe, _ItemCode));
	
	return 0;
}

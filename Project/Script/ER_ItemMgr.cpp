#include "pch.h"
#include "ER_ItemMgr.h"

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
}

void ER_ItemMgr::random()
{
	// ���� ������ �ʱ�ȭ
	std::random_device rd;  // �ϵ���� ���� �����⸦ ����Ͽ� �õ� ����
	std::mt19937 gen(rd()); // �õ� ������ ���� ���� ���� �ʱ�ȭ
	std::uniform_int_distribution<> dis01(0, 41); // 0�� 41�� �����ϴ� �������� ������ ���� ����
	std::uniform_int_distribution<> dis02(1, 4); // 0�� 41�� �����ϴ� �������� ������ ���� ����

	// ������ ���� ���� �� ���
	int random_number01 = dis01(gen);
	int random_number02 = dis02(gen);
}

void ER_ItemMgr::GetIngredient(UINT _ItemID, std::queue<UINT>* _IngredientQueue)
{
	// ���վ��������� �˻�
	// ���վ������� �ƴ� ���
	if (m_umapIngredient.end() == m_umapIngredient.find(_ItemID))
	{
		(*_IngredientQueue).push(_ItemID);
		return;
	}

	ER_RECIPE recipe = {};
	recipe.recipe = m_umapIngredient.find(_ItemID)->second;

	// ������������ �˻� ����
	GetIngredient(recipe.ingredient_1, _IngredientQueue);
	GetIngredient(recipe.ingredient_2, _IngredientQueue);
}

void ER_ItemMgr::GetIngredient(UINT _ItemID, unordered_map<UINT, int>* _Ingrediendmap)
{
	// ���� ���� ������� ��Ͽ� ���� �ʴ´�.
	if (m_umapIngredient.end() == m_umapIngredient.find(_ItemID))
		return;

	// �ʿ� ������ ��Ͽ� �ִ¾��������� �˻�
	if (_Ingrediendmap->end() != _Ingrediendmap->find(_ItemID))
	{
		// �ִ� �������ΰ�� ������ �÷��ش�.
		_Ingrediendmap->find(_ItemID)->second++;
	}
	else
	{
		// �űԷ� �־��ش�
		_Ingrediendmap->insert(make_pair(_ItemID, 1));
	}
	

	ER_RECIPE recipe = {};
	recipe.recipe = m_umapIngredient.find(_ItemID)->second;

	// ������������ �˻� ����
	GetIngredient(recipe.ingredient_1, _Ingrediendmap);
	GetIngredient(recipe.ingredient_2, _Ingrediendmap);
}

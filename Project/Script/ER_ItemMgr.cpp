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

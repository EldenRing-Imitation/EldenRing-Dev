#include "pch.h"
#include "CSQLMgr.h"

//�Ŵ��� ����
#include "CPathMgr.h"

CSQLMgr::CSQLMgr()
	:db(nullptr)
{
}

CSQLMgr::~CSQLMgr()
{
}

void CSQLMgr::Init()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	int rc = sqlite3_open("example.db", &db);
	if (rc) {
		int a = 0;
	}
	else {
		int a = 0;
	}
}

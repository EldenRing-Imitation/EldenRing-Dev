#include "pch.h"
#include "CSQLMgr.h"

//�Ŵ��� ����
#include "CPathMgr.h"

//assert ��ũ��
#define EXECQUERY(Query, ErrMsg)		\
	if (sqlite3_exec(db, Query, 0, 0, &ErrMsg) != SQLITE_OK)	\
		{ \
			assert(false);	\
		}

CSQLMgr::CSQLMgr()
	:db(nullptr)
{
}

CSQLMgr::~CSQLMgr()
{
	sqlite3_close(db);
}

void CSQLMgr::init()
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	wstring upperPath = strPath + L"SQLLight\\EldenRing_DB.db";
	
	string stringResult;
	stringResult.assign(upperPath.begin(), upperPath.end());

	const char* cstr = stringResult.c_str();

	int rc = sqlite3_open(cstr, &db);
	if (rc) {
		assert(false);
	}
}

int CSQLMgr::InsertToLevel(wstring _levelName)
{
	wstring query = L"INSERT INTO Level(Level_Name) VALUES('" + _levelName + L"');";

	string sQuery;
	sQuery.assign(query.begin(), query.end());

	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

int CSQLMgr::InsertToLayer(int _levelID, wstring _layerName)
{
	wstring query = L"INSERT INTO Layer(Layer_Name, Level_ID) VALUES('" + _layerName + L" '," + std::to_wstring(_levelID)+ L");";

	string sQuery;
	sQuery.assign(query.begin(), query.end());

	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

int CSQLMgr::InsertToGameObject(int _layerID, wstring _gameObjectName)
{
	wstring query = L"INSERT INTO GameObject(GameObject_Name, Layer_ID) VALUES('" + _gameObjectName + L" '," + std::to_wstring(_layerID) + L");";

	string sQuery;
	sQuery.assign(query.begin(), query.end());

	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);

	long long insertedId = sqlite3_last_insert_rowid(db);

	return insertedId;
}

void CSQLMgr::InsertToComponent(int _gameObjectID, int _type, wstring _attribute)
{
}

void CSQLMgr::DeleteAllRecordToAllTable()
{
	string sQuery = "DELETE FROM Level;";
	const char* cQuery = sQuery.c_str();

	char* errMsg;
	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM Layer;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM GameObject;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);

	sQuery = "DELETE FROM Component;";
	cQuery = sQuery.c_str();

	EXECQUERY(cQuery, errMsg);
}

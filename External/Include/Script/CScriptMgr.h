#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	EDITCAMCONTROLSCRIPT,
	FOLLOWMAINCAMSCRIPT,
	MONSTERSCRIPT,
	PLANETSCRIPT,
	ER_ACTIONSCRIPT_AYA,
	ER_ACTIONSCRIPT_HYUNWOO,
	ER_ACTIONSCRIPT_JACKIE,
	ER_ACTIONSCRIPT_RIO,
	ER_ACTIONSCRIPT_YUKI,
	ER_ARROWEFFECTSCRIPT,
	ER_CAMCONTROLLERSCRIPT,
	ER_CURSOR,
	ER_DATASCRIPT_ARROW,
	ER_DATASCRIPT_BULLET,
	ER_DATASCRIPT_CHARACTER,
	ER_DATASCRIPT_ITEM,
	ER_DATASCRIPT_ITEMBOX,
	ER_DATASCRIPT_LANDMESHBASE,
	ER_EFFECTSCRIPT,
	ER_PLAYERSCRIPT,
	ER_PLAYERSCRIPT_RANGE,
	ER_RIOBAEFFECT,
	ER_RIOQEFFECT,
	ER_UISCRIPT_CRAFTSLOT,
	ER_UISCRIPT_GAUGE,
	ER_UISCRIPT_ITEMBOX,
	ER_UISCRIPT_ITEMSLOT,
	ER_UISCRIPT_SKILLSLOT,
	ER_UISCRIPT_SKILLUPBTN,
	ER_UISCRIPT_STATUSBARGAUGE,
	ER_UISCRIPT_TRACKINGSTATUSBAR,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};

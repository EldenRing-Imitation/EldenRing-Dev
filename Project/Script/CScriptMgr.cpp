#include "pch.h"
#include "CScriptMgr.h"

#include "CEditCamControlScript.h"
#include "CFollowMainCamScript.h"
#include "CMonsterScript.h"
#include "CPlanetScript.h"
#include "ER_ActionScript_Aya.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_AIScript_Aya.h"
#include "ER_AIScript_Hyunwoo.h"
#include "ER_AIScript_Rio.h"
#include "ER_AIScript_Yuki.h"
#include "ER_ArrowEffectScript.h"
#include "ER_AyaBAEffect.h"
#include "ER_AyaQEffect.h"
#include "ER_AyaREffect.h"
#include "ER_AyaWBulletEffect.h"
#include "ER_BulletEffectScript.h"
#include "ER_CamControllerScript.h"
#include "ER_Cursor.h"
#include "ER_DataScript_Arrow.h"
#include "ER_DataScript_Bullet.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_LandMeshBase.h"
#include "ER_EffectScript.h"
#include "ER_HyunwooBAEffect.h"
#include "ER_HyunwooQEffect.h"
#include "ER_HyunwooWEffect.h"
#include "ER_PlayerScript.h"
#include "ER_PlayerScript_Range.h"
#include "ER_RioBAEffect.h"
#include "ER_RioQEffect.h"
#include "ER_UIScript_CraftSlot.h"
#include "ER_UIScript_Gauge.h"
#include "ER_UIScript_ItemBox.h"
#include "ER_UIScript_ItemSlot.h"
#include "ER_UIScript_SkillSlot.h"
#include "ER_UIScript_SkillUpBtn.h"
#include "ER_UIScript_StatusBarGauge.h"
#include "ER_UIScript_TimeUIBar.h"
#include "ER_UIScript_TrackingStatusBar.h"
#include "ER_YukiBAEffect.h"
#include "ER_YukiEEffect.h"
#include "ER_YukiQEffect.h"
#include "ER_YukiREffect.h"
#include "ER_YukiWEffect.h"
#include "HyunwooEEffect.h"
#include "HyunwooREffect.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CEditCamControlScript");
	_vec.push_back(L"CFollowMainCamScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlanetScript");
	_vec.push_back(L"ER_ActionScript_Aya");
	_vec.push_back(L"ER_ActionScript_Hyunwoo");
	_vec.push_back(L"ER_ActionScript_Jackie");
	_vec.push_back(L"ER_ActionScript_Rio");
	_vec.push_back(L"ER_ActionScript_Yuki");
	_vec.push_back(L"ER_AIScript_Aya");
	_vec.push_back(L"ER_AIScript_Hyunwoo");
	_vec.push_back(L"ER_AIScript_Rio");
	_vec.push_back(L"ER_AIScript_Yuki");
	_vec.push_back(L"ER_ArrowEffectScript");
	_vec.push_back(L"ER_AyaBAEffect");
	_vec.push_back(L"ER_AyaQEffect");
	_vec.push_back(L"ER_AyaREffect");
	_vec.push_back(L"ER_AyaWBulletEffect");
	_vec.push_back(L"ER_BulletEffectScript");
	_vec.push_back(L"ER_CamControllerScript");
	_vec.push_back(L"ER_Cursor");
	_vec.push_back(L"ER_DataScript_Arrow");
	_vec.push_back(L"ER_DataScript_Bullet");
	_vec.push_back(L"ER_DataScript_Character");
	_vec.push_back(L"ER_DataScript_Item");
	_vec.push_back(L"ER_DataScript_ItemBox");
	_vec.push_back(L"ER_DataScript_LandMeshBase");
	_vec.push_back(L"ER_EffectScript");
	_vec.push_back(L"ER_HyunwooBAEffect");
	_vec.push_back(L"ER_HyunwooQEffect");
	_vec.push_back(L"ER_HyunwooWEffect");
	_vec.push_back(L"ER_PlayerScript");
	_vec.push_back(L"ER_PlayerScript_Range");
	_vec.push_back(L"ER_RioBAEffect");
	_vec.push_back(L"ER_RioQEffect");
	_vec.push_back(L"ER_UIScript_CraftSlot");
	_vec.push_back(L"ER_UIScript_Gauge");
	_vec.push_back(L"ER_UIScript_ItemBox");
	_vec.push_back(L"ER_UIScript_ItemSlot");
	_vec.push_back(L"ER_UIScript_SkillSlot");
	_vec.push_back(L"ER_UIScript_SkillUpBtn");
	_vec.push_back(L"ER_UIScript_StatusBarGauge");
	_vec.push_back(L"ER_UIScript_TimeUIBar");
	_vec.push_back(L"ER_UIScript_TrackingStatusBar");
	_vec.push_back(L"ER_YukiBAEffect");
	_vec.push_back(L"ER_YukiEEffect");
	_vec.push_back(L"ER_YukiQEffect");
	_vec.push_back(L"ER_YukiREffect");
	_vec.push_back(L"ER_YukiWEffect");
	_vec.push_back(L"HyunwooEEffect");
	_vec.push_back(L"HyunwooREffect");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CEditCamControlScript" == _strScriptName)
		return new CEditCamControlScript;
	if (L"CFollowMainCamScript" == _strScriptName)
		return new CFollowMainCamScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlanetScript" == _strScriptName)
		return new CPlanetScript;
	if (L"ER_ActionScript_Aya" == _strScriptName)
		return new ER_ActionScript_Aya;
	if (L"ER_ActionScript_Hyunwoo" == _strScriptName)
		return new ER_ActionScript_Hyunwoo;
	if (L"ER_ActionScript_Jackie" == _strScriptName)
		return new ER_ActionScript_Jackie;
	if (L"ER_ActionScript_Rio" == _strScriptName)
		return new ER_ActionScript_Rio;
	if (L"ER_ActionScript_Yuki" == _strScriptName)
		return new ER_ActionScript_Yuki;
	if (L"ER_AIScript_Aya" == _strScriptName)
		return new ER_AIScript_Aya;
	if (L"ER_AIScript_Hyunwoo" == _strScriptName)
		return new ER_AIScript_Hyunwoo;
	if (L"ER_AIScript_Rio" == _strScriptName)
		return new ER_AIScript_Rio;
	if (L"ER_AIScript_Yuki" == _strScriptName)
		return new ER_AIScript_Yuki;
	if (L"ER_ArrowEffectScript" == _strScriptName)
		return new ER_ArrowEffectScript;
	if (L"ER_AyaBAEffect" == _strScriptName)
		return new ER_AyaBAEffect;
	if (L"ER_AyaQEffect" == _strScriptName)
		return new ER_AyaQEffect;
	if (L"ER_AyaREffect" == _strScriptName)
		return new ER_AyaREffect;
	if (L"ER_AyaWBulletEffect" == _strScriptName)
		return new ER_AyaWBulletEffect;
	if (L"ER_BulletEffectScript" == _strScriptName)
		return new ER_BulletEffectScript;
	if (L"ER_CamControllerScript" == _strScriptName)
		return new ER_CamControllerScript;
	if (L"ER_Cursor" == _strScriptName)
		return new ER_Cursor;
	if (L"ER_DataScript_Arrow" == _strScriptName)
		return new ER_DataScript_Arrow;
	if (L"ER_DataScript_Bullet" == _strScriptName)
		return new ER_DataScript_Bullet;
	if (L"ER_DataScript_Character" == _strScriptName)
		return new ER_DataScript_Character;
	if (L"ER_DataScript_Item" == _strScriptName)
		return new ER_DataScript_Item;
	if (L"ER_DataScript_ItemBox" == _strScriptName)
		return new ER_DataScript_ItemBox;
	if (L"ER_DataScript_LandMeshBase" == _strScriptName)
		return new ER_DataScript_LandMeshBase;
	if (L"ER_EffectScript" == _strScriptName)
		return new ER_EffectScript;
	if (L"ER_HyunwooBAEffect" == _strScriptName)
		return new ER_HyunwooBAEffect;
	if (L"ER_HyunwooQEffect" == _strScriptName)
		return new ER_HyunwooQEffect;
	if (L"ER_HyunwooWEffect" == _strScriptName)
		return new ER_HyunwooWEffect;
	if (L"ER_PlayerScript" == _strScriptName)
		return new ER_PlayerScript;
	if (L"ER_PlayerScript_Range" == _strScriptName)
		return new ER_PlayerScript_Range;
	if (L"ER_RioBAEffect" == _strScriptName)
		return new ER_RioBAEffect;
	if (L"ER_RioQEffect" == _strScriptName)
		return new ER_RioQEffect;
	if (L"ER_UIScript_CraftSlot" == _strScriptName)
		return new ER_UIScript_CraftSlot;
	if (L"ER_UIScript_Gauge" == _strScriptName)
		return new ER_UIScript_Gauge;
	if (L"ER_UIScript_ItemBox" == _strScriptName)
		return new ER_UIScript_ItemBox;
	if (L"ER_UIScript_ItemSlot" == _strScriptName)
		return new ER_UIScript_ItemSlot;
	if (L"ER_UIScript_SkillSlot" == _strScriptName)
		return new ER_UIScript_SkillSlot;
	if (L"ER_UIScript_SkillUpBtn" == _strScriptName)
		return new ER_UIScript_SkillUpBtn;
	if (L"ER_UIScript_StatusBarGauge" == _strScriptName)
		return new ER_UIScript_StatusBarGauge;
	if (L"ER_UIScript_TimeUIBar" == _strScriptName)
		return new ER_UIScript_TimeUIBar;
	if (L"ER_UIScript_TrackingStatusBar" == _strScriptName)
		return new ER_UIScript_TrackingStatusBar;
	if (L"ER_YukiBAEffect" == _strScriptName)
		return new ER_YukiBAEffect;
	if (L"ER_YukiEEffect" == _strScriptName)
		return new ER_YukiEEffect;
	if (L"ER_YukiQEffect" == _strScriptName)
		return new ER_YukiQEffect;
	if (L"ER_YukiREffect" == _strScriptName)
		return new ER_YukiREffect;
	if (L"ER_YukiWEffect" == _strScriptName)
		return new ER_YukiWEffect;
	if (L"HyunwooEEffect" == _strScriptName)
		return new HyunwooEEffect;
	if (L"HyunwooREffect" == _strScriptName)
		return new HyunwooREffect;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::EDITCAMCONTROLSCRIPT:
		return new CEditCamControlScript;
		break;
	case (UINT)SCRIPT_TYPE::FOLLOWMAINCAMSCRIPT:
		return new CFollowMainCamScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLANETSCRIPT:
		return new CPlanetScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA:
		return new ER_ActionScript_Aya;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO:
		return new ER_ActionScript_Hyunwoo;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE:
		return new ER_ActionScript_Jackie;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO:
		return new ER_ActionScript_Rio;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI:
		return new ER_ActionScript_Yuki;
		break;
	case (UINT)SCRIPT_TYPE::ER_AISCRIPT_AYA:
		return new ER_AIScript_Aya;
		break;
	case (UINT)SCRIPT_TYPE::ER_AISCRIPT_HYUNWOO:
		return new ER_AIScript_Hyunwoo;
		break;
	case (UINT)SCRIPT_TYPE::ER_AISCRIPT_RIO:
		return new ER_AIScript_Rio;
		break;
	case (UINT)SCRIPT_TYPE::ER_AISCRIPT_YUKI:
		return new ER_AIScript_Yuki;
		break;
	case (UINT)SCRIPT_TYPE::ER_ARROWEFFECTSCRIPT:
		return new ER_ArrowEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_AYABAEFFECT:
		return new ER_AyaBAEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_AYAQEFFECT:
		return new ER_AyaQEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_AYAREFFECT:
		return new ER_AyaREffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_AYAWBULLETEFFECT:
		return new ER_AyaWBulletEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_BULLETEFFECTSCRIPT:
		return new ER_BulletEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return new ER_CamControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_CURSOR:
		return new ER_Cursor;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ARROW:
		return new ER_DataScript_Arrow;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_BULLET:
		return new ER_DataScript_Bullet;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return new ER_DataScript_Character;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return new ER_DataScript_Item;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX:
		return new ER_DataScript_ItemBox;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_LANDMESHBASE:
		return new ER_DataScript_LandMeshBase;
		break;
	case (UINT)SCRIPT_TYPE::ER_EFFECTSCRIPT:
		return new ER_EffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_HYUNWOOBAEFFECT:
		return new ER_HyunwooBAEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_HYUNWOOQEFFECT:
		return new ER_HyunwooQEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_HYUNWOOWEFFECT:
		return new ER_HyunwooWEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return new ER_PlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE:
		return new ER_PlayerScript_Range;
		break;
	case (UINT)SCRIPT_TYPE::ER_RIOBAEFFECT:
		return new ER_RioBAEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_RIOQEFFECT:
		return new ER_RioQEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_CRAFTSLOT:
		return new ER_UIScript_CraftSlot;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_GAUGE:
		return new ER_UIScript_Gauge;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMBOX:
		return new ER_UIScript_ItemBox;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT:
		return new ER_UIScript_ItemSlot;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT:
		return new ER_UIScript_SkillSlot;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLUPBTN:
		return new ER_UIScript_SkillUpBtn;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_STATUSBARGAUGE:
		return new ER_UIScript_StatusBarGauge;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_TIMEUIBAR:
		return new ER_UIScript_TimeUIBar;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_TRACKINGSTATUSBAR:
		return new ER_UIScript_TrackingStatusBar;
		break;
	case (UINT)SCRIPT_TYPE::ER_YUKIBAEFFECT:
		return new ER_YukiBAEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_YUKIEEFFECT:
		return new ER_YukiEEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_YUKIQEFFECT:
		return new ER_YukiQEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_YUKIREFFECT:
		return new ER_YukiREffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_YUKIWEFFECT:
		return new ER_YukiWEffect;
		break;
	case (UINT)SCRIPT_TYPE::HYUNWOOEEFFECT:
		return new HyunwooEEffect;
		break;
	case (UINT)SCRIPT_TYPE::HYUNWOOREFFECT:
		return new HyunwooREffect;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::EDITCAMCONTROLSCRIPT:
		return L"CEditCamControlScript";
		break;

	case SCRIPT_TYPE::FOLLOWMAINCAMSCRIPT:
		return L"CFollowMainCamScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLANETSCRIPT:
		return L"CPlanetScript";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA:
		return L"ER_ActionScript_Aya";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO:
		return L"ER_ActionScript_Hyunwoo";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE:
		return L"ER_ActionScript_Jackie";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO:
		return L"ER_ActionScript_Rio";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI:
		return L"ER_ActionScript_Yuki";
		break;

	case SCRIPT_TYPE::ER_AISCRIPT_AYA:
		return L"ER_AIScript_Aya";
		break;

	case SCRIPT_TYPE::ER_AISCRIPT_HYUNWOO:
		return L"ER_AIScript_Hyunwoo";
		break;

	case SCRIPT_TYPE::ER_AISCRIPT_RIO:
		return L"ER_AIScript_Rio";
		break;

	case SCRIPT_TYPE::ER_AISCRIPT_YUKI:
		return L"ER_AIScript_Yuki";
		break;

	case SCRIPT_TYPE::ER_ARROWEFFECTSCRIPT:
		return L"ER_ArrowEffectScript";
		break;

	case SCRIPT_TYPE::ER_AYABAEFFECT:
		return L"ER_AyaBAEffect";
		break;

	case SCRIPT_TYPE::ER_AYAQEFFECT:
		return L"ER_AyaQEffect";
		break;

	case SCRIPT_TYPE::ER_AYAREFFECT:
		return L"ER_AyaREffect";
		break;

	case SCRIPT_TYPE::ER_AYAWBULLETEFFECT:
		return L"ER_AyaWBulletEffect";
		break;

	case SCRIPT_TYPE::ER_BULLETEFFECTSCRIPT:
		return L"ER_BulletEffectScript";
		break;

	case SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return L"ER_CamControllerScript";
		break;

	case SCRIPT_TYPE::ER_CURSOR:
		return L"ER_Cursor";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ARROW:
		return L"ER_DataScript_Arrow";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_BULLET:
		return L"ER_DataScript_Bullet";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return L"ER_DataScript_Character";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return L"ER_DataScript_Item";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX:
		return L"ER_DataScript_ItemBox";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_LANDMESHBASE:
		return L"ER_DataScript_LandMeshBase";
		break;

	case SCRIPT_TYPE::ER_EFFECTSCRIPT:
		return L"ER_EffectScript";
		break;

	case SCRIPT_TYPE::ER_HYUNWOOBAEFFECT:
		return L"ER_HyunwooBAEffect";
		break;

	case SCRIPT_TYPE::ER_HYUNWOOQEFFECT:
		return L"ER_HyunwooQEffect";
		break;

	case SCRIPT_TYPE::ER_HYUNWOOWEFFECT:
		return L"ER_HyunwooWEffect";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return L"ER_PlayerScript";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE:
		return L"ER_PlayerScript_Range";
		break;

	case SCRIPT_TYPE::ER_RIOBAEFFECT:
		return L"ER_RioBAEffect";
		break;

	case SCRIPT_TYPE::ER_RIOQEFFECT:
		return L"ER_RioQEffect";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_CRAFTSLOT:
		return L"ER_UIScript_CraftSlot";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_GAUGE:
		return L"ER_UIScript_Gauge";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_ITEMBOX:
		return L"ER_UIScript_ItemBox";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT:
		return L"ER_UIScript_ItemSlot";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT:
		return L"ER_UIScript_SkillSlot";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_SKILLUPBTN:
		return L"ER_UIScript_SkillUpBtn";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_STATUSBARGAUGE:
		return L"ER_UIScript_StatusBarGauge";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_TIMEUIBAR:
		return L"ER_UIScript_TimeUIBar";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_TRACKINGSTATUSBAR:
		return L"ER_UIScript_TrackingStatusBar";
		break;

	case SCRIPT_TYPE::ER_YUKIBAEFFECT:
		return L"ER_YukiBAEffect";
		break;

	case SCRIPT_TYPE::ER_YUKIEEFFECT:
		return L"ER_YukiEEffect";
		break;

	case SCRIPT_TYPE::ER_YUKIQEFFECT:
		return L"ER_YukiQEffect";
		break;

	case SCRIPT_TYPE::ER_YUKIREFFECT:
		return L"ER_YukiREffect";
		break;

	case SCRIPT_TYPE::ER_YUKIWEFFECT:
		return L"ER_YukiWEffect";
		break;

	case SCRIPT_TYPE::HYUNWOOEEFFECT:
		return L"HyunwooEEffect";
		break;

	case SCRIPT_TYPE::HYUNWOOREFFECT:
		return L"HyunwooREffect";
		break;

	}
	return nullptr;
}
#pragma once
#include <Engine\CScript.h>
#include "CScriptMgr.h"

class ER_ActionScript_Character;
class ER_DataScript_Character;
class CBehaviorTree;


class Selector_Node;
class BTNode;

class ER_AIScript :
    public CScript
{
protected:
    ER_ActionScript_Character*  m_Character;
    ER_DataScript_Character*    m_pActionScript;
    CBehaviorTree*              m_pBehaviorTree;

    queue<CGameObject*>         m_queueItemBox; // ������� Ž���� �����۹ڽ� ť

protected:
    void SetBattleStartTime(float _fTime);
    void RegistAllItemBox(int _StartIdx);

    Selector_Node* SetERCharacterAIBase();  // ���� Sel Node �Ʒ��� ��ӱ���
    
    /*
    1.
                    Root
                Seq_CheckArrive
           Con_IsArrive  Sel_ExecuteAICommand
                 [ �ð��Ǵ� ]            [ ���� ���� ]
              Seq_CheckGameTime
    Con_CheckGamtime    Sel_ExploreCommand
      
      
    2.                  Sel_ExploreCommand
        Seq_SkillUp             Sel_Craft              Sel_Explore
    Con_IsHaveSkillPoint    Con_IsCraftState           Seq_Farming
    Act_SkillLevelUp                                Con_IsFarmingState
                              Seq_CraftItem            Act_Farming
                            Con_IsCanCraftItem       
                            Act_CraftItem               Seq_Explore
                                                       Con_hasNextBox
                                                     Act_FarmingCommand


    // Skill, Craft, Farming

    /*
                    [Root]
            [Seq_Dead]

    
    
    */

public:
    virtual void begin() override {}
    virtual void tick() override {}

    CGameObject* GetNextItemBox();

public:
    ER_AIScript(SCRIPT_TYPE _type);
    ~ER_AIScript();

    CLONE_DISABLE(ER_AIScript);
};


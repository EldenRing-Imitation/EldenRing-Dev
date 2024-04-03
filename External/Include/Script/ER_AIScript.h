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

protected:
    void SetBattleStartTime(float _fTime);

    Selector_Node* SetERCharacterAIBase();  // ���� Sel Node �Ʒ��� ��ӱ���
    
    /*
    1.
                    Root
                Seq_CheckArrive
           Con_IsArrive  Sel_ExecuteAICommand
                 [ �ð��Ǵ� ]            [ ���� ���� ]
              Seq_CheckGameTime
    Con_CheckGamtime    Sel_ExploreCommand
      
      
    2.        Sel_ExploreCommand
    ����
    ��ų����Ʈ�� �ִ���
    -> 

    3.
    ����
    ���۰��� �������� �ִ���
    ->

    4.                  


    */

    // Skill, Craft, Farming

    /*
                    [Root]
            [Seq_Dead]

    
    
    */

public:
    virtual void begin() override {}
    virtual void tick() override {}

public:
    ER_AIScript(SCRIPT_TYPE _type);
    ~ER_AIScript();

    CLONE_DISABLE(ER_AIScript);
};


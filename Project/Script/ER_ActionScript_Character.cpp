#include "pch.h"
#include "ER_ActionScript_Character.h"
#include "ER_DataScript_Character.h"

ER_ActionScript_Character::ER_ActionScript_Character(SCRIPT_TYPE _type)
	: CScript((UINT)_type)
	, FSMContext(nullptr)
	, StateList{}
	, m_Data(nullptr)
	, m_iCurState(0)
	, m_iPrevState(0)
	, m_AccessGrade(eAccessGrade::BASIC)
{
}
ER_ActionScript_Character::~ER_ActionScript_Character()
{
	if (FSMContext)
		delete FSMContext;

	for (int i = 0; i < (UINT)ER_CHAR_ACT::END; ++i)
	{
		if (StateList[i])
			delete StateList[i];
	}
}
void ER_ActionScript_Character::begin()
{
	StateInit();
	FSMContext = new FSM(StateList[(UINT)ER_CHAR_ACT::WAIT]);
	m_Data = GetOwner()->GetScript<ER_DataScript_Character>();

	ChangeState(ER_CHAR_ACT::ARRIVE);
}
void ER_ActionScript_Character::tick()
{
	FSMContext->UpdateState();
}

void ER_ActionScript_Character::Wait(tFSMData& _Data)
{
	// �⺻ ������
	if (IsAbleChange(eAccessGrade::BASIC))
		ChangeState(ER_CHAR_ACT::WAIT);
}
void ER_ActionScript_Character::Move(tFSMData& _Data)
{
	// �̵����� ���� �Ǵ�
	if (IsAbleChange(eAccessGrade::BASIC))
	{
		if (m_iCurState != (UINT)ER_CHAR_ACT::MOVE)
		{
			STATEDATA_SET(MOVE, _Data);
			ChangeState(ER_CHAR_ACT::MOVE);				// �̵����� �ƴϾ��ٸ� �̵����� ��ȯ
		}
		else
			FindPath()->FindPath(_Data.v4Data);			// �̵����̾��ٸ� ��ǥ���� ����
	}
}
void ER_ActionScript_Character::Rest(tFSMData& _Data)
{
	if (IsAbleChange(eAccessGrade::BASIC))
		ChangeState(ER_CHAR_ACT::REST);
}
void ER_ActionScript_Character::Attack(tFSMData& _Data)
{
	/*
	[ATTACK]
	bData[0]	: ���ݵ��� ���������� ����
	bData[1]	: Battle Event ���� ����
	bData[2]	: ���� Ÿ�� ���� ����
	iData[0]	: Ÿ������ �ִϸ��̼� ������ = Hit Frame
	lParam		: Ÿ�� ������Ʈ
	RParam		: Ÿ�� ���� ������Ʈ

	[MOVE]
	bData[0]	: Ÿ�� ���� ����
	fData		: ���� ���� �Ÿ�
	iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ��, 2 - �����۹ڽ�
	v4Data		: ��ǥ �̵� ��ǥ
	*/

	// Ÿ�� ������Ʈ ������� �Ǵ�
	if (((CGameObject*)_Data.lParam)->GetScript<ER_DataScript_Character>()->IsDeadState())
		return;

	// ���� ���ݻ����� �����͸� �޾ƿ´�
	tFSMData PrevData = STATEDATA_GET(ATTACK);

	//  ���� �������̾��°�
	if (PrevData.bData[0])
	{
		_Data.bData[0]	= PrevData.bData[0];	// ���� ���� ����
		_Data.bData[1]	= PrevData.bData[1];	// Hit ���� ����
		_Data.fData		= PrevData.fData;		// ���� ���� �Ÿ� ����
		_Data.iData[0]  = PrevData.iData[0];		// HitFrame ����

		// ���ο� Ÿ���� �����ߴ°�
		if (!(PrevData.lParam) && _Data.lParam != PrevData.lParam)
		{
			_Data.RParam	= _Data.lParam;			// ���ο� Ÿ���� ���� Ÿ������ ����
			_Data.lParam	= PrevData.lParam;		// ����Ÿ�� ����
			_Data.bData[2]	= true;					// ����Ÿ�� �����Ǿ�����
		}

		STATEDATA_SET(ATTACK, _Data);				// ������ ������Ʈ
		return;										// ���¸� �������� �ʰ� ����
	}
	// Ÿ�� �������̴�
	else
	{
		_Data.fData = GetStatus()->fAtkRange;		// �Ϲݰ��� ���� �Ÿ�;

		// ���� �����Ÿ� ���� �Ǵ�
		if (IsInRange((CGameObject*)_Data.lParam, _Data.fData))
		{
			STATEDATA_SET(ATTACK, _Data);			// �Է¹��� �����ͷ� ����
			SetStateGrade(eAccessGrade::BASIC);	// ����� ��Ұ���
			ChangeState(ER_CHAR_ACT::ATTACK);
		}
		else
		{
			tFSMData MoveData = {};
			MoveData.lParam		= _Data.lParam;		// Ÿ��
			MoveData.bData[0]	= true;				// Ÿ�� ������
			MoveData.fData		= _Data.fData;		// ���� ���� �Ÿ�
			MoveData.iData[0]	= 1;				// Ÿ�� Ÿ�� : 1 ���ݴ��
			MoveData.v4Data		= ((CGameObject*)_Data.lParam)->Transform()->GetRelativePos();
			Move(MoveData);
		}
	}

}
void ER_ActionScript_Character::Farming(tFSMData& _Data)
{
	/*
	����Լ�
	�Ÿ��Ǵ� �� �������
	
	Player :
	�Ĺ� ���

	Enter -> UIȣ��
	Update -> UI �����ϸ鼭 UIObject�� ��ȣ�ۿ� 
	Exit ->	UI�� ȣ�������Ѵ�.

	AI (Player�� �ƴ�):

	Enter -> �ƹ�������������
	Update -> Time 2�� �Ҹ�
	: 
	
	�ʿ��Ʈ�� ���۰��� �������� �ִٸ� ���� -> Craft �Լ� ȣ��
	lParam�� ����Object�����ϰ� Craft ������ �ٽ� �Ĺ�ȣ��			- �Ұ��Ҷ����� �ݺ�

	���� �������� ���°�� �ʿ������ Ȯ��
	������ �ִ� -> ����
	������ ���� -> AI��ũ��Ʈ�� Ÿ�� ���ڿ��� ����.


	[FARMING]
	bData[0]	: �Ĺ������� ����
	fData		: �Ĺ� ���� �Ÿ�
	lParam		: Ÿ�ٹڽ� ������Ʈ
	
	[MOVE]
	bData[0]	: Ÿ�� ���� ����
	fData		: �Ĺ� ���� �Ÿ�
	iData[0]	: Ÿ�� Ÿ�� : 1 - ���ݴ��, 2 - �����۹ڽ�
	v4Data		: ��ǥ �̵� ��ǥ
	*/

	// ���� ���ݻ����� �����͸� �޾ƿ´�
	tFSMData PrevData = STATEDATA_GET(FARMING);

	//  ���� �Ĺ����̰� �����ڽ��� �Ĺָ���� ���ȴ°�
	if (PrevData.bData[0] && PrevData.lParam == _Data.lParam)
		return;						// ���¸� �������� �ʰ� ����
	else
	{
		float fFarmingDist = 2.f;								// �Ĺְ��� �Ÿ� 2�Ÿ�
		
			// �����۹ڽ��� ���� ���� �Ÿ����ִ���
		if (IsInRange((CGameObject*)_Data.lParam, fFarmingDist))
		{
			STATEDATA_SET(FARMING, _Data);
			SetStateGrade(eAccessGrade::BASIC);	// ����� ��Ұ���
			ChangeState(ER_CHAR_ACT::FARMING);
		}
		else
		{
			tFSMData MoveData	= {};
			MoveData.bData[0]	= true;				// �Ĺֹڽ� ������
			MoveData.iData[0]   = 2;				// Ÿ�� Ÿ�� : 2 - �����۹ڽ�
			MoveData.v4Data = ((CGameObject*)_Data.lParam)->Transform()->GetRelativePos();
			Move(MoveData);
		}
	}
}
void ER_ActionScript_Character::Craft(tFSMData& _Data)
{
	/*
	����� ������ �� ������ ��������
	
	���۰��� �������� ������ ���� �Լ���

	ER_CMB_SLOT ���� �޾ƿ��� : int 1 ���Թ�ȣ, int 2 ���Թ�ȣ, UINT ������ ������
	�Ѱ��༭
	�ִϸ��̼� ����Ǹ� ���� 1, ���� 2 ������ Destory���ְ�,  nullptr�� ���� ����ְ�

	CharacterData ������ ���� �Լ� ȣ��
	

	[CRAFT]
	bData[0]	: ������ ����
	iData[0]	: ������ ������ �ڵ�
	iData[1]	: ���1 ������ ���� ��ȣ
	iData[2]	: ���2 ������ ���� ��ȣ
	*/

	// ���� ���ݻ����� �����͸� �޾ƿ´�
	tFSMData PrevData = STATEDATA_GET(CRAFT);

	//  ���� ������ �������ΰ�
	if (PrevData.bData[0])
		return;						// ���¸� �������� �ʰ� ����
	else
	{
		STATEDATA_SET(CRAFT, _Data);
		SetStateGrade(eAccessGrade::BASIC);	// ����� ��Ұ���
		ChangeState(ER_CHAR_ACT::CRAFT);
	};
}
void ER_ActionScript_Character::Dead(tFSMData& _Data)
{
	ChangeState(ER_CHAR_ACT::DEAD,eAccessGrade::UTMOST);
	m_Data->SetGameDead();	// ĳ���� ������� ó�� : Ÿ�������Ұ�, tick �������� ����
}

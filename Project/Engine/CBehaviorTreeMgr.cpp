#include "pch.h"
#include "CBehaviorTreeMgr.h"

#include "CPathMgr.h"
#include "CBehaviorTree.h"

int CBehaviorTreeMgr::SaveBTNode(BTNode* _pNode, FILE* _File)
{
    // === ��� ���� ===

    // ��� �̸�
    SaveWString(_pNode->GetNodeName(), _File);

    // ��� Ÿ��
    UINT NodeType = (UINT)_pNode->GetNodeType();
    fwrite(&NodeType, sizeof(UINT), 1, _File);

    // ��� �÷���
    UINT NodeFlag = _pNode->GetNodeFlag();
    fwrite(&NodeFlag, sizeof(UINT), 1, _File);

    // �ڽĳ�� ��
    UINT ChildCnt = _pNode->GetChildCnt();
    fwrite(&ChildCnt, sizeof(UINT), 1, _File);


    // ������ ����
    if (NodeType == (UINT)BT_DECORATOR ||
        NodeType == (UINT)BT_TASK)
    {
        // BB���� �߰��� �־��ٳ��� ���⿡ �ֱ�.
    }

    // === �ڽ� ��� ��ȸ���� ===
    for (auto child : _pNode->GetChild())
    {
        SaveBTNode(child, _File);
    }

    return 0;
}

Root_Node* CBehaviorTreeMgr::LoadBehaviorTree(const wstring& _BehaviorTreePath)
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _BehaviorTreePath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"rb");

    if (nullptr == pFile)
        return nullptr;

    // ��Ʈ ���� ����
    Root_Node* NewRoot = new Root_Node;

    wstring Name;   // ��� �̸�
    LoadWString(Name, pFile);
    UINT eType = 0; // ��� Ÿ��
    fread(&eType, sizeof(UINT), 1, pFile);
    UINT iFlag;     // ��� �÷���
    fread(&iFlag, sizeof(UINT), 1, pFile);
    UINT ChildCnt;  // �ڽĳ�� ��
    fread(&ChildCnt, sizeof(UINT), 1, pFile);

    BTNode* NewNode = CreateBTNode((BTNode::NODETYPE)eType, iFlag);

    // ������ ���� ����
    // ������ ���� �ֱ�

    LoadBTNode(pFile, NewNode);

    fclose(pFile);

    return NewRoot;
}

BTNode* CBehaviorTreeMgr::LoadBTNode(FILE* _File, BTNode* _Parent)
{
    wstring Name;   // ��� �̸�
    LoadWString(Name, _File);

    UINT eType = 0; // ��� Ÿ��
    fread(&eType, sizeof(UINT), 1, _File);

    UINT iFlag;     // ��� �÷���
    fread(&iFlag, sizeof(UINT), 1, _File);

    UINT ChildCnt;  // �ڽĳ�� ��
    fread(&ChildCnt, sizeof(UINT), 1, _File);

    Root_Node* Root = (Root_Node*)(_Parent->GetRootNode());

    BTNode* NewNode = CreateBTNode((BTNode::NODETYPE)eType, iFlag, Root);

    // ������ ����
    if (eType == (UINT)BT_DECORATOR ||
        eType == (UINT)BT_TASK)
    {
        // BB���� �߰��� �־��ٳ��� ���⿡ �ֱ�.
    }

    _Parent->AddChild(NewNode);

    for (size_t i = 0; i < ChildCnt; ++i)
    {
        LoadBTNode(_File, NewNode);
    }

    return NewNode;
}

BTNode* CBehaviorTreeMgr::CreateBTNode(BTNode::NODETYPE _type, UINT _flag, BTNode* _RootNode)
{
    BTNode* NewNode = nullptr;
    Root_Node* Root = dynamic_cast<Root_Node*>(_RootNode);

    switch (_type)
    {
    case BT_ROOT:
        NewNode = new Root_Node;
        Root = (Root_Node*)NewNode;
        break;
    case BT_COMPOSITE:
        NewNode = new Composite_Node;
        break;
    case BT_DECORATOR:
        NewNode = new Decorator_Node;
        break;
    case BT_TASK:
        NewNode = new Task_Node;
        break;
    }

    // Root Node ����
    if (Root)
    {
        NewNode->SetRootNode(Root);
    }

    // Flag ����
    NewNode->SetNodeFlag(_flag);

    return NewNode;
}

int CBehaviorTreeMgr::SaveBehaviorTree(const wstring& _BehaviorTreePath, Root_Node* _mainRoot)
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _BehaviorTreePath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // ��Ʈ ���� ����

    // ������ ������ ����

    // ��� ����


    SaveBTNode(_mainRoot, pFile);

    fclose(pFile);

    return S_OK;
}

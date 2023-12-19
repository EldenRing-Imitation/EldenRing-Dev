#include "pch.h"
#include "CBehaviorTree.h"

#include "CPathMgr.h"

int SaveBTNode(BTNode* _pNode, FILE* _File);
BTNode* LoadBTNode(FILE* _File, BTNode* _Parent);
BTNode* CreateBTNode(NODETYPE _type, UINT _flag, BTNode* _RootNode = nullptr);

int SaveBehaviorTree(const wstring& _BehaviorTreePath, Root_Node* _mainRoot)
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

int SaveBTNode(BTNode* _pNode, FILE* _File)
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
    if (NodeType == (UINT)NODETYPE::DECORATOR ||
        NodeType == (UINT)NODETYPE::TASK)
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

Root_Node* LoadBehaviorTree(const wstring& _BehaviorTreePath)
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

    BTNode* NewNode = CreateBTNode((NODETYPE)eType, iFlag);
    
    // ������ ���� ����
    // ������ ���� �ֱ�

    LoadBTNode(pFile, NewNode);

    fclose(pFile);

    return NewRoot;
}

BTNode* LoadBTNode(FILE* _File, BTNode* _Parent)
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

    BTNode* NewNode = CreateBTNode((NODETYPE)eType, iFlag, Root);

    // ������ ����
    if (eType == (UINT)NODETYPE::DECORATOR ||
        eType == (UINT)NODETYPE::TASK)
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


BTNode* CreateBTNode(NODETYPE _type, UINT _flag, BTNode* _RootNode)
{
    BTNode* NewNode = nullptr;
    Root_Node* Root = dynamic_cast<Root_Node*>(_RootNode);

    switch (_type)
    {
    case NODETYPE::ROOT:
        NewNode = new Root_Node;
        Root = (Root_Node*)NewNode;
        break;
    case NODETYPE::COMPOSITE:
        NewNode = new Composite_Node;
        break;
    case NODETYPE::DECORATOR:
        NewNode = new Decorator_Node;
        break;
    case NODETYPE::TASK:
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
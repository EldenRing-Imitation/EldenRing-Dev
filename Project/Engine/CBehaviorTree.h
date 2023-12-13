#pragma once
#include "CComponent.h"

class CBehaviorTree;
class Decorator_Node;
class BTNode;

enum class NODETYPE
{
    ROOT = 0,
    COMPOSITE = 1,
    DECORATOR = 2,
    SERVICE = 3,
    TASK = 4,

    // Composite
    SELECTOR = 10,
    SEQUENCE = 11,
    RANDSELECTOR = 12,

    // Decorator
    BLACKBOARD = 20,             // Ư�� ������ Key�� ���� �����Ǿ��ִ��� Ȯ��
    COMPARE_BBENTRIES = 21,      // �� ������ Ű ���� ���Ͽ� ����� ���� ���� �Ǵ�

    // Service


    // Task
    CUSTOM_TASK = 40,            // Client���� ������ TASK ����
    PLAY_ANIMATION = 41,         // �ִϸ��̼� ���
    PLAY_SOUND = 42,             // ���� ���
    WAIT = 43,                   // ���ð� ���� : ������ �� �����Ͽ� ���
    WAIT_BLACKBOARD_TIME = 44,   // ���ð� ���� : �����忡 ������ �� ���
    
                            // Transform ����
};
enum class BT_STATUS
{
    NONE,
    SUCCESS,
    FAILURE,
    RUNNING,
};

class BTNode
{
private:
    int         m_iProgressIdx; // ���� �ε���
    wstring     m_NodeName;     // ��� �̸�
    NODETYPE    m_NodeType;     // ��� Ÿ��
    BTNode*     m_ParentNode;   // �θ���
    CBehaviorTree* m_OwnerBT;   // ������ BehaviorTree Component

private:
    void SetRunningNode(); 

protected:
    BTNode* GetParentNode() { return m_ParentNode; }
    void SetParentNode(BTNode* pNode) { m_ParentNode = pNode; }

public:
    NODETYPE GetNodeType() { return m_NodeType; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }

    virtual BT_STATUS Run() = 0;
    void SetOwnerTree(CBehaviorTree* OwnerTree) { m_OwnerBT = OwnerTree; }
    void SetBehaviorTree(CBehaviorTree* pBehaviorTree) { m_OwnerBT = pBehaviorTree; }
    CBehaviorTree* GetBehaviorTree() { return m_OwnerBT; }


public:
    BTNode(NODETYPE eType) : m_iProgressIdx(-1), m_NodeType(eType), m_ParentNode(nullptr), m_OwnerBT(nullptr) { m_NodeName = L"NoName"; }
    ~BTNode() {}
};
class BB
{
private:
    unordered_map<wstring, void*> m_ExternedItem;   // �ܺ� ������
    unordered_map<wstring, void*> m_CreatedItem;    // ��ü ���� ������
    
public:
    // �ܺο��� ������� �޸��ּҸ� �����忡 �����´�.
    template<typename T>
    T* AddItem(const wstring& key, T* Item)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        m_ExternedItem.insert(make_pair(key, Item));
        iter = m_ExternedItem.find(key);

        return (T*)(iter->second);
    }

    // ������ ���ο� ���� �޸𸮸� �Ҵ��Ͽ� �����͸� �Է��Ѵ�.
    template<typename T>
    T* AddItem(const wstring& key, T Data)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = T;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        return (T*)(iter->second);
    }

    template<typename T>
    T* FindItem(const wstring& key)
    {
        unordered_map<wstring, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        iter = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        return nullptr;
    }

    void ClearCreatedItem()
    {
        unordered_map<wstring, void*>::iterator iter = m_CreatedItem.begin();
        
        while (iter != m_CreatedItem.end())
        {
            delete iter->second;
            iter = m_CreatedItem.erase(iter);
        }
    }

public:
    BB() {}
    ~BB() { ClearCreatedItem(); }
};
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
    BTNode* m_ChildNode;
    BTNode* m_RunningNode;

public:
    virtual BT_STATUS Run() override 
    {
        if (m_RunningNode != nullptr)
        {
            if (m_RunningNode->Run() == BT_STATUS::RUNNING)
                return BT_STATUS::RUNNING;

            m_RunningNode = nullptr;

            return BT_STATUS::SUCCESS;
        }
        else if (m_ChildNode != nullptr)
            return m_ChildNode->Run();

        return BT_STATUS::NONE;
    };
    
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_ChildNode(nullptr), m_RunningNode(nullptr) { m_BlackBoard = new BB(); }
    ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); DELETE_UNVAILUBLE(m_ChildNode); m_RunningNode = nullptr; }
};
#pragma region Composite Node
// �б��� ��Ʈ�� �ش� �бⰡ ����Ǵ� ����� �⺻ ��Ģ
class Composite_Node : public BTNode
{
protected:
    Decorator_Node* m_AttachedNode; // ���ڷ����� ��� ����
    list<BTNode*> m_ChildNodes;     // �ڽĳ��

public:
    virtual BT_STATUS Run();
    virtual void NodeAttach(Decorator_Node* pNode);
    virtual void AddChildNode(BTNode* pNode)
    {
        m_ChildNodes.emplace_back(pNode);
    }

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE), m_AttachedNode(nullptr) {}
    ~Composite_Node() {}
};

class Selector : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

class Sequence : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

class RandomSelector : public Composite_Node
{
public:
    virtual BT_STATUS Run();
};

#pragma endregion
#pragma region Decorator Node
// ���ǽ�
class Decorator_Node : public BTNode
{
private:


public:
    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    ~Decorator_Node() {}
    
};
// ===============================
class Compare_BBEntries : public Decorator_Node
{
private:
    bool    IsEqualTo;
    wstring BBKeyA;
    wstring BBKeyB;

public:
    virtual BT_STATUS Run() override {}
    
public:
    Compare_BBEntries() : IsEqualTo(false) {}
    ~Compare_BBEntries() {}
};

#pragma endregion
#pragma region Service Node
// ������ ��忡 ����ġ, �������� Ȯ�� �� ������Ʈ�� ���.
class Service_Node : public BTNode
{
private:


public:
    Service_Node() : BTNode(NODETYPE::SERVICE) {}
    ~Service_Node() {}

};
#pragma endregion
#pragma region Task Node
// �������
class Task_Node : public BTNode
{
protected:
    Decorator_Node* m_AttachedNode; // ���ڷ����� ��� ����

public:
    virtual BT_STATUS Run();

public:
    Task_Node() : BTNode(NODETYPE::TASK), m_AttachedNode(nullptr) {}
    ~Task_Node() {}
};
#pragma endregion

class CBehaviorTree :
    public CComponent
{
private:
    Root_Node* m_RootNode;

private:
    BTNode* CreateNewNode(NODETYPE eType);

public:
    virtual void begin() {}
    virtual void tick();
    virtual void finaltick();

public:
    void SetRunningNode(BTNode* pNode) { m_RootNode->SetRunningNode(pNode); }
    void AddChildNode(BTNode* ParentNode, NODETYPE eType);
    void NodeAttach(BTNode* TargetNode ,NODETYPE eType);
    void SetNodeName(const wstring& name);


public:
    template<typename T>
    T* FindItem(const wstring& key) { return m_RootNode->GetBlackBoard()->FindItem(key); }

    // �ܺ� ������ �߰�
    template<typename T>
    T* AddItem(const wstring& key, T* Item) { return m_RootNode->GetBlackBoard()->AddItem(key, Item); }

    // �ű� ������ ���� �� ������ �Է�
    template<typename T>
    T* AddItem(const wstring& key, T Data) { return m_RootNode->GetBlackBoard()->AddItem(key, Data); }

    // �ű� ������ ���� (�޸� ������ ����)
    template<typename T>
    T* AddItem(const wstring& key) { return m_RootNode->GetBlackBoard()->AddItem(key); }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};


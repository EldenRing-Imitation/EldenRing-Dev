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

// ========================= ������ Ŭ���� =========================
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

// ========================= �⺻ ��� =========================
class BTNode
{
private:
    int             m_iProgressIdx; // ���� �ε���
    wstring         m_NodeName;     // ��� �̸�
    NODETYPE        m_NodeType;     // ��� Ÿ��
    CBehaviorTree*  m_OwnerBT;      // ������ BehaviorTree Component

protected:
    BTNode*         m_ParentNode;   // �θ���
    BTNode*         m_AttachedNode; // ���ڷ�����,���� ��� ����
    BTNode*         m_ChildNode;    // ���� �ڽ�
    list<BTNode*>   m_ChildNodes;   // ���� �ڽĸ��

public:
    virtual BT_STATUS Run() = 0;
    
    BTNode* GetChildNode() { return m_ChildNode; }
    list<BTNode*> GetChildNodes() { return m_ChildNodes; }

    void SetParentNode(BTNode* _ParentNode) { m_ParentNode = _ParentNode; }

    NODETYPE GetNodeType() { return m_NodeType; }
    
    const wstring& GetNodeName() { return m_NodeName; }
    CBehaviorTree* GetBehaviorTree() { return m_OwnerBT; }
    
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    void SetBehaviorTree(CBehaviorTree* pBehaviorTree) { m_OwnerBT = pBehaviorTree; }

    void DisconnectFromParent();


public:
    BTNode(NODETYPE eType) : m_iProgressIdx(-1), m_NodeType(eType), m_OwnerBT(nullptr), m_ParentNode(nullptr), m_ChildNode(nullptr), m_AttachedNode(nullptr) { m_NodeName = L"NoName"; }
    virtual ~BTNode();
};

// ========================= ��Ʈ ��� =========================
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
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
    
    void AddChildNode(BTNode* pNode);
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_RunningNode(nullptr) { m_BlackBoard = new BB(); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }
};

// ========================= ������ ��� =========================
#pragma region Composite Node
// �б��� ��Ʈ�� �ش� �бⰡ ����Ǵ� ����� �⺻ ��Ģ
class Composite_Node : public BTNode
{
public:
    virtual BT_STATUS Run();
    virtual BTNode* NodeAttach(BTNode* pNode);
    virtual BTNode* AddChildNode(BTNode* pNode)
    {
        m_ChildNodes.emplace_back(pNode);
        return pNode;
    }

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) {}
    virtual ~Composite_Node() {}
};

class Selector : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    Selector() { SetNodeName(L"NewSelectorNode"); }
    virtual ~Selector() {}
};

class Sequence : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    Sequence() { SetNodeName(L"NewSequenceNode"); }
};

class RandomSelector : public Composite_Node
{
public:
    virtual BT_STATUS Run();

public:
    RandomSelector() { SetNodeName(L"NewRandomSelectorNode"); }
};

#pragma endregion

// ========================= ���ڷ����� ��� =========================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
private:


public:
    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    virtual ~Decorator_Node() {}
    
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
    virtual ~Compare_BBEntries() {}
};

#pragma endregion

// ========================= ���� ��� =========================
#pragma region Service Node
// ������ ��忡 ����ġ, �������� Ȯ�� �� ������Ʈ�� ���.
class Service_Node : public BTNode
{
private:


public:
    Service_Node() : BTNode(NODETYPE::SERVICE) {}
    virtual ~Service_Node() {}

};
#pragma endregion

// ========================= �׽�ũ(����) ��� =========================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    virtual BT_STATUS Run();
    virtual BTNode* NodeAttach(BTNode* pNode);

public:
    Task_Node() : BTNode(NODETYPE::TASK) { SetNodeName(L"NewTaskNode"); }
    virtual ~Task_Node() {}
};
#pragma endregion


// ========================= �ൿƮ�� ������Ʈ =========================
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
    BTNode* AddChildNode(BTNode* ParentNode, NODETYPE eType);
    BTNode* NodeAttach(BTNode* TargetNode, NODETYPE eType);
    BTNode* GetRootNode() { return m_RootNode; }

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


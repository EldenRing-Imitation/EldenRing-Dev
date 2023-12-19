#pragma once
#include "CComponent.h"

class CBehaviorTree;
class Decorator_Node;
class BTNode;

enum class NODETYPE
{
    ROOT,       // ��Ʈ���
    COMPOSITE,  // �귣ġ���   : ���� �ڽ�
    DECORATOR,  // ���ǳ��     : ���� �ڽ�
    TASK        // ������     : �ڽ� �Ұ�
};

enum class BT_STATUS
{
    NONE,
    SUCCESS,
    FAILURE,
    RUNNING,
};

enum class BBType
{
    INT,
    FLOAT,
    BOOL,
    GAMEOBJECT,
    STRING,
};

struct BBkey
{
    wstring key;
    BBType  type;
};

#define BT_SUCCESS BT_STATUS::SUCCESS
#define BT_FAILURE BT_STATUS::FAILURE
#define BT_RUNNING BT_STATUS::RUNNING

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
protected:
    wstring         m_NodeName;     // ��� �̸�
    NODETYPE        m_NodeType;     // ��� Ÿ�� : ROOT,COMPOSITE,DECORATOR,TASK
    Root_Node*      m_RootNode;     // Root_Node
    int             m_NodeFlag;     // FlagType
    
    BTNode*         m_Parent;       // �θ���
    list<BTNode*>   m_Child;        // �ڽĳ�� ����Ʈ
    
    UINT            m_ChildCnt;     // �ڽĳ�� ��

public:
    virtual BT_STATUS Run() = 0;
    
    // ========= �θ� ��� =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }

    // ========= �ڽ� ��� =========
    virtual void AddChild(BTNode* ChildNode) {};
    UINT GetChildCnt() { return m_ChildCnt; }

    // ========= ��� ���� =========
    const wstring& GetNodeName() { return m_NodeName; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    
    NODETYPE GetNodeType() { return m_NodeType; }
    
    Root_Node* GetRootNode() { return m_RootNode; }
    void SetRootNode(Root_Node* _RootNode) { m_RootNode = _RootNode; }

    const int GetNodeFlag() { return m_NodeFlag; }
    void SetNodeFlag(UINT _flag) { m_NodeFlag = _flag; }

    void DisconnectFromParent();
    bool IsAncestor(BTNode* _Node);

public:
    BTNode(NODETYPE eType) {}
    virtual ~BTNode();
};

// ========================= ��Ʈ ��� =========================
class Root_Node : public BTNode
{
private:
    BB* m_BlackBoard;
    BTNode* m_RunningNode;

public:
    virtual BT_STATUS Run() override;
    
    virtual void AddChild(BTNode* ChildNode);

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
    enum CompositNodeFlag
    {
        CompositeNodeFlag_SEQUENCE,
        CompositeNodeFlag_SELECTOR,
        CompositeNodeFlag_RANDOM_SELECTOR,
    };

public:
    virtual BT_STATUS Run();
    virtual void AddChild(BTNode* ChildNode) {};

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) {}
    virtual ~Composite_Node() {}
};

#pragma endregion

// ========================= ���ڷ����� ��� =========================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
public:
    enum DecoratorNodeFlag
    {
        DecoratorNodeFlag_BLACKBOARD,           // Ư�� ������ Key�� ���� �����Ǿ��ִ��� Ȯ��
        DecoratorNodeFlag_COMPARE_BBENTRIES,    // �� ������ Ű ���� ���Ͽ� ����� ���� ���� �Ǵ�
    };

public:
    virtual BT_STATUS Run();

    Decorator_Node() : BTNode(NODETYPE::DECORATOR) {}
    virtual ~Decorator_Node() {}
    
};
#pragma endregion

// ========================= �׽�ũ(����) ��� =========================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    enum TaskNodeFlag
    {
        TaskNodeFlag_PLAY_ANIMATION,         // �ִϸ��̼� ���
        TaskNodeFlag_PLAY_SOUND,             // ���� ���
        TaskNodeFlag_WAIT,                   // ���ð� ���� : ������ �� �����Ͽ� ���
        TaskNodeFlag_WAIT_BLACKBOARD_TIME,   // ���ð� ���� : �����忡 ������ �� ���
    };
    
    virtual BT_STATUS Run();

public:
    Task_Node() : BTNode(NODETYPE::TASK) {}
    virtual ~Task_Node() {}
};
#pragma endregion

BTNode* CreateBTNode(NODETYPE _type, int _flag = -1)
{
    BTNode* NewNode = nullptr;

    switch (_type)
    {
    case NODETYPE::ROOT:
        NewNode = new Root_Node;
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

    // Flag ������ ����
    if (0 <= _flag)
    {
        NewNode->SetNodeFlag(_flag);
    }

    return NewNode;
}

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
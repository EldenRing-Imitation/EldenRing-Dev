#pragma once
#include "CComponent.h"

#define BT_SUCCESS BTNode::BT_STATUS::SUCCESS
#define BT_FAILURE BTNode::BT_STATUS::FAILURE
#define BT_RUNNING BTNode::BT_STATUS::RUNNING
#define BT_ROOT BTNode::NODETYPE::ROOT
#define BT_COMPOSITE BTNode::NODETYPE::COMPOSITE
#define BT_DECORATOR BTNode::NODETYPE::DECORATOR
#define BT_TASK BTNode::NODETYPE::TASK

class CBehaviorTree;
class Decorator_Node;
class BTNode;

// ========================= ������ Ŭ���� =========================
class BB
{
public:
    enum class BBType
    {
        BOOL,       // boolen
        INT,        // int
        FLOAT,      // float
        Vec2,       // float2
        Vec3,       // float3
        Vec4,       // float4
        STRING,     // string
        GAMEOBJECT, // Object
    };

    struct BBkey
    {
        wstring key;
        BBType  type;
    };

private:
    unordered_map<wstring, void*> m_ExternedItem;   // �ܺ� ������
    unordered_map<wstring, void*> m_CreatedItem;    // ��ü ���� ������
    

    // ExternedItem �� Load ������ ������ �� �ֵ���
    // GameObject���� ������ �Լ���, System���� ������ �� �ִ� �Լ���
    // ���ڸ� �����Ѵ�.

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
        *NewItem = Data;

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
public:
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

protected:
    wstring         m_NodeName;     // ��� �̸�
    NODETYPE        m_NodeType;     // ��� Ÿ�� : ROOT,COMPOSITE,DECORATOR,TASK
    BTNode*         m_RootNode;     // Root_Node
    UINT            m_NodeFlag;     // FlagType
    
    BTNode*         m_Parent;       // �θ���
    list<BTNode*>   m_Child;        // �ڽĳ�� ����Ʈ
    
    UINT            m_ChildCnt;     // �ڽĳ�� ��

public:
    virtual BT_STATUS Run() { return BT_STATUS::NONE; }

    // ========= ��� ���� =========
   
    void SwapFront();
    void SwapBack();

    BTNode* DisconnectFromParent()
    {
        if (!m_Parent)
            return nullptr;

        BTNode* BeforeParentNode = m_Parent;

        list<BTNode*>::iterator iter = m_Parent->m_Child.begin();

        for (; iter != m_Parent->m_Child.end(); ++iter)
        {
            if (this == *iter)
            {
                m_Parent->m_Child.erase(iter);
                m_Parent->m_ChildCnt--;
                m_Parent = nullptr;

                return BeforeParentNode;
            }
        }

        assert(nullptr);

        return nullptr;
    }

    bool IsAncestorNode(BTNode* _Node)
    {
        BTNode* pParent = m_Parent;
        while (pParent)
        {
            if (pParent == _Node)
            {
                return true;
            }
            pParent = pParent->m_Parent;
        }

        return false;
    }

    // ========= �θ� ��� =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }


    // ========= �ڽ� ��� =========
    BTNode* AddChild(BTNode* ChildNode)
    {
        // Task Node��� �ڽ��� ���� �� ����.
        if (m_NodeType == NODETYPE::TASK)
        {
            return ChildNode;
            assert(nullptr);
        }

        // �ڽ��� 1�� �̻� ���� ���
        if (0 < m_ChildCnt)
        {
            // �����ڽ��� ������ ���
            if (m_NodeType == NODETYPE::ROOT
                || m_NodeType == NODETYPE::DECORATOR)
            {
                // ���� �ڽİ� �θ� �������
                BTNode* BeforeChild = m_Child.front();
                BeforeChild->DisconnectFromParent();

                ChildNode->AddChild(BeforeChild);

                ChildNode->SetRootNode(m_RootNode);

                m_Child.emplace_back(ChildNode);
                ChildNode->SetParentNode(this);
                m_ChildCnt++;

                return ChildNode;
            }
        }

        ChildNode->DisconnectFromParent();

        m_Child.emplace_back(ChildNode);
        ChildNode->SetParentNode(this);
        ChildNode->SetRootNode(m_RootNode);
        m_ChildCnt++;

        return ChildNode;
    }

    UINT GetChildCnt() { return m_ChildCnt; }
    list<BTNode*> GetChild() { return m_Child; }


    // ========= ��� ���� =========
    const wstring& GetNodeName() { return m_NodeName; }
    void SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    
    NODETYPE GetNodeType() { return m_NodeType; }
    
    BTNode* GetRootNode() { return m_RootNode; }
    void SetRootNode(BTNode* _RootNode) { m_RootNode = _RootNode; }

    const int GetNodeFlag() { return m_NodeFlag; }
    void SetNodeFlag(UINT _flag) { m_NodeFlag = _flag; }

    

public:
    BTNode(NODETYPE eType);
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
    
    // ========= �� ���� =========
    template<typename T>
    T* FindItem(const wstring& key) { return m_BlackBoard->FindItem(key); }

    // �ܺ� ������ �߰�
    template<typename T>
    T* AddItem(const wstring& key, T* Item) { return m_BlackBoard->AddItem(key, Item); }

    // �ű� ������ ���� �� ������ �Է�
    template<typename T>
    T* AddItem(const wstring& key, T Data) { return m_BlackBoard->AddItem(key, Data); }

    // �ű� ������ ���� (�޸� ������ ����)
    template<typename T>
    T* AddItem(const wstring& key) { return m_BlackBoard->AddItem(key); }

    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_RunningNode(nullptr) { m_BlackBoard = new BB(); SetNodeName(L"NewRoot");
    }
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

public:
    Composite_Node() : BTNode(NODETYPE::COMPOSITE) { SetNodeName(L"NewComposite"); }
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

    Decorator_Node() : BTNode(NODETYPE::DECORATOR) { SetNodeName(L"NewDecorator"); }
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
    Task_Node() : BTNode(NODETYPE::TASK) { SetNodeName(L"NewTask"); }
    virtual ~Task_Node() {}
};
#pragma endregion

// ========================= �ൿƮ�� ������Ʈ =========================
class CBehaviorTree :
    public CComponent
{
private:
    Root_Node* m_RootNode;

public:
    virtual void begin() {}
    virtual void tick();
    virtual void finaltick() {}

public:
    BTNode* GetRootNode() { return m_RootNode; }

public:
    Root_Node* SetRootNode(Root_Node* _Root)
    { 
        m_RootNode = _Root; 

        return m_RootNode;
    }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};
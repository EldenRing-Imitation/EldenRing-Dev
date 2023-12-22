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

struct tBBKey
{
    enum eDataType
    {
        UNKNOWN,
        INT,
        FLOAT,
        VECTOR2,
        VECTOR3,
        VECTOR4,
    };

    enum eDataSlot
    {
        EXTERNED,
        CREATED,
    };

    string Key;
    eDataType Type = eDataType::UNKNOWN;
    eDataSlot Slot = eDataSlot::CREATED;

    void GetBBKeyInfoString(string& _Type, string& _Slot)
    {
        _Type = GetType();
        _Slot = GetSlot();
    }

    const char* GetType()
    {
        const char* type = nullptr;

        switch (Type)
        {
        case tBBKey::UNKNOWN:
            type = "Unknown";
            break;
        case tBBKey::INT:
            type = "int";
            break;
        case tBBKey::FLOAT:
            type = "float";
            break;
        case tBBKey::VECTOR2:
            type = "Vec2";
            break;
        case tBBKey::VECTOR3:
            type = "Vec3";
            break;
        case tBBKey::VECTOR4:
            type = "Vec4";
            break;
        }

        return type;
    }

    const char* GetSlot()
    {
        const char* slot= nullptr;

        switch (Slot)
        {
        case tBBKey::EXTERNED:
            slot = "Externed";
            break;
        case tBBKey::CREATED:
            slot = "Created";
            break;
        default:
            break;
        }
        return slot;
    }
    
};

// ========================= ������ Ŭ���� =========================
class BB
{
private:
    unordered_map<string, void*> m_ExternedItem;   // �ܺ� ������
    unordered_map<string, void*> m_CreatedItem;    // ��ü ���� ������
    list<tBBKey> m_DataKeyList;                    // �������� ������ ����Ʈ

    // ExternedItem �� Load ������ ������ �� �ֵ���
    // GameObject���� ������ �Լ���, System���� ������ �� �ִ� �Լ���
    // ���ڸ� �����Ѵ�.

private:
    template<typename T>
    void AddBBKey(string _key, bool _IsExterned)
    {
        tBBKey tmp;
        tmp.Key = _key;

        string type = typeid(T).name();

        if (type == typeid(int).name())
        {
            tmp.Type = tBBKey::eDataType::INT;
        }
        else if (type == typeid(float).name())
        {
            tmp.Type = tBBKey::eDataType::FLOAT;
        }
        else if (type == typeid(Vector2).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR2;
        }
        else if (type == typeid(Vector3).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR3;
        }
        else if (type == typeid(Vector4).name())
        {
            tmp.Type = tBBKey::eDataType::VECTOR4;
        }
        else
        {
            tmp.Type = tBBKey::eDataType::UNKNOWN;
        }

        if (_IsExterned)
        {
            tmp.Slot = tBBKey::eDataSlot::EXTERNED;
            
        }
        else
        {
            tmp.Slot = tBBKey::eDataSlot::CREATED;
        }
        
        m_DataKeyList.push_back(tmp);
    }

public:
    // �ܺο��� ������� �޸��ּҸ� �����忡 �����´�.
    template<typename T>
    T* AddItem(const string& key, T* ItemAdress)
    {
        unordered_map<string, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        m_ExternedItem.insert(make_pair(key, ItemAdress));
        iter = m_ExternedItem.find(key);

        AddBBKey<T>(key, true);

        return (T*)(iter->second);
    }

    // ������ ���ο� ���� �޸𸮸� �Ҵ��Ͽ� �����͸� �Է��Ѵ�.
    template<typename T>
    T* AddItem(const string& key, T Data)
    {
        unordered_map<string, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = Data;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        AddBBKey<T>(key, false);

        return (T*)(iter->second);
    }

    template<typename T>
    T* AddItem(const string& key)
    {
        unordered_map<string, void*>::iterator iter
            = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        T* NewItem = new T();
        *NewItem = 0;

        m_CreatedItem.insert(make_pair(key, NewItem));
        iter = m_CreatedItem.find(key);

        AddBBKey<T>(key, false);

        return (T*)(iter->second);
    }

    template<typename T>
    T* FindItem(const string& key)
    {
        unordered_map<string, void*>::iterator iter
            = m_ExternedItem.find(key);

        if (iter != m_ExternedItem.end())
            return (T*)(iter->second);

        iter = m_CreatedItem.find(key);

        if (iter != m_CreatedItem.end())
            return (T*)(iter->second);

        return nullptr;
    }

    void GetvalueStr(tBBKey _key, string& _Dest)
    {
        char tmp[100] = {};

        switch (_key.Type)
        {
        case tBBKey::UNKNOWN:
            _Dest = "--";
            break;
        case tBBKey::INT:
        {
            int* data = FindItem<int>(_key.Key);
            sprintf_s(tmp, "%d", *data);
            break;
        }
        case tBBKey::FLOAT:
        {
            float* data = FindItem<float>(_key.Key);
            sprintf_s(tmp, "%.3f", *data);
            break;
        }
        case tBBKey::VECTOR2:
        {
            Vector2* data = FindItem<Vector2>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f}", (*data).x, (*data).y);
            break;
        }
        case tBBKey::VECTOR3:
        {
            Vector3* data = FindItem<Vector3>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f, %.3f}", (*data).x, (*data).y, (*data).z);
            break;
        }
        case tBBKey::VECTOR4:
        {
            Vector4* data = FindItem<Vector4>(_key.Key);
            sprintf_s(tmp, "{ %.3f, %.3f, %.3f, %.3f}", (*data).x, (*data).y, (*data).z, (*data).w);
        }
        break;
        }

        _Dest = tmp;
    }

    void ClearCreatedItem()
    {
        unordered_map<string, void*>::iterator iter = m_CreatedItem.begin();
        
        while (iter != m_CreatedItem.end())
        {
            delete iter->second;
            iter = m_CreatedItem.erase(iter);
        }
    }

    const list<tBBKey>& GetKeyList() { return m_DataKeyList; }

public:
    BB() { }
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

                ChildNode->AddChild(BeforeChild);

                
                ChildNode->DisconnectFromParent();
                ChildNode->SetRootNode(m_RootNode);
                m_Child.emplace_back(ChildNode);
                ChildNode->SetParentNode(this);
                m_ChildCnt++;

                return ChildNode;
            }
        }
        
        ChildNode->DisconnectFromParent();
        ChildNode->SetRootNode(m_RootNode);
        m_Child.emplace_back(ChildNode);
        ChildNode->SetParentNode(this);
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

    int GetNodeFlag() { return m_NodeFlag; }
    void SetNodeFlag(UINT _flag) { m_NodeFlag = _flag; }
    virtual const char** GetFlagList() { return nullptr; }
    virtual UINT GetFlagCnt() { return 0; }

    

public:
    BTNode(NODETYPE eType);
    virtual ~BTNode();

    
};

// ========================= ��Ʈ ��� =========================
class Root_Node : public BTNode
{
private:
    CGameObject* m_OwnerObj;
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
    Root_Node() : BTNode(NODETYPE::ROOT), m_BlackBoard(nullptr), m_RunningNode(nullptr), m_OwnerObj(nullptr) { m_BlackBoard = new BB(); SetNodeName(L"NewRoot"); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }

    friend class CBehaviorTree;
    friend class Decorator_Node;
    friend class Task_Node;
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
        END,
    };

public:
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return CompositNodeFlag::END; }
    virtual const char** GetFlagList()
    { 
        static const char* CompositeFlags[] =
        {
            "SEQUENCE",
            "SELECTOR",
            "RANDOM_SELECTOR"
        };

        return CompositeFlags;
    }

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
        END,
    };

public:
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return DecoratorNodeFlag::END; }
    virtual const char** GetFlagList()
    {
        static const char* DecoratorFlags[] =
        {
            "BLACKBOARD",
            "COMPARE_BBENTRIES"
        };

        return DecoratorFlags;
    }

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
        END,
    };
    
    virtual BT_STATUS Run();
    virtual UINT GetFlagCnt() { return TaskNodeFlag::END; }
    virtual const char** GetFlagList()
    {
        static const char* TaskFlags[] =
        {
            "PLAY_ANIMATION",
            "PLAY_SOUND",
            "WAIT",
            "WAIT_BLACKBOARD_TIME"
        };

        return TaskFlags;
    }

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
        m_RootNode->m_OwnerObj = GetOwner();

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
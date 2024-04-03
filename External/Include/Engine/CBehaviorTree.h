#pragma once
#include "CComponent.h"

#define BT_SUCCESS BT_STATUS::SUCCESS
#define BT_FAILURE BT_STATUS::FAILURE
#define BT_RUNNING BT_STATUS::RUNNING
#define BT_INVALID BT_STATUS::INVALID

enum class BT_STATUS
{
    SUCCESS,	// ����
    FAILURE,	// ����
    RUNNING,	// ������
    INVALID		// ��ȿ���� ����
};

// ========================= ������ Ŭ���� =========================
class BB
{
public:
    struct tBBData
    {
        wstring         strKey;
        const wchar_t*  strDataType;
        DWORD_PTR       pDataPtr;
        wstring         strData;
        
        tBBData(wstring _key, const wchar_t* _Type, DWORD_PTR _Dataptr) : strKey(_key), strDataType(_Type), pDataPtr(_Dataptr) {}
        tBBData(wstring _key, const wchar_t* _Type, wstring _strData) : strKey(_key), strDataType(_Type), pDataPtr(0), strData(_strData) {}
    };

private:
    unordered_map<wstring, tBBData*> m_BBDataList;    // Total BB List
 
public:
    HRESULT AddBBData(const wstring& _BBKey, int _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, float _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, CGameObject* _ItemPtr);
    HRESULT AddBBData(const wstring& _BBKey, wstring _wstring);

    unordered_map<wstring, tBBData*> GetBBList() { return m_BBDataList; }

    HRESULT FindBBData(const wstring& _BBKey, int& _Dest);
    HRESULT FindBBData(const wstring& _BBKey, float& _Dest);
    HRESULT FindBBData(const wstring& _BBKey, CGameObject** _Dest);
    HRESULT FindBBData(const wstring& _BBKey, wstring& _Dest);

    HRESULT SetBBData(const wstring& _BBKey, int _Src);
    HRESULT SetBBData(const wstring& _BBKey, float _Src);
    HRESULT SetBBData(const wstring& _BBKey, CGameObject* _Src);
    HRESULT SetBBData(const wstring& _BBKey, const wstring& _Src);

    void DeleteBBData(const wstring& _BBKey);

    void ClearBBData()
    {
        unordered_map<wstring, tBBData*>::iterator iter = m_BBDataList.begin();;
        
        while (iter != m_BBDataList.end())
        {
            if (iter->second->strDataType == L"int")
            {
                delete (int*)iter->second->pDataPtr;
            }

            if(iter->second->strDataType == L"float")
            {
                delete (float*)iter->second->pDataPtr;
            }

            delete iter->second;
            iter = m_BBDataList.erase(iter);
        }
    }

public:
    BB() { }
    ~BB() { ClearBBData(); }
};

class Root_Node;

// ========================= �⺻ ��� =========================
class BTNode
{
protected:
    wstring         m_NodeName;     // ��� �̸�
    BTNode*         m_RootNode;     // Root_Node
    
    BTNode*         m_Parent;       // �θ���
    list<BTNode*>   m_Child;        // �ڽĳ�� ����Ʈ
    
    UINT            m_ChildCnt;     // �ڽĳ�� ��

    CGameObject*    m_pOwner;

public:
    virtual BT_STATUS Run() = 0;

    // ========= �θ� ��� =========
    BTNode* GetParentNode() { return m_Parent; }
    void SetParentNode(BTNode* _ParentNode) { m_Parent = _ParentNode; }

    // ========= �ڽ� ��� =========
    template<typename T>
    T* AddChild()
    {
        T* ChildNode = new T();

        // �����ϴ� ��ü�� BTNode ����� �ƴϸ� �����Ұ�
        if (nullptr == dynamic_cast<BTNode*>(ChildNode))
        {
            delete ChildNode;
            return nullptr;
        }
                
        ChildNode->SetRootNode(m_RootNode);     // ��Ʈ ��� ����
        m_Child.emplace_back(ChildNode);        // �ڽ� ���� �߰�
        ChildNode->SetParentNode(this);         // �θ��� ����
        ChildNode->m_pOwner = m_pOwner;         // ���ӿ�����Ʈ ���
        m_ChildCnt++;                           // �ڽİ��� �߰�

        return ChildNode;
    }

    UINT GetChildCnt() { return m_ChildCnt; }
    list<BTNode*> GetChild() { return m_Child; }

    // ========= ��� ���� =========
    const wstring&          GetNodeName() { return m_NodeName; }
    void                    SetNodeName(const wstring& NodeName) { m_NodeName = NodeName; }
    
    BTNode*                 GetRootNode() { return m_RootNode; }
    void                    SetRootNode(BTNode* _RootNode) { m_RootNode = _RootNode; }

    BB*                     GetBlackBoard();

    CGameObject*            GetOwner() { return m_pOwner; }

public:
    BTNode();
    virtual ~BTNode();

    friend class Root_Node;
};

// ========================= ��Ʈ ��� =========================
class Root_Node : public BTNode
{
protected:
    BB*                 m_BlackBoard;
    BTNode*             m_RunningNode;
    

public:
    virtual BT_STATUS Run() override;
    void SetRunningNode(BTNode* pNode) { m_RunningNode = pNode; }
    void SetOwner(CGameObject* _Owner) { m_pOwner = _Owner; }

    // ========= �ڽ� ��� =========
    template<typename T>
    T* AddChild()
    {
        T* ChildNode = new T();

        // �����ϴ� ��ü�� BTNode ����� �ƴϸ� �����Ұ�
        if (nullptr == dynamic_cast<BTNode*>(ChildNode))
        {
            delete ChildNode;
            return nullptr;
        }

        ChildNode->SetRootNode((BTNode*)this);     // ��Ʈ ��� ����
        m_Child.emplace_back(ChildNode);        // �ڽ� ���� �߰�
        ChildNode->SetParentNode(this);         // �θ��� ����
        ChildNode->m_pOwner = m_pOwner;         // ���ӿ�����Ʈ ���
        m_ChildCnt++;                           // �ڽİ��� �߰�

        return ChildNode;
    }

    void ClearRunningNode() { if (m_RunningNode != nullptr) m_RunningNode = nullptr; }

    // ========= �� ���� =========
    BB* GetBlackBoard() { return m_BlackBoard; }

public:
    Root_Node() : m_BlackBoard(nullptr), m_RunningNode(nullptr)  { m_BlackBoard = new BB(); }
    virtual ~Root_Node() { DELETE_UNVAILUBLE(m_BlackBoard); m_RunningNode = nullptr; }

    friend class CBehaviorTree;
};

// ==================================================================
// ========================= [ ������ ��� ] =========================
// ==================================================================
#pragma region Composite Node
// �б��� ��Ʈ�� �ش� �бⰡ ����Ǵ� ����� �⺻ ��Ģ
class Composite_Node : public BTNode
{
public:
    Composite_Node() {}
    virtual ~Composite_Node() {}
};

// ������ ���
class Sequence_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

// ������ ���
class Selector_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

// ���������� ���
class RandSelector_Node : public Composite_Node
{
public:
    virtual BT_STATUS Run() override;
};

#pragma endregion

// ==================================================================
// ========================= [ ���ڷ����� ��� ] =====================
// ==================================================================
#pragma region Decorator Node
class Decorator_Node : public BTNode
{
public:
    virtual BT_STATUS Run() = 0;

    Decorator_Node() {}
    virtual ~Decorator_Node() {}
    
};
#pragma endregion


// ===================================================================
// ========================= [ �׽�ũ(����) ��� ] ====================
// ===================================================================
#pragma region Task Node
class Task_Node : public BTNode
{
public:
    virtual BT_STATUS Run() = 0;

public:
    Task_Node() {}
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
    Root_Node* GetRootNode() { return m_RootNode; }
    void SetOwner(CGameObject* _Owner) { m_RootNode->SetOwner(_Owner); }

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    
    CLONE(CBehaviorTree);

public:
    CBehaviorTree();
    ~CBehaviorTree();

    friend class BTNode;
};
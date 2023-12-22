#pragma once
#include "UI.h"

// =========
// TreeNode
// =========
class TreeUI;
class TreeNode
{
private:
    TreeUI*             m_Owner;        // ��带 �����ϰ� �ִ� Ʈ��
    TreeNode*           m_ParentNode;   // �θ���
    vector<TreeNode*>   m_vecChildNode; // ����� �ڽ� ���
    UINT                m_CurGroupIdx;  // ���Ϻθ��ڽ� ����� ����
    UINT                m_ChildNodeSize;// �ڽĳ�� ����
    UINT                m_NodeColorNum; // ���۾� ����
    UINT                m_CustomFlags;  // ����� �÷���
        
    string              m_strName;      // ����� ��� �̸�
    UINT                m_ID;           // ����� ���� ID

    DWORD_PTR           m_Data;         // ��忡 ����� ������


    bool                m_CategoryNode; // �׸� ��ǥ ���
    bool                m_Hilight;      // ��� ���̶���Ʈ ó��
    bool                m_ColorChange;  // ���󺯰濩��
    bool                m_bHovered;     // ȣ���� ����


public:
    void SetCategoryNode(bool _category)
    {
        m_CategoryNode = _category;
    }

public:
    const string& GetName() { return m_strName; }
    DWORD_PTR GetData() { return m_Data; }
    UINT GetChildNodeSize() { return m_ChildNodeSize; }
    
    void SetFlags(UINT _flags) { m_CustomFlags = _flags; }
    void SetGroupIdx(UINT _idx) { m_CurGroupIdx = _idx; }
    void SetChildNodeSize(UINT _i) { m_ChildNodeSize = _i; }
    void SetNodeColor(UINT _colorNum) { m_NodeColorNum = _colorNum; m_ColorChange = true; }

private:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};



// ======
// TreeUI
// ======
class TreeUI :
    public UI
{
private:
    TreeNode*       m_RootNode; // Ʈ���� �����ϰ� �ִ� ��� �� ��Ʈ ���
    UINT            g_NextId;       // �����Ǵ� ���ڿ� �ٿ��� ���� ����
    UINT            m_NodeFlags;    // ��� �÷���

    bool            m_bShowRoot;
    bool            m_ArrowBtn;     // ȭ��ǥ ��ư ����
    bool            m_GroupIdx;     // �׷� ���� ǥ�� ����

    TreeNode* m_SelectedNode;
    TreeNode* m_LbtDownNode;

    TreeNode* m_DragNode;
    TreeNode* m_DropNode;
    TreeNode* m_SwapFrontNode;
    TreeNode* m_SwapBackNode;

    DWORD_PTR       m_dwPrevSelected;

    UI* m_SelectInst;
    UI_DELEGATE_1   m_SelectFunc;

    UI* m_DragDropInst;
    UI_DELEGATE_2   m_DragDropFunc;

    UI* m_SwapFrontInst;
    UI_DELEGATE_1   m_SwapFrontFunc;

    UI* m_SwapBackInst;
    UI_DELEGATE_1   m_SwapBackFunc;

    string          m_strDragDropID;


public:
    virtual int render_update() override;

public:
    void Clear();
    TreeNode* AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent = nullptr);
    void ShowRoot(bool _Show) { m_bShowRoot = _Show; }
    void ShowArrowBtn(bool _Show) { m_ArrowBtn = _Show; }
    void ShowGroupIdx(bool _Show) { m_GroupIdx = _Show; }
    void SetFlags(UINT _flags) { m_NodeFlags = _flags; }

    void AddDynamic_Select(UI* _UI, UI_DELEGATE_1 _MemFunc)
    {
        m_SelectInst = _UI;
        m_SelectFunc = _MemFunc;
    }

    void AddDynamic_DragDrop(UI* _UI, UI_DELEGATE_2 _MemFunc)
    {
        m_DragDropInst = _UI;
        m_DragDropFunc = _MemFunc;
    }

    void AddDynamic_SwapFront(UI* _UI, UI_DELEGATE_1 _MemFunc)
    {
        m_SwapFrontInst = _UI;
        m_SwapFrontFunc = _MemFunc;
    }

    void AddDynamic_SwapBack(UI* _UI, UI_DELEGATE_1 _MemFunc)
    {
        m_SwapBackInst = _UI;
        m_SwapBackFunc = _MemFunc;
    }

    void SetDragDropID(const string& _strID) { m_strDragDropID = _strID; }


private:
    void SetSelectedNode(TreeNode* _Node);   
    void SetDragNode(TreeNode* _Node);
    void SetDropNode(TreeNode* _Node);
    void SetSwapFrontNode(TreeNode* _Node);
    void SetSwapBackNode(TreeNode* _Node);

public:
    TreeNode* GetSelectedNode() { return m_SelectedNode; }
    bool GetSelectedNode(DWORD_PTR _Data);  // �Էµ� �����Ϳ� ������ �����͸� �����ϰ� �ִ� ��带 ���û��·� ����


public:
    TreeUI();
    ~TreeUI();

    friend class TreeNode;
};


#include "pch.h"
#include "TreeUI.h"

// ========
// TreeNode
// ========
TreeNode::TreeNode()
    : m_Owner(nullptr)
    , m_ParentNode(nullptr)
    , m_CurGroupIdx(0)
    , m_ChildNodeSize(0)
    , m_NodeColorNum(0)
    , m_CustomFlags(0)
    , m_ID(0)
    , m_Data(0)
    , m_CategoryNode(false)
    , m_Hilight(false)    
    , m_ColorChange(false)
    , m_bHovered(false)
{
}

TreeNode::~TreeNode()
{
    Safe_Del_Vec(m_vecChildNode);
}

void TreeNode::render_update()
{
    // FinalName �����
    string strFinalName = m_strName;
    strFinalName += "##";
    
    char szBuff[100] = {};
    _itoa_s(m_ID, szBuff, 10);
    strFinalName += szBuff;

    // Flag üũ
    UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

    // �ڽ� ��尡 ������ Lear �÷��׸� �����Ѵ�(ȭ��ǥ ����)
    if (m_vecChildNode.empty())    
        flag |= ImGuiTreeNodeFlags_Leaf;
    
    // Ŭ�� �Ǿ��ų�, �׸� ��ǥ ����� ��� Selected �÷��׷� ���̶���Ʈ�� �ش�.
    if(m_Hilight || m_CategoryNode)
        flag |= ImGuiTreeNodeFlags_Selected;

    flag |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    flag |= m_CustomFlags;
    
    if (m_Owner->m_GroupIdx)
    {
        ImGui::Text("%d", m_CurGroupIdx);
        ImGui::SameLine();
    }
    
    // ��� ���� �̵�
    if (m_Owner->m_ArrowBtn)
    {
        float FrmHeight = ImGui::GetFrameHeight();
        ImVec2 BtnSize = { FrmHeight,FrmHeight };

        // �θ��尡 �ְ� ���ϰ��� ��� ���� 2�� �̻��� ���
        if (m_ParentNode && 1 < m_ParentNode->m_vecChildNode.size())
        {

            if (m_CurGroupIdx == m_ParentNode->m_vecChildNode.size())
            {
                ImGui::InvisibleButton("NONE", BtnSize);
            }
            else
            {
                string UpBtn = "u##";
                UpBtn += szBuff;
                if (ImGui::ArrowButtonSz(UpBtn.c_str(), ImGuiDir_Down, BtnSize))
                {
                    if (m_Owner->m_SwapBackInst && m_Owner->m_SwapBackFunc)
                        m_Owner->SetSwapBackNode(this);
                }
            }

            if (m_CurGroupIdx > 1)
            {
                ImGui::SameLine();
                string DownBtn = "d##";
                DownBtn += szBuff;
                if (ImGui::ArrowButtonSz(DownBtn.c_str(), ImGuiDir_Up, BtnSize))
                {
                    if (m_Owner->m_SwapFrontInst && m_Owner->m_SwapFrontFunc)
                        m_Owner->SetSwapFrontNode(this);
                }
            }
            else
            {
                ImGui::SameLine();
                ImGui::InvisibleButton("NONE", BtnSize);
            }
        }
        ImGui::SameLine();
    }

    // ��� ���� ����
    int ChangCnt = 0;
    if (m_ColorChange)
    {
        if (!m_Hilight && !m_bHovered)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(m_NodeColorNum / 7.0f, 0.5f, 0.9f));
            ChangCnt += 1;
        }

        ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(m_NodeColorNum / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(m_NodeColorNum / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(m_NodeColorNum / 7.0f, 0.8f, 0.8f));
        ChangCnt += 3;
    }


    if (ImGui::TreeNodeEx(strFinalName.c_str(), flag))
    {
        if (ImGui::IsItemHovered())
            m_bHovered = true;
        else
            m_bHovered = false;
        

        // ��� ������ ����Ǿ����� ����ó��
        if (0 < ChangCnt)
        {
            ImGui::PopStyleColor(ChangCnt);
            ChangCnt = 0;
        }
    
        
        // �ش� ��忡 ���콺 ��Ŭ���� �߻��ϸ� ���ó��� ���� �ش�.
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {            
            m_Owner->m_LbtDownNode = this;
        }
        else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_Owner->SetSelectedNode(this);
        }
        // �Ǵ�, Ʈ�� �籸�� ������ ���õ� ��忴�ٸ�, �ٽ� ���ó��� ó���Ѵ�.
        else if (0 != m_Owner->m_dwPrevSelected && m_Owner->m_dwPrevSelected == m_Data)
        {
            m_Owner->m_dwPrevSelected = 0;
            m_Owner->SetSelectedNode(this);
        }

        

        // �ش� ��� ������ �巡�� ��ŸƮ üũ
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
            ImGui::Text(m_strName.c_str());

            // Tree �� �巡�� ��� ���
            m_Owner->SetDragNode(this);

            ImGui::EndDragDropSource();
        }


        // �巡�� ���� ��, ����� �ĺ��� ���
        if (ImGui::BeginDragDropTarget())
        {
            // �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(m_Owner->m_strDragDropID.c_str());
            if (pPayLoad)
            {
                // DropNode �� Tree �� �˸�
                m_Owner->SetDropNode(this);
            }

            ImGui::EndDragDropTarget();
        }

        for (size_t i = 0; i < m_vecChildNode.size(); ++i)
        {
            m_vecChildNode[i]->render_update();
        }

        ImGui::TreePop();
    }
    
    // ��� ������ ����Ǿ����� ����ó�� : if���� ���� �ʾ� �������� �ʾ��� ��� ����ó��
    if (0 < ChangCnt)
    {
        ImGui::PopStyleColor(ChangCnt);
        ChangCnt = 0;
    }
}




// ================
//      TreeUI
// ================
TreeUI::TreeUI()
    : UI("##Tree")
    , m_RootNode(nullptr)
    , g_NextId(0)
    , m_NodeFlags(0)
    , m_bShowRoot(true)
    , m_ArrowBtn(false)
    , m_GroupIdx(false)
    , m_SelectedNode(nullptr)
    , m_LbtDownNode(nullptr)
    , m_DragNode(nullptr)
    , m_DropNode(nullptr)
    , m_SwapFrontNode(nullptr)
    , m_SwapBackNode(nullptr)
    , m_dwPrevSelected(0)
    , m_SelectInst(nullptr)
    , m_SelectFunc(nullptr)    
    , m_DragDropInst(nullptr)
    , m_DragDropFunc(nullptr)
    , m_SwapFrontInst(nullptr)
    , m_SwapFrontFunc(nullptr)
    , m_SwapBackInst(nullptr)
    , m_SwapBackFunc(nullptr)
{   
    m_strDragDropID = "PayLoad";
}

TreeUI::~TreeUI()
{
    if (nullptr != m_RootNode)
        delete m_RootNode;
}

int TreeUI::render_update()
{
    if (nullptr != m_RootNode)
    {
        if (m_bShowRoot)
        {
            m_RootNode->render_update();
        }
        else
        {
            for (size_t i = 0; i < m_RootNode->m_vecChildNode.size(); ++i)
            {
                m_RootNode->m_vecChildNode[i]->render_update();
            }
        }
    }

    // Drag Drop ��� �Ѵ� �ִ� ���
    if ( (m_DragNode && m_DropNode) || (m_DragNode && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
    {
        if (m_DragDropInst && m_DragDropFunc)
        {
            (m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
        }
        
        m_DragNode = nullptr;
        m_DropNode = nullptr;
    }

    // SwapFront ����
    if (m_SwapFrontNode)
    {
        if (m_SwapFrontInst && m_SwapFrontFunc)
        {
            (m_SwapFrontInst->*m_SwapFrontFunc)((DWORD_PTR)m_SwapFrontNode);
        }

        m_SwapFrontNode = nullptr;
    }
    
    // SwapBack ����
    if (m_SwapBackNode)
    {
        if (m_SwapBackInst && m_SwapBackFunc)
        {
            (m_SwapBackInst->*m_SwapBackFunc)((DWORD_PTR)m_SwapBackNode);
        }

        m_SwapBackNode = nullptr;
    }

    return 0;
}

void TreeUI::Clear()
{
    // ������ ���õ� ��尡 ������
    if (nullptr != m_SelectedNode)
    {
        // �̸��� ����صд�.
        m_dwPrevSelected = m_SelectedNode->GetData();
    }

    if (nullptr != m_RootNode)
    {
        delete m_RootNode;
        m_RootNode = nullptr;
    }
}

TreeNode* TreeUI::AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent)
{    
    TreeNode* pNewNode = new TreeNode;

    pNewNode->m_Owner = this;
    pNewNode->m_strName = _strNodeName;
    pNewNode->m_Data = _Data;
    pNewNode->m_ID = g_NextId++;

    // ��Ʈ�� NULL �̴� ==> Ʈ���� ���� ������ ������
    if (nullptr == m_RootNode)
    {
        // ���� ������ �Է��ε�, �θ� ������ ���
        assert(!_pParent);

        m_RootNode = pNewNode;
    }

    // Ʈ���� ���� �����Ͱ� ���ʰ� �ƴϴ�.
    else
    {
        if (_pParent)
        {
            // ����� �θ�� ������ ��尡 �ش� Ʈ�� �Ҽ��� �ƴϴ�.
            if (_pParent->m_Owner != this)
                assert(nullptr);

            // ������ �θ��� �ڽ����� ����
            _pParent->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = _pParent;
        }

        // �θ�� ������ ��尡 ���°��, ��Ʈ ������ �ִ´�
        else
        {
            // ���� ������ ��带 ��Ʈ����� �ڽ����� ����
            m_RootNode->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = m_RootNode;            
        }
    }

    return pNewNode;
}


void TreeUI::SetSelectedNode(TreeNode* _Node)
{
    // ���콺�� ���� ���� �� ��尡 ��ġ�ؾ� ��
    if (m_LbtDownNode != _Node)
    {
        m_LbtDownNode = nullptr;
        return;
    }        

    if (m_SelectedNode)
        m_SelectedNode->m_Hilight = false;

    m_SelectedNode = _Node;
    m_LbtDownNode = nullptr;

    if (m_SelectedNode)
    {
        m_SelectedNode->m_Hilight = true;

        if (m_SelectInst && m_SelectFunc)
        {
            (m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
        }
    }
}

void TreeUI::SetDragNode(TreeNode* _Node)
{
    m_DragNode = _Node;
    m_DropNode = nullptr;
}

void TreeUI::SetDropNode(TreeNode* _Node)
{
    m_DropNode = _Node;
}

void TreeUI::SetSwapFrontNode(TreeNode* _Node)
{
    m_SwapFrontNode = _Node;
}

void TreeUI::SetSwapBackNode(TreeNode* _Node)
{
    m_SwapBackNode = _Node;
}

bool TreeUI::GetSelectedNode(DWORD_PTR _Data)
{
    static list<TreeNode*> queue;
    queue.clear();

    queue.push_back(m_RootNode);

    TreeNode* pCurNode = nullptr;
    while (!queue.empty())
    {
        pCurNode = queue.front();
        queue.pop_front();

        for (size_t i = 0; i < pCurNode->m_vecChildNode.size(); ++i)
        {
            queue.push_back(pCurNode->m_vecChildNode[i]);
        }

        if (pCurNode->GetData() == _Data)
        {
            SetSelectedNode(pCurNode);
            return true;
        }        
    }

    return false;
}

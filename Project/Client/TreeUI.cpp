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
    // FinalName 만들기
    string strFinalName = m_strName;
    strFinalName += "##";
    
    char szBuff[100] = {};
    _itoa_s(m_ID, szBuff, 10);
    strFinalName += szBuff;

    // Flag 체크
    UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

    // 자식 노드가 없으면 Lear 플래그를 설정한다(화살표 제거)
    if (m_vecChildNode.empty())    
        flag |= ImGuiTreeNodeFlags_Leaf;
    
    // 클릭 되었거나, 항목 대표 노드인 경우 Selected 플래그로 하이라이트를 준다.
    if(m_Hilight || m_CategoryNode)
        flag |= ImGuiTreeNodeFlags_Selected;

    flag |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    flag |= m_CustomFlags;
    
    if (m_Owner->m_GroupIdx)
    {
        ImGui::Text("%d", m_CurGroupIdx);
        ImGui::SameLine();
    }
    
    // 노드 상하 이동
    if (m_Owner->m_ArrowBtn)
    {
        float FrmHeight = ImGui::GetFrameHeight();
        ImVec2 BtnSize = { FrmHeight,FrmHeight };

        // 부모노드가 있고 동일계층 노드 수가 2개 이상인 경우
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

    // 노드 색상 변경
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
        

        // 노드 색상이 변경되었으면 해제처리
        if (0 < ChangCnt)
        {
            ImGui::PopStyleColor(ChangCnt);
            ChangCnt = 0;
        }
    
        
        // 해당 노드에 마우스 왼클릭이 발생하면 선택노드로 지정 준다.
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {            
            m_Owner->m_LbtDownNode = this;
        }
        else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            m_Owner->SetSelectedNode(this);
        }
        // 또는, 트리 재구성 이전에 선택된 노드였다면, 다시 선택노드로 처리한다.
        else if (0 != m_Owner->m_dwPrevSelected && m_Owner->m_dwPrevSelected == m_Data)
        {
            m_Owner->m_dwPrevSelected = 0;
            m_Owner->SetSelectedNode(this);
        }

        // 해당 노드에 마우스 오른쪽클릭이 발생하면 선택노드로 지정 준다.
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            m_Owner->m_RbtDownNode = this;
        }
        else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            m_Owner->SetRSelectedNode(this);
        }

        

        // 해당 노드 위에서 드래그 스타트 체크
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
            ImGui::Text(m_strName.c_str());

            // Tree 에 드래그 노드 등록
            m_Owner->SetDragNode(this);

            ImGui::EndDragDropSource();
        }


        // 드래그 시작 후, 드랍의 후보인 경우
        if (ImGui::BeginDragDropTarget())
        {
            // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
            const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(m_Owner->m_strDragDropID.c_str());
            if (pPayLoad)
            {
                // DropNode 를 Tree 에 알림
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
    
    // 노드 색상이 변경되었으면 해제처리 : if문에 들어가지 않아 해제되지 않았을 경우 예외처리
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

    // Drag Drop 노드 둘다 있는 경우
    if ( (m_DragNode && m_DropNode) || (m_DragNode && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
    {
        if (m_DragDropInst && m_DragDropFunc)
        {
            (m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
        }
        
        m_DragNode = nullptr;
        m_DropNode = nullptr;
    }

    // SwapFront 실행
    if (m_SwapFrontNode)
    {
        if (m_SwapFrontInst && m_SwapFrontFunc)
        {
            (m_SwapFrontInst->*m_SwapFrontFunc)((DWORD_PTR)m_SwapFrontNode);
        }

        m_SwapFrontNode = nullptr;
    }
    
    // SwapBack 실행
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
    // 이전에 선택된 노드가 있으면
    if (nullptr != m_SelectedNode)
    {
        // 이름을 기억해둔다.
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

    // 루트가 NULL 이다 ==> 트리에 들어온 최초의 데이터
    if (nullptr == m_RootNode)
    {
        // 최초 데이터 입력인데, 부모를 지정한 경우
        assert(!_pParent);

        m_RootNode = pNewNode;
    }

    // 트리에 들어온 데이터가 최초가 아니다.
    else
    {
        if (_pParent)
        {
            // 노드의 부모로 지정된 노드가 해당 트리 소속이 아니다.
            if (_pParent->m_Owner != this)
                assert(nullptr);

            // 지정된 부모의 자식으로 연결
            _pParent->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = _pParent;
        }

        // 부모로 지정된 노드가 없는경우, 루트 밑으로 넣는다
        else
        {
            // 새로 생성한 노드를 루트노드의 자식으로 연결
            m_RootNode->m_vecChildNode.push_back(pNewNode);
            pNewNode->m_ParentNode = m_RootNode;            
        }
    }

    return pNewNode;
}


void TreeUI::SetSelectedNode(TreeNode* _Node)
{
    // 마우스를 누른 노드와 뗀 노드가 일치해야 함
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

void TreeUI::SetRSelectedNode(TreeNode* _Node)
{
    // 마우스를 누른 노드와 뗀 노드가 일치해야 함
    if (m_RbtDownNode != _Node)
    {
        m_RbtDownNode = nullptr;
        return;
    }

    m_RSelectedNode = _Node;
    m_RbtDownNode = nullptr;

    if (m_RSelectedNode)
    {
        if (m_RSelectInst && m_RSelectFunc)
        {
            (m_RSelectInst->*m_RSelectFunc)((DWORD_PTR)m_RSelectedNode);
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

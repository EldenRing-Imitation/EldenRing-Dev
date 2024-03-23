#include "pch.h"
#include "CText.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CPathMgr.h"

#include "CFontMgr.h"
#include "CTransform.h"
#include "CEngine.h"

CText::CText()
	: CComponent(COMPONENT_TYPE::TEXTCOMP)
{
}

CText::CText(const CText& _Origin)
	: CComponent(COMPONENT_TYPE::TEXTCOMP)
{
}

CText::~CText()
{
	Safe_Del_Vec(m_vecTextInfo);
}

void CText::begin()
{
}

void CText::tick()
{
}

int CText::AddText(Vec2 _vOffsetPos, float _fSize, UINT _Color)
{
	tTextInfo* NewText = new tTextInfo;
	NewText->vOffsetPos = _vOffsetPos;
	NewText->fSize = _fSize;
	NewText->FontColor = _Color;

	int TextIdx = m_vecTextInfo.size();

	m_vecTextInfo.push_back(NewText);

	return TextIdx;
}

bool CText::SetReference(int _Textidx, UINT _RefDataType, DWORD_PTR _RefPtr)
{
	// �Է����� ���� �ε����� TextInfo �迭 ������츸 ����
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->RefDataType = _RefDataType;
		m_vecTextInfo[_Textidx]->RefPtr = _RefPtr;
		return true;
	}

	return false;
}

bool CText::SetText(int _Textidx, const wchar_t* _str)
{
	// �Է����� ���� �ε����� TextInfo �迭 ������츸 ����
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->str = _str;
		return true;
	}

	return false;
}

bool CText::SetOffset(int _Textidx, Vec2 _vOffset)
{
	// �Է����� ���� �ε����� TextInfo �迭 ������츸 ����
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->vOffsetPos = _vOffset;
		return true;
	}

	return false;
}

bool CText::SetSize(int _Textidx, float _size)
{
	// �Է����� ���� �ε����� TextInfo �迭 ������츸 ����
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->fSize = _size;
		return true;
	}

	return false;
}


bool CText::SetColor(int _Textidx, UINT _Color)
{
	// �Է����� ���� �ε����� TextInfo �迭 ������츸 ����
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->str = _Color;
		return true;
	}

	return false;
}

bool CText::SetLeft(int _Textidx)
{
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->Flags = FW1_VCENTER | FW1_LEFT;
		return true;
	}

	return false;
}

bool CText::SetCenter(int _Textidx)
{
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->Flags = FW1_VCENTER | FW1_CENTER;
		return true;
	}

	return false;
}

bool CText::SetRight(int _Textidx)
{
	if (m_vecTextInfo.size() < _Textidx)
	{
		m_vecTextInfo[_Textidx]->Flags = FW1_VCENTER | FW1_RIGHT;
		return true;
	}

	return false;
}

void CText::DeleteLastIdx()
{
	int MaxSize = (int)m_vecTextInfo.size();
	vector<CText::tTextInfo*>::iterator iter = m_vecTextInfo.begin() + (MaxSize - 1);
	delete* iter;
	*iter = nullptr;
	m_vecTextInfo.erase(iter);
}

void CText::finaltick()
{
	// ��ġ ��ȯ
	RECT rect = {};
	GetClientRect(CEngine::GetInst()->GetMainWnd(), &rect);
	float width = (float)(rect.right - rect.left);
	float height = (float)(rect.bottom - rect.top);

	Vec3 vPos = Transform()->GetWorldPos();
	vPos.y *= -1.f;

	for (auto Text : m_vecTextInfo)
	{
		// ��ġ ����
		Text->vFontPos.x = vPos.x + width / 2.f + Text->vOffsetPos.x;
		Text->vFontPos.y = vPos.y + height / 2.f - Text->vOffsetPos.y;

		// �����͸� �����ؼ� ����ϴ��� �Ǵ�, ��������� �ִ°�� �ؽ�Ʈ ����
		if (Text->bReference)
		{
			// �����Էµ� �����Ͱ� ���� ���۷��� �������� ���� ��ġ���� �ʴ´� = �����ʿ�
			switch (Text->RefDataType)
			{
			case (UINT)eTextRefType::WSTRING:
			{
				// ���ڿ� ���� ���� ������ ����
				if (Text->str != (wchar_t*)(Text->RefPtr))
					Text->str = (wchar_t*)(Text->RefPtr);
			}
				break;
			case (UINT)eTextRefType::INT:
			{
				int CurRefint = *(int*)(Text->RefPtr);

				// ���ڿ� ����
				if (CurRefint != Text->PrevRefData.i)
				{
					Text->str = std::to_wstring(CurRefint);
					Text->PrevRefData.i = CurRefint;
				}
			}
				break;
			case (UINT)eTextRefType::FLOAT_DP1:
			case (UINT)eTextRefType::FLOAT_DP2:
			{
				float CurReffloat = *((float*)(Text->RefPtr));
				CurReffloat = CTruncate(CurReffloat, Text->RefDataType);

				// ���ڿ� ����
				if (CurReffloat != Text->PrevRefData.f)
				{
					Text->str = std::to_wstring(CurReffloat);
					Text->PrevRefData.f = CurReffloat;
				}
			}
				break;
			}
		}
	}
}

void CText::render()
{
	LEVEL_STATE state = CLevelMgr::GetInst()->GetCurLevel()->GetState();
	// Play ���¿����� ����Ѵ�.

	if (state == LEVEL_STATE::PLAY)
	{
		for (auto Text : m_vecTextInfo)
		{
			CFontMgr::GetInst()->DrawFont(
				Text->str.c_str(),			// ���ڿ�
				Text->Font.c_str(),			// ��Ʈ�йи�
				Text->vFontPos.x,			// x Pos
				Text->vFontPos.y,			// y Pos
				Text->fSize,				// font size
				Text->FontColor,			// font color
				Text->Flags);				// flags
		}
	}
}

void CText::SavePrefab(const wstring& _key)
{
	// ����� ����
	wstring RelativePath = L"prefab\\text\\";
	RelativePath += _key;
	RelativePath += L".txtprp";	// Ȯ����

	// ���� ��� �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	path path_content = CPathMgr::GetInst()->GetContentPath();
	path path_prefab = path_content.wstring() + L"prefab\\text\\";

	if (false == exists(path_prefab))
	{
		create_directory(path_prefab);
	}

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// Ű��
	SaveWString(m_PrefabKey, pFile);

	UINT iCount = (UINT)m_vecTextInfo.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		tTextInfo* textinfo = m_vecTextInfo[i];

		SaveWString(textinfo->str, pFile);
		fwrite(&textinfo->vOffsetPos, sizeof(Vec2), 1, pFile);
		SaveWString(textinfo->Font, pFile);
		fwrite(&textinfo->fSize, sizeof(float), 1, pFile);
		fwrite(&textinfo->FontColor, sizeof(UINT), 1, pFile);
		fwrite(&textinfo->Flags, sizeof(UINT), 1, pFile);
	}

	fclose(pFile);
}

void CText::LoadPrefab(const wstring& _key)
{
	// �б���� ���Ͽ���
	FILE* pFile = nullptr;
	
	wstring RelativePath = L"prefab\\text\\";
	RelativePath += _key;
	RelativePath += L".txtprp";	// Ȯ����

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + RelativePath;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	// Ű��
	LoadWString(m_PrefabKey, pFile);

	UINT iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecTextInfo.resize(iCount);

	for (UINT i = 0; i < iCount; ++i)
	{
		tTextInfo* tmp = new tTextInfo;
		m_vecTextInfo[i] = tmp;

		LoadWString(m_vecTextInfo[i]->str, pFile);
		fread(&m_vecTextInfo[i]->vOffsetPos, sizeof(Vec2), 1, pFile);
		LoadWString(m_vecTextInfo[i]->Font, pFile);
		fread(&m_vecTextInfo[i]->fSize, sizeof(float), 1, pFile);
		fread(&m_vecTextInfo[i]->FontColor, sizeof(UINT), 1, pFile);
		fread(&m_vecTextInfo[i]->Flags, sizeof(UINT), 1, pFile);
	}
}

void CText::SaveToLevelFile(FILE* _File)
{
}

void CText::LoadFromLevelFile(FILE* _FILE)
{
}
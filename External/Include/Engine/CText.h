#pragma once
#include "CComponent.h"

class CText :
    public CComponent
{
private:
	// Text ������ ����ü
	union uniReferenceData
	{
		int		i;
		float	f;
	}typedef PrevRefData;

	enum class eRefDataType
	{
		INT,		// ������
		FLOAT_DP1,	// �Ǽ��� �Ҽ��� 1��°�ڸ�
		FLOAT_DP2,	// �Ǽ��� �Ҽ��� 2��°�ڸ�
	};

	struct tTextInfo
	{
		wstring				str;			// ����� ���ڿ�
		Vec2				vFontPos;		// ������ġ
		Vec2				vOffsetPos;		// ��������ġ
		wstring				Font;			// ��Ʈ�йи�
		float				fSize;			// ����ũ��
		UINT				FontColor;		// ���ڻ���
		UINT				Flags;			// �����÷���
		bool				bReference;		// ��������
		UINT				RefDataType;	// �����ϴ� ������ Ÿ��
		uniReferenceData	PrevRefData;	// ������ ����ϰ� ���� �� ���� �Է��� �����Ͱ�
		
		DWORD_PTR			RefPtr;		// ���������� ������

		tTextInfo()
			: vFontPos{}
			, vOffsetPos{}
			, fSize(100.f)
			, FontColor(FONT_RGBA(255, 255, 255, 255))
			, Flags(1 | 4)	// FW1_CENTER | FW1_VCENTER
			, bReference(false)
			, RefDataType(0)
			, RefPtr(0)
			, PrevRefData{}
		{
			str = L"NODATA";
			Font = L"�ؽ�Lv2���";
		}
	};

public:
	enum class eTextRefType
	{
		WSTRING,	// �⺻�� ���ڿ�
		INT,		// ������
		FLOAT_DP1,	// �Ҽ��� ù���ڸ�
		FLOAT_DP2,	// �Ҽ��� ��°�ڸ�
	};

private:
	wstring								m_PrefabKey;		// Text ������ ����Ѱ�� 
	vector<tTextInfo*>					m_vecTextInfo;		// ������ �ؽ�Ʈ ���� ����

public:
	virtual void finaltick() override;

	virtual void begin() override;
	virtual void tick() override;
	
	int AddText(Vec2 _vOffsetPos = Vec2(0.f, 0.f), float _fSize = 100.f, UINT _Color = FONT_RGBA(255, 255, 255, 255));
	bool SetReference(int _Textidx, UINT _RefDataType, DWORD_PTR _RefPtr);

	vector<tTextInfo*> GetTexts() { return m_vecTextInfo; }

	bool SetText(int _Textidx, const wchar_t* _str);
	bool SetOffset(int _Textidx, Vec2 _vOffset);
	bool SetSize(int _Textidx, float _size);
	bool SetColor(int _Textidx, UINT _Color);

	void render();

public:
	void SavePrefab(const wstring& _key);
	void LoadPrefab(const wstring& _key);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CText);

public:
    CText();
    CText(const CText& _Origin);
    ~CText();

	friend class TextUI;
};


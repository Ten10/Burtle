#pragma once

class CWindow;
class CDialog;
class CDataExchange
{
	bool m_bSave;
	DWORD m_LastControlID;
	CDialog& m_Dialog;

	friend CDialog;

	CDataExchange(CDialog& Dialog, bool bSave);
public:
	bool IsStoring() const;
	void Fail();
	void Fail(LPCTSTR Message, UINT nID);
	void DoDataExchange(UINT nIDC, CWindow& Window);
	void DoCheckBoxExchange(UINT nIDC, bool& bChecked);

	template <typename T>
	typename ::std::enable_if<!::std::is_base_of<CWindow, T>::value, void>::type DoDataExchange(UINT nIDC, T& Value)
	{
		if (IsStoring())
			m_Dialog.GetDlgItemValue(nIDC, Value);
		else
			m_Dialog.SetDlgItemValue(nIDC, Value);
	};
};

template <typename T>
void DDX(CDataExchange& DataExchange, UINT nIDC, T& Value)
{
	DataExchange.DoDataExchange(nIDC, Value);
}

void DDX_CheckBox(CDataExchange& DataExchange, UINT nIDC, bool& bChecked);
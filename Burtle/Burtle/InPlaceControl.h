#pragma once

class CListCtrlEx;
class CInPlaceControlImpl
{
public:
	CInPlaceControlImpl(CControl& Control, CListCtrlEx& ListCtrl, bool bAllowEmptyValues = false);
	~CInPlaceControlImpl();

	bool Show(DWORD dwStyle, int nRow, int nColumn, const CString& sOriginalValue);
	void InitializeEventMap();

	void RequestClose();
	void Close();
	void Cancel();

protected:
	void OnClose(const CWindow* pSender);
	void OnKillFocus(const CWindow* pSender, CFocusEventArgs& Args);
	void OnChar(const CWindow* pSender, CCharEventArgs& Args);

private:
	bool m_bAllowEmptyValues;
	bool m_bCancel;
	int m_RowIndex;
	int m_ColumnIndex;
	CString m_OriginalValue;
	CListCtrlEx& m_ListCtrl;
	CControl& m_Control;
};

template <typename TControl>
class CControlWrapper : public TControl
{
	friend class CListCtrlEx;
protected:
	CInPlaceControlImpl m_Impl;

	void InitializeEventMap() override
	{
		__super::InitializeEventMap();
		m_Impl.InitializeEventMap();
	}

public:
	CControlWrapper(CListCtrlEx& ListCtrl, bool bAllowEmptyValues = false)
		: m_Impl(*this, ListCtrl)
	{

	}

	bool Show(DWORD dwStyle, int nRowIndex, int nColumnIndex, const CString& sOriginalValue)
	{
		return m_Impl.Show(dwStyle, nRowIndex, nColumnIndex, sOriginalValue);
	}
	
	void Close()
	{
		m_Impl.Close();
	}
};

using CInPlaceEdit = CControlWrapper<CEdit>;

class CInPlaceComboBox : public CControlWrapper<CComboBox>
{
	friend CListCtrlEx;

	CInPlaceComboBox(CListCtrlEx& ListCtrl);

	void InitializeEventMap() override;
	void OnCloseUp(const CWindow* pSender);
};

class CInPlaceDateTimeControl : public CControlWrapper<CDateTimeControl>
{
	friend CListCtrlEx;

	CInPlaceDateTimeControl(CListCtrlEx& ListCtrl);

	CString GetSelectedItem() const override;
	bool SetSelectedItem(const CString& sItem) override;

public:
	bool Show(DWORD dwStyle, int nRowIndex, int nColumnIndex, const CString& sOriginalValue);
};
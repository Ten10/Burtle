#pragma once
#include "InPlaceControl.h"

class CListCtrlEx : public CListCtrl
{
public:
	CListCtrlEx();
	~CListCtrlEx();

	enum eColumnEditType
	{
		typeReadOnly,
		typeDateCtrl,
		typeEditCtrl,
		typeComboCtrl
	};

	function<void(size_t Row)> fnOnCheckBoxClicked;
	function<eColumnEditType(size_t Column)> fnGetColumnEditType;
	function<void(CComboBox&, size_t Row, size_t Column)> fnInitializeComboBox;

	bool OnOK();
	void EnableVScroll(bool bEnable = true);
	void EnableHScroll(bool bEnable = true);

protected:
	void OnHScroll(const CWindow* pSender, CScrollEventArgs& Args);
	void OnVScroll(const CWindow* pSender, CScrollEventArgs& Args);
	void OnLButtonDown(const CWindow* pSender, CButtonPressEventArgs& Args);
	void OnBeginLabelEdit(const CWindow* pSender, CLabelBeginEditArgs& Args);

	void InitializeEventMap() override;

private:
	DWORD m_ComboBoxStyle;
	DWORD m_EditBoxStyle;
	DWORD m_DateTimeControlStyle;
	CInPlaceComboBox m_InPlaceComboBox;
	CInPlaceEdit m_InPlaceEditBox;
	CInPlaceDateTimeControl m_InPlaceDateTimeControl;

	bool ShowInPlaceEdit(UINT RowIndex, UINT ColumnIndex, bool bShow);

	bool IsCombo(int ColumnIndex);
	bool IsReadOnly(int ColumnIndex);
	void ScrollToView(int ColumnIndex, /*int iOffSet, */CRect& obCellRect);
};
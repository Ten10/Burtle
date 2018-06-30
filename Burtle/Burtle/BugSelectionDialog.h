// BugSelectionDialog.h : Declaration of the CBugSelectionDialog

#pragma once

#include "resource.h"       // main symbols
#include "ListCtrlEx.h"

// CBugSelectionDialog
struct CProgressEventArgs : public CEventArgs
{
	CStringA TaskName;
	CStringA Text;

	CProgressEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	{
		if (CStringA* pTaskName = reinterpret_cast<CStringA*>(wParam))
		{
			TaskName = *pTaskName;
			delete pTaskName;
		}
		if (CStringA* pText = reinterpret_cast<CStringA*>(lParam))
		{
			Text = *pText;
			delete pText;
		}
	}
};

struct CScheduledTaskEventArgs : public CEventArgs
{
	void* pContext = nullptr;
	concurrency::TaskProc_t pTask = nullptr;

	CScheduledTaskEventArgs(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	{
		pTask = reinterpret_cast<concurrency::TaskProc_t>(wParam);
		pContext = reinterpret_cast<void*>(lParam);
	}
};

class CBugSelectionDialog : public CDialog, public concurrency::scheduler_interface
{
protected:
	struct CColumnInfo
	{
		CListCtrlEx::eColumnEditType Type;

		CStringA Name;
		CString DisplayName;
		vector<CBugField::CFieldValue> AllowedValues;

		vector<CString> RowValues;
	};

	CHAR m_Delimiter;
	CFont m_OldListFont;
	CFont m_BoldListFont;
	CComboBox m_SearchByComboBox;
	CListCtrlEx m_ListCtrl;
	CStringA m_RequestName;
	CStringA m_ProgressMessage;
	CLayoutHelper m_LayoutHelper;

	CServer& m_Server;
	concurrency::task<vector<CBugField>> m_GetBugFields;
	concurrency::task<CBugzillaQuery> m_GetBugInfos;

	CBugzillaQuery& m_Query;
	vector<CColumnInfo> m_Columns;
	map<int, CBugModificationInfo>& m_BugModificationInfos;
	UINT m_MaxNumberOfBugsPerSearch;

	int GetBugIDFromRow(size_t Row) const;
	CString GetCellText(size_t Row, size_t Column) const;

	void InitSearchByComboBox();
	void InitializeEventMap() override;

	void schedule(concurrency::TaskProc_t, void*) override;
	void OnProgressMessage(const CStringA& TaskName, const CStringA& Message) const;
public:
	CBugSelectionDialog(CServer& Server, CBugzillaQuery& Query, map<int, CBugModificationInfo>& ModificationInfo, UINT MaxNumberOfBugsPerSearch);
	~CBugSelectionDialog();

	enum { IDD = IDD_BUGSELECTIONDIALOG };

	bool OnInitDialog() override;
	void DoDataExchange(CDataExchange& DX) override;
	void OnFinalMessage() override;

	map<int, CBugModificationInfo> GetBugModificationInfos() const;

	void OnBugFieldsAvailable(vector<CBugField>& Fields);
	void OnBugInfosAvailable(CBugzillaQuery&& Query);
	void OnBugInfosCompleted();

	void OnBnClickedSearchButton(const CWindow* pSender);
	void OnSelectionChanged(const CWindow* pSender);
	void OnEndLabelEdit(const CWindow* pSender, CLabelEndEditArgs& Args);
	void OnListCustomDraw(const CWindow* pSender, CListControlCustomDrawArgs& Args);
	void OnGetdispinfoList(const CWindow* pSender, CGetItemInfoArgs& Args);
	void OnTaskProgressMessage(const CWindow* pSender, CProgressEventArgs& Args);
	void OnSize(const CWindow* pSender, CSizeEventArgs& Args);
	void OnScheduledTask(const CWindow* pSender, CScheduledTaskEventArgs& Args);
};



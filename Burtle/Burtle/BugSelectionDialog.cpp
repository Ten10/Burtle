// BugSelectionDialog.cpp : Implementation of CBugSelectionDialog

#include "stdafx.h"
#include "BurtleProvider.h"
#include "BugSelectionDialog.h"
#include "FieldGetSet.h"
#include "FieldNameRetrieval.h"
#include "Util.h"

using namespace Bugzilla::BugFunctionParams;

static const UINT WM_TASK_PROGRESS = WM_USER;
static const UINT WM_SCHEDULE_TASK = WM_USER + 1;

class CScopedSetCallback
{
	CServer& m_Server;
public:
	CScopedSetCallback(CServer& Server, const CServer::ProgressCallback& Callback)
		: m_Server(Server)
	{
		Server.Callback = Callback;
		Callback("Started");
	}

	~CScopedSetCallback()
	{
		if (m_Server.Callback)
		{
			m_Server.Callback("Completed");
			m_Server.Callback("");
		}
		m_Server.Callback = nullptr;
	}
};

void CBugSelectionDialog::InitializeEventMap()
{
	__super::InitializeEventMap();
	m_ListCtrl.OnEndLabelEdit += CEventHandler<CLabelEndEditArgs>(&CBugSelectionDialog::OnEndLabelEdit, *this);
	m_ListCtrl.OnCustomDraw += CEventHandler<CListControlCustomDrawArgs>(&CBugSelectionDialog::OnListCustomDraw, *this);
	m_ListCtrl.OnGetItemInfo += CEventHandler<CGetItemInfoArgs>(&CBugSelectionDialog::OnGetdispinfoList, *this);
	m_SearchByComboBox.OnSelectionChanged += CEventHandler<CNoEventArgs>(&CBugSelectionDialog::OnSelectionChanged, *this);
	CButtonPressEvent<IDC_SEARCH_BUTTON>(m_EventMap) += CEventHandler<CNoEventArgs>(&CBugSelectionDialog::OnBnClickedSearchButton, *this);
	CEvent<WM_TASK_PROGRESS, CProgressEventArgs>(m_EventMap) += CEventHandler<CProgressEventArgs>(&CBugSelectionDialog::OnTaskProgressMessage, *this);
	CEvent<WM_SCHEDULE_TASK, CScheduledTaskEventArgs>(m_EventMap) += CEventHandler<CScheduledTaskEventArgs>(&CBugSelectionDialog::OnScheduledTask, *this);
	CWindowBase::OnSize += CEventHandler<CSizeEventArgs>(&CBugSelectionDialog::OnSize, *this);
	CWindowBase::OnGetMinMaxInfo += [=](const CWindow* pSender, CGetMinMaxInfoArgs& Args)
	{
		Args.MinMaxInfo.ptMinTrackSize.x = 250;
		Args.MinMaxInfo.ptMinTrackSize.y = 150;
	};
}

void CBugSelectionDialog::schedule(concurrency::TaskProc_t pTask, void* pContext)
{
	PostMessage(WM_SCHEDULE_TASK, (WPARAM)pTask, (LPARAM)pContext);
}

void CBugSelectionDialog::OnProgressMessage(const CStringA& TaskName, const CStringA& Message) const
{
	PostMessage(WM_TASK_PROGRESS, (WPARAM)new CStringA(TaskName), (LPARAM)new CStringA(Message));
}

CBugSelectionDialog::CBugSelectionDialog(CServer& Server, CBugzillaQuery& Query, map<int, CBugModificationInfo>& ModificationInfo, UINT MaxNumberOfBugsPerSearch)
	: m_Query(Query),
	m_Server(Server),
	m_BugModificationInfos(ModificationInfo),
	m_MaxNumberOfBugsPerSearch(MaxNumberOfBugsPerSearch)
{
	m_Delimiter = _T(';');
	m_ListCtrl.fnOnCheckBoxClicked = [=](size_t Row)
	{
		int nBugID = GetBugIDFromRow(Row);
		auto it = m_BugModificationInfos.find(nBugID);
		if (it != m_BugModificationInfos.end())
			m_BugModificationInfos.erase(it);
		else
			m_BugModificationInfos.emplace(nBugID, CBugModificationInfo());
		m_ListCtrl.RedrawItems((int)Row, (int)Row);
	};
	m_ListCtrl.fnGetColumnEditType = [=](size_t Column)
	{
		CListCtrlEx::eColumnEditType type = CListCtrlEx::typeReadOnly;
		if (Column < m_Columns.size())
			type = m_Columns[Column].Type;
		return type;
	};
	m_ListCtrl.fnInitializeComboBox = [=](CComboBox& ComboBox, size_t Row, size_t Column)
	{
		if (Column < static_cast<int>(m_Columns.size()))
		{
			auto& columnInfo = m_Columns[Column];
			if (Row < columnInfo.RowValues.size())
			{
				CString sValue = columnInfo.RowValues[Row];
				auto& values = columnInfo.AllowedValues;
				auto it = find_if(values.begin(), values.end(), [=](const CBugField::CFieldValue& FieldValue)
				{
					return CA2W(FieldValue.Name) == sValue;
				});
				if (it == values.end())
				{
					ComboBox.AddString(sValue);
					ComboBox.SetCurSel(0);
				}
				else
				{
					int nSelectedItem = -1;
					CString sOriginalValue = GetCellText(Row, Column);
					if (it->CanChangeTo.Container.size())
					{
						for (const CBugField::CFieldValue::CStatusOption& value : it->CanChangeTo.Container)

						{
							CString sValue = CA2W(value.Name);
							int nItem = ComboBox.AddString(sValue);
							if (sValue == sOriginalValue)
								nSelectedItem = nItem;
						}
					}
					else
					{
						for (const CBugField::CFieldValue& fieldValue : values)

						{
							CString sValue = CA2W(fieldValue.Name);
							int nItem = ComboBox.AddString(sValue);
							if (sValue == sOriginalValue)
								nSelectedItem = nItem;
						}
					}
					ComboBox.SetCurSel(nSelectedItem);
				}
			}
		}
	};
}

CBugSelectionDialog::~CBugSelectionDialog()
{
}

// CBugSelectionDialog

int CBugSelectionDialog::GetBugIDFromRow(size_t Row) const
{
	int nBugID = 0;
	if (Row < m_Query.BugInfos.size())
		nBugID = m_Query.BugInfos[Row].GetBugID();
	ASSERT(nBugID);
	return nBugID;
}

CString CBugSelectionDialog::GetCellText(size_t Row, size_t Column) const
{
	int nBugID = GetBugIDFromRow(Row);
	ENSURE(nBugID > 0 && Column < m_Columns.size());

	auto& info = m_Columns[Column];
	CString cellText;
	auto it = m_BugModificationInfos.find(nBugID);
	if (it != m_BugModificationInfos.end())
	{
		auto it2 = it->second.FieldNameToNewFieldValue.find(info.Name);
		if (it2 != it->second.FieldNameToNewFieldValue.end())
			cellText = it2->second;
	}
	if (cellText.empty())
	{
		ENSURE(Row < info.RowValues.size());
		cellText = info.RowValues[Row];
	}
	return cellText;
}

void CBugSelectionDialog::InitSearchByComboBox()
{
	FieldNameAndValueVector fields;
	CFieldNameRetreiveVisitor visitor;
	CBugSearchParam param;
	param.Accept(visitor);

	for (auto& fieldName : visitor.FieldNames)

		m_SearchByComboBox.AddString(CA2T(fieldName));
	m_SearchByComboBox.SetCurSel(-1);
}

bool CBugSelectionDialog::OnInitDialog()
{
	__super::OnInitDialog();
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	LOGFONT font = {0};
	m_ListCtrl.GetFont().GetLogFont(font);
	font.lfWeight = FW_BOLD;
	m_BoldListFont.CreateFontIndirect(font);

	using eLayoutFlags = CLayoutHelper::CLayoutInitializer::eLayoutFlags;
	CLayoutHelper::CLayoutInitializer initializer = m_LayoutHelper.InitializeLayout(m_hWnd);
	initializer.AddControl(IDC_SEARCH_BUTTON, eLayoutFlags::Right);
	initializer.AddControl(IDC_SEARCH_COMBO, eLayoutFlags::Right);
	initializer.AddControl(IDC_SEARCH_EDIT, eLayoutFlags::LeftRight);
	initializer.AddControl(IDC_SEARCH_LIST, eLayoutFlags::All);
	initializer.AddControl(IDC_SEARCH_GROUP_BOX, eLayoutFlags::All);
	initializer.AddControl(IDOK, eLayoutFlags::BottomRight);
	initializer.AddControl(IDCANCEL, eLayoutFlags::BottomRight);

	InitSearchByComboBox();

	m_GetBugFields = concurrency::create_task([=]()
	{
		CScopedSetCallback setCallback(m_Server, [=](const CStringA& Message)
		{
			OnProgressMessage("Bug Fields", Message);
		});
		CGetBugFieldParam param;
		return m_Server.GetBugFields(param).Objects.Container;
	});
	m_GetBugFields.then([=](concurrency::task<vector<CBugField>>& TaskFields)
	{ 
		try
		{
			OnBugFieldsAvailable(TaskFields.get());
		}
		catch (const CBugzillaException& BugzillaException)
		{
			BurtleMessageBox(CA2T(BugzillaException.ErrorMessage), MB_OK);
		}
	}, *this);
	::SetFocus(GetDlgItem(m_hWnd, IDC_SEARCH_BUTTON));
	return false;
}

void CBugSelectionDialog::DoDataExchange(CDataExchange& DX)
{
	DDX(DX, IDC_SEARCH_COMBO, m_SearchByComboBox);
	DDX(DX, IDC_SEARCH_LIST, m_ListCtrl);

	if (!DX.IsStoring())
	{
		for (size_t index = 0; index < m_Columns.size(); ++index)
			m_ListCtrl.SetColumnWidth(static_cast<int>(index), LVSCW_AUTOSIZE_USEHEADER);
		
		CFieldNameRetreiveVisitor retreiveFieldNameVisitor;
		m_Query.SearchParam.Accept(retreiveFieldNameVisitor);

		FieldNameAndValueVector fieldNameAndValues;
		for (auto& fieldName : retreiveFieldNameVisitor.FieldNames)
			fieldNameAndValues.emplace_back(fieldName, CStringA());

		CGetSetFieldsVisitor visitor(fieldNameAndValues, true);
		m_Query.SearchParam.Accept(visitor);

		CStringA sValue;
		for (auto& fieldNameAndValue : fieldNameAndValues)
		{
			if (!fieldNameAndValue.second.empty())
				sValue += fieldNameAndValue.first + "=" + fieldNameAndValue.second + m_Delimiter;
		}
		SetDlgItemValue(IDC_SEARCH_EDIT, sValue);
	}
}

void CBugSelectionDialog::OnFinalMessage()
{
	__super::OnFinalMessage();
	try
	{
		m_GetBugFields.wait();
		m_GetBugInfos.wait();
	}
	catch (exception&)
	{
	}
}

map<int, CBugModificationInfo> CBugSelectionDialog::GetBugModificationInfos() const
{
	return m_BugModificationInfos;
}

void CBugSelectionDialog::OnBugFieldsAvailable(vector<CBugField>& Fields)
{
	m_RequestName.clear();
	m_ProgressMessage.clear();
	m_ListCtrl.Invalidate();
	UINT nColumnCount = 0;
	for (const CBugField& field : Fields)
	{
		if (!field.ShouldDisplay())
			continue;
		CColumnInfo info;
		info.Name = CStringA(field.Name.c_str());
		info.DisplayName = CA2W(field.DisplayName);
		info.AllowedValues = field.Values.Container;
		switch (field.FieldType)
		{
		case CBugField::typeDate:
			info.Type = CListCtrlEx::typeDateCtrl;
			break;
		case CBugField::typeFreeText:
		case CBugField::typeLargeTextBox:
			info.Type = CListCtrlEx::typeEditCtrl;
			break;
		case CBugField::typeDropDown:
		case CBugField::typeMultipleSelectionBox:
			info.Type = CListCtrlEx::typeComboCtrl;
			break;
		default:
			info.Type = CListCtrlEx::typeReadOnly;
			break;
		}
		m_ListCtrl.InsertColumn(nColumnCount++, info.DisplayName.c_str());
		m_Columns.push_back(move(info));
	}

	m_ListCtrl.SetItemCount(0);
	for (size_t index = 0; index < m_Columns.size(); ++index)
		m_ListCtrl.SetColumnWidth(static_cast<int>(index), LVSCW_AUTOSIZE_USEHEADER);
	UpdateData(false);
}

void CBugSelectionDialog::OnBugInfosAvailable(CBugzillaQuery&& Query)
{
	/*TODO need to add cancellation token so we can cancel the tasks when exiting the dialog*/
	m_RequestName.clear();
	m_ProgressMessage.clear();
	m_ListCtrl.Invalidate();

	m_Query = std::move(Query);
	for (auto itColumn = m_Columns.begin(); itColumn != m_Columns.end(); ++itColumn)
		itColumn->RowValues.clear();

	vector<CListCtrlEx::eColumnEditType> fieldTypes;
	vector<pair<string, string>> fieldNameAndValueMain;
	fieldTypes.reserve(m_Columns.size());
	fieldNameAndValueMain.reserve(m_Columns.size());
	for (auto it = m_Columns.begin(); it != m_Columns.end(); ++it)
	{
		fieldTypes.push_back(it->Type);
		fieldNameAndValueMain.push_back(make_pair(it->Name.c_str(), ""));
	}

	for (auto it = m_Query.BugInfos.begin(); it != m_Query.BugInfos.end(); ++it)
	{
		auto fieldNameAndValue = fieldNameAndValueMain;
		CGetSetFieldsVisitor visitor(fieldNameAndValue, true);
		it->Accept(visitor);
		for (auto itValue = fieldNameAndValue.begin(); itValue != fieldNameAndValue.end(); ++itValue)
		{
			CStringA fieldName = itValue->first;
			if (itValue->second.empty() && !it->BugInfo.hasMember(fieldName))
			{
				//dangerous, we will try and support this field if we can determine its type.
				size_t index = distance(fieldNameAndValue.begin(), itValue);
				CListCtrlEx::eColumnEditType fieldType = fieldTypes[index];
				if (fieldType != CListCtrlEx::eColumnEditType::typeDateCtrl)
					m_Columns[index].Type = CListCtrlEx::eColumnEditType::typeReadOnly; //cannot edit this field
				else
				{
					tm time = { 0 };
					it->BugInfo[fieldName] = XmlRpcValue(&time);
				}
			}
			m_Columns[distance(fieldNameAndValue.begin(), itValue)].RowValues.push_back(CA2W(itValue->second));
		}
	}
	OnBugInfosCompleted();
}

void CBugSelectionDialog::OnBugInfosCompleted()
{
	m_ListCtrl.SetItemCount((UINT)m_Query.BugInfos.size());
	UpdateData(false);
	EnableDlgItem(IDC_SEARCH_BUTTON, true);
}

void CBugSelectionDialog::OnBnClickedSearchButton(const CWindow* pSender)
{
	m_BugModificationInfos.clear();

	CString searchParamText;
	GetDlgItemValue(IDC_SEARCH_EDIT, searchParamText);

	FieldNameAndValueVector fieldNameAndValue;
	vector<CString> searchParams = Split(searchParamText, _T(";"));
	fieldNameAndValue.reserve(searchParams.size());
	for (const CString& searchParam : searchParams)
	{
		size_t pos = searchParam.find_first_of('=');
		if (pos == CString::npos)
			continue;

		fieldNameAndValue.emplace_back(CT2A(searchParam.substr(0, pos)), CT2A(searchParam.substr(pos + 1)));
	}
	
	BugFunctionParams::CBugSearchParam param;
	param.Offset = 0;
	param.Limit = m_MaxNumberOfBugsPerSearch;

	CGetSetFieldsVisitor visitor(fieldNameAndValue, false);
	param.Accept(visitor);
	
	m_GetBugInfos = concurrency::create_task([=]()
	{
		m_GetBugFields.wait();
		CScopedSetCallback setCallback(m_Server, [=](const CStringA& Message) { OnProgressMessage("Bug Information", Message); });
		CBugzillaQuery query;
		query.SearchParam = param;
		query.BugInfos = m_Server.GetBugs(param).Objects.Container;
		return std::move(query);
	});
	m_GetBugInfos.then([=](concurrency::task<CBugzillaQuery> QueryTask) 
	{ 
		try
		{
			OnBugInfosAvailable(QueryTask.get());
		}
		catch (const CBugzillaException& BugzillaException)
		{
			OnBugInfosCompleted();
			BurtleMessageBox(CA2T(BugzillaException.ErrorMessage), MB_OK);
		}
	}, *this);
	EnableDlgItem(IDC_SEARCH_BUTTON, false);
}

void CBugSelectionDialog::OnSelectionChanged(const CWindow* pSender)
{
	CWindow* pWindow = CWindow::FromHandle(GetDlgItem(m_hWnd, IDC_SEARCH_EDIT));
	if (!pWindow)
		return;

	CString sField = m_SearchByComboBox.GetSelectedItem();
	sField += _T("=");

	CString sText = pWindow->GetText();
	size_t pos = sText.find(sField);
	size_t to = CString::npos;
	if (pos != CString::npos)
	{
		pos += sField.length();
		to = sText.find(TCHAR(m_Delimiter), pos);
	}
	else
	{
		if (sText.empty())
			sText = sField;
		else
		{
			sText.push_back(_T(';'));
			sText += sField;
		}
		pos = sText.length();
	}
	if (to == CString::npos)
		to = sText.length();
	pWindow->SetText(sText);
	pWindow->SendMessage(EM_SETSEL, pos, to);
	pWindow->SetFocus();
}

void CBugSelectionDialog::OnEndLabelEdit(const CWindow* pSender, CLabelEndEditArgs& Args)
{
	int nRow = Args.Row;
	int nColumn = Args.Column;

	int nBugID = GetBugIDFromRow(nRow);

	auto it = m_BugModificationInfos.find(nBugID);
	CStringA sColumnName = m_Columns[nColumn].Name;
	CString sNewValue = Args.Text;
	CString sOldValue = m_ListCtrl.GetItemText(nRow, nColumn);
	if (sOldValue == sNewValue)
		return;
	else if (it != m_BugModificationInfos.end())
	{
		auto& container = it->second.FieldNameToNewFieldValue;
		auto it2 = container.find(sColumnName);
		if (it2 != container.end())
		{
			if (m_Columns[nColumn].RowValues[nRow] == sNewValue)
			{
				container.erase(it2);
				if (container.empty())
					m_BugModificationInfos.erase(it);
			}
			else
				it2->second = sNewValue;
		}
		else
			container[sColumnName] = sNewValue;
	}
	else
	{
		CBugModificationInfo info;
		info.FieldNameToNewFieldValue[sColumnName] = sNewValue;
		m_BugModificationInfos.emplace(nBugID, info);
	}
	m_ListCtrl.Invalidate();
}

void CBugSelectionDialog::OnListCustomDraw(const CWindow* pSender, CListControlCustomDrawArgs& Args)
{
	switch (Args.DrawStage)
	{
		case CDDS_PREPAINT:
			Args.Result = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
			break;
		case CDDS_ITEMPREPAINT:
			Args.Result = CDRF_NOTIFYSUBITEMDRAW;
			break;
		case CDDS_POSTPAINT:
			{
				CDC* pDC = CDC::FromHandle(Args.DeviceContext);
				if (pDC && !m_RequestName.empty())
				{
					string text = m_RequestName;
					if (!m_ProgressMessage.empty())
					{
						text += ": ";
						text += m_ProgressMessage;
					}
					CRect textRect;
					pDC->Draw(text, textRect, DT_CALCRECT);
					
					textRect.OffsetRect(textRect.Center() - CClientRect(m_hWnd).Center());

					CBrush greySolidBrush;
					if (greySolidBrush.CreateSolidBrush(eColor::LightGrey))
						pDC->Fill(textRect, greySolidBrush);

					pDC->Draw(text, textRect, DT_CENTER);
				}
			}
			break;
		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
			{
				CDC* pDC = CDC::FromHandle(Args.DeviceContext);

				auto& column = m_Columns[Args.Column];

				Args.SetBackgroundColor(column.Type == CListCtrlEx::typeReadOnly ? (COLORREF)eColor::LightGrey : (COLORREF)eColor::White);

				int nBugID = GetBugIDFromRow(Args.Row);
				auto it = m_BugModificationInfos.find(nBugID);
				if (it != m_BugModificationInfos.end())
				{
					ENSURE((size_t)Args.Column < m_Columns.size());
					auto it2 = it->second.FieldNameToNewFieldValue.find(column.Name);
					if (it2 != it->second.FieldNameToNewFieldValue.end())
					{
						m_OldListFont = pDC->SelectObject(m_BoldListFont);
						Args.Result = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
					}
				}
			}
			break;
		case CDDS_SUBITEM | CDDS_ITEMPOSTPAINT:
			{
				if (m_OldListFont)
				{
					CDC* pDC = CDC::FromHandle(Args.DeviceContext);
					pDC->SelectObject(m_OldListFont);
					m_OldListFont = CFont();
				}
			}
			break;
		default:
			break;
	}
}

void CBugSelectionDialog::OnGetdispinfoList(const CWindow* pSender, CGetItemInfoArgs& Args)
{
	int row = Args.Row;
	int col = Args.Column;

	if (row < 0 || col < 0)
		return;

	if (Args.Mask & LVIF_TEXT)
		Args.SetItemText(GetCellText(row, col));

	if (Args.Mask & LVIF_STATE)
	{
		int nBugID = GetBugIDFromRow(row);
		auto it = m_BugModificationInfos.find(nBugID);
		bool bModifiedBug = it != m_BugModificationInfos.end();
		Args.SetItemState(INDEXTOSTATEIMAGEMASK(bModifiedBug ? 2 : 1), LVIS_STATEIMAGEMASK);
	}
}

void CBugSelectionDialog::OnTaskProgressMessage(const CWindow* pSender, CProgressEventArgs& Args)
{
	if (!Args.Text.empty())
	{
		m_RequestName = Args.TaskName.empty() ? "Unknown Request" : Args.TaskName;
		m_ProgressMessage = Args.Text;
	}
	else
	{
		m_RequestName.clear();
		m_ProgressMessage.clear();
	}
	m_ListCtrl.Invalidate(true);
}

void CBugSelectionDialog::OnSize(const CWindow* pSender, CSizeEventArgs& Args)
{
	m_LayoutHelper.OnLayoutChanged();
}

void CBugSelectionDialog::OnScheduledTask(const CWindow* pSender, CScheduledTaskEventArgs& Args)
{
	if (Args.pTask)
		(*Args.pTask)(Args.pContext);
}
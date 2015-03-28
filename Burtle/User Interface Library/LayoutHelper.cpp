#include "stdafx.h"
#include "LayoutHelper.h"

static bool HasLayoutFlag(CLayoutHelper::CLayoutInitializer::eLayoutFlags Flags, CLayoutHelper::CLayoutInitializer::eLayoutFlags Flag)
{
	return ((DWORD)Flags & (DWORD)Flag) == (DWORD)Flag;
}

CLayoutHelper::CLayoutInitializer::CLayoutInitializer(CLayoutHelper& LayoutHelper)
	: m_LayoutHelper(LayoutHelper)
{
}

CLayoutHelper::CLayoutInitializer::~CLayoutInitializer()
{
	CScreenRect windowRect = m_LayoutHelper.m_WindowRect;
	for (auto it = m_ControlIDToLayoutFlags.begin(); it != m_ControlIDToLayoutFlags.end(); ++it)
	{
		CControlInfo& info = m_LayoutHelper.m_ControlIDToControlInfo[it->first];
		info.ControlWindow = ::GetDlgItem(m_LayoutHelper.m_Window, it->first);

		CScreenRect rect(info.ControlWindow);
		info.ControlMarginRect = CRect(-1, -1, -1, -1);
		if (HasLayoutFlag(it->second, eLayoutFlags::Left))
			info.ControlMarginRect.left = rect.left - windowRect.left;
		if (HasLayoutFlag(it->second, eLayoutFlags::Right))
			info.ControlMarginRect.right = windowRect.right - rect.right;
		if (HasLayoutFlag(it->second, eLayoutFlags::Top))
			info.ControlMarginRect.top = rect.top - windowRect.top;
		if (HasLayoutFlag(it->second, eLayoutFlags::Bottom))
			info.ControlMarginRect.bottom = windowRect.bottom - rect.bottom;
	}
}

void CLayoutHelper::CLayoutInitializer::AddControl(UINT ControlID, eLayoutFlags LayoutFlags)
{
	m_ControlIDToLayoutFlags[ControlID] = LayoutFlags;
}

CLayoutHelper::CLayoutInitializer CLayoutHelper::InitializeLayout(HWND hWindow)
{
	m_Window = hWindow;
	m_WindowRect = CClientRect(m_Window).ToScreen(m_Window);
	return CLayoutInitializer(*this);
}

bool CLayoutHelper::OnLayoutChanged()
{
	HDWP hDWP = ::BeginDeferWindowPos((int)m_ControlIDToControlInfo.size());
	if (!hDWP)
		return false;

	CScreenRect screenRect = CClientRect(m_Window).ToScreen(m_Window);

	if (screenRect.Width() == m_WindowRect.Width() && screenRect.Height() == m_WindowRect.Height())
		return false;

	for (auto it = m_ControlIDToControlInfo.begin(); it != m_ControlIDToControlInfo.end(); ++it)
	{
		CControlInfo& info = it->second;
		CScreenRect controlRect(info.ControlWindow);
		if (info.ControlMarginRect.left > 0 && info.ControlMarginRect.right > 0)
		{
			controlRect.left = screenRect.left + info.ControlMarginRect.left;
			controlRect.right = screenRect.right - info.ControlMarginRect.right;
		}
		else if (info.ControlMarginRect.right > 0)
		{
			UINT nWidth = controlRect.Width();
			controlRect.right = screenRect.right - info.ControlMarginRect.right;
			controlRect.left = controlRect.right - nWidth;
		}

		if (info.ControlMarginRect.top > 0 && info.ControlMarginRect.bottom > 0)
		{
			controlRect.top = screenRect.top + info.ControlMarginRect.top;
			controlRect.bottom = screenRect.bottom - info.ControlMarginRect.bottom;
		}
		else if (info.ControlMarginRect.bottom > 0)
		{
			UINT nHeight = controlRect.Height();
			controlRect.bottom = screenRect.bottom - info.ControlMarginRect.bottom;
			controlRect.top = controlRect.bottom - nHeight;
		}

		CWindowRelativeRect relativeRect(controlRect, screenRect);
		hDWP = ::DeferWindowPos(hDWP, info.ControlWindow, nullptr, relativeRect.left, relativeRect.top, relativeRect.Width(), relativeRect.Height(), SWP_NOZORDER);
		if (!hDWP)
			return false;
	}
	bool bSuccess = ::EndDeferWindowPos(hDWP) ? true : false;
	m_WindowRect = screenRect;
	return bSuccess;
}
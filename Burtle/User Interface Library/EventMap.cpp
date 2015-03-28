#include "stdafx.h"
#include "EventMap.h"
#include "Window.h"

CEventID::CEventID(UINT uMsg, UINT SubMsgID, UINT ControlID)
	: Message(uMsg),
	SubMessageID(SubMsgID),
	ControlID(ControlID)
{

}

bool CEventID::operator==(const CEventID& Other) const
{
	return Message == Other.Message
		&& SubMessageID == Other.SubMessageID
		&& ControlID == Other.ControlID;
}

CEventID CEventID::FromMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nSubMsgID = 0;
	UINT nControlID = 0;
	if (uMsg == WM_NOTIFY || uMsg == WM_NOTIFY_REFLECT)
	{
		auto pNMHDR = (LPNMHDR)lParam;
		if (pNMHDR)
		{
			nSubMsgID = pNMHDR->code;
			if (uMsg == WM_NOTIFY)
				nControlID = (UINT)pNMHDR->idFrom;
		}
	}
	else if (uMsg == WM_CONTROL_REFLECT)
	{
		nSubMsgID = (UINT)wParam;
	}
	else if (uMsg == WM_COMMAND)
	{
		nSubMsgID = HIWORD(wParam);
		nControlID = LOWORD(wParam);
	}
	return CEventID(uMsg, nSubMsgID, nControlID);
}

size_t CEventIDHasher::operator()(const CEventID& EventID) const
{
	return ((hash<UINT>()(EventID.Message)
		^ (hash<UINT>()(EventID.SubMessageID) << 1) >> 1)
		^ (hash<UINT>()(EventID.ControlID) << 1));
}
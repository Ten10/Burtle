#pragma once
#include "EventArgs.h"

#include <unordered_map>
#include <functional>
#include <memory>

#define WM_REFLECT 0x2000
#define WM_NOTIFY_REFLECT (WM_NOTIFY + WM_REFLECT)
#define WM_CONTROL_REFLECT (WM_COMMAND + WM_REFLECT)

class CWindow;
struct CEventID
{
	UINT Message;
	UINT SubMessageID;
	UINT ControlID;
	CEventID(UINT uMsg, UINT SubMsgID, UINT ControlID);

	bool operator==(const CEventID& Other) const;

	static CEventID FromMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

struct CEventIDHasher
{
	size_t operator()(const CEventID& MessageID) const;
};

struct CEventHandlerBase
{
public:
	virtual ~CEventHandlerBase()
	{
	}
};

template <typename TEventArgs = CEventArgs>
struct CEventHandler : public CEventHandlerBase
{
	typedef ::std::function<void(const CWindow*, TEventArgs& Args)> EventDelegateFunction;
	EventDelegateFunction EventDelegate;
	
	CEventHandler(const EventDelegateFunction& Delegate)
		: EventDelegate(Delegate)
	{
	}

	template <typename TClass>
	CEventHandler(void (TClass::*Delegate)(const CWindow*, TEventArgs&), TClass& Instance)
		: EventDelegate(bind(Delegate, &Instance, ::std::placeholders::_1, ::std::placeholders::_2))
	{
	}

	void operator () (const CWindow* pSender, TEventArgs& Args) const
	{
		EventDelegate(pSender, Args);
	}
};

template <>
struct CEventHandler<CNoEventArgs> : public CEventHandlerBase
{
	typedef ::std::function<void(const CWindow*)> EventDelegateFunction;
	EventDelegateFunction EventDelegate;

	CEventHandler(const EventDelegateFunction& Delegate)
		: EventDelegate(Delegate)
	{
	}

	template <typename TClass>
	CEventHandler(void (TClass::*Delegate)(const CWindow*), TClass& Instance)
		: EventDelegate(bind(Delegate, &Instance, ::std::placeholders::_1))
	{
	}

	void operator()(const CWindow* pSender) const
	{
		EventDelegate(pSender);
	}
};

struct CEventHandlerListBase
{
	friend class CWindow;

	virtual ~CEventHandlerListBase()
	{
	}

	virtual bool PushBack(CEventHandlerBase&& Handler) = 0;
	virtual LRESULT Call(const CWindow& Window, WPARAM wParam, LPARAM lParam) = 0;
};

template <typename TEventArgs = CEventArgs>
struct CEventHandlerList : public CEventHandlerListBase
{
	::std::shared_ptr<::std::list<CEventHandler<TEventArgs>>> spList;

	CEventHandlerList()
	{
		spList = ::std::make_shared<::std::list<CEventHandler<TEventArgs>>>();
	}

	bool PushBack(CEventHandlerBase&& Handler) override
	{
		bool bPushedBack = false;
		if (auto pHandler = dynamic_cast<const CEventHandler<TEventArgs>*>(&Handler))
		{
			spList->emplace_back(move(*pHandler));
			bPushedBack = true;
		}
		ASSERT(bPushedBack);
		return bPushedBack;
	}

	LRESULT Call(const CWindow& Window, WPARAM wParam, LPARAM lParam) override
	{
		auto spCallList = spList;
		TEventArgs args(Window, wParam, lParam);
		for (auto& handler : *spCallList)
			handler(nullptr, args);

		return args;
	}
};

template <>
struct CEventHandlerList<CNoEventArgs> : public CEventHandlerListBase
{
	::std::shared_ptr<::std::list<CEventHandler<CNoEventArgs>>> spList;

	CEventHandlerList()
	{
		spList = ::std::make_shared<::std::list<CEventHandler<CNoEventArgs>>>();
	}

	bool PushBack(CEventHandlerBase&& Handler) override
	{
		bool bPushedBack = false;
		if (auto pHandler = dynamic_cast<const CEventHandler<CNoEventArgs>*>(&Handler))
		{
			spList->emplace_back(std::move(*pHandler));
			bPushedBack = true;
		}
		ASSERT(bPushedBack);
		return bPushedBack;
	}

	LRESULT Call(const CWindow& Window, WPARAM wParam, LPARAM lParam)
	{
		auto spCallList = spList;
		for (auto& handler : *spList)
			handler(nullptr);

		return true;
	}
};

typedef ::std::unordered_map<CEventID, ::std::unique_ptr<CEventHandlerListBase>, CEventIDHasher> CEventMap;

template <UINT MessageID, UINT SubMessageID, UINT ControlID, typename TEventArgs = CEventArgs>
struct CGenericEvent
{
	CEventMap& m_EventMap;
public:
	CGenericEvent(CEventMap& Map)
		: m_EventMap(Map)
	{
	}

	template <typename ...TArgs>
	CGenericEvent& operator +=(TArgs&&... Args)
	{
		unique_ptr<CEventHandlerListBase>& spList = m_EventMap[CEventID(MessageID, SubMessageID, ControlID)]; 
		if (!spList)
			spList.reset(new CEventHandlerList<TEventArgs>());
		spList->PushBack(CEventHandler<TEventArgs>(Args...));
		return *this;
	}
};

template <UINT MessageID, typename TEventArgs = CNoEventArgs>
using CEvent = CGenericEvent<MessageID, 0, 0, TEventArgs>;

template <UINT CommandID, UINT ControlID, typename TEventArgs = CNoEventArgs>
using CCommandEvent = CGenericEvent<WM_COMMAND, CommandID, ControlID, TEventArgs>;

template <UINT ButtonID>
using CButtonPressEvent = CCommandEvent<BN_CLICKED, ButtonID, CNoEventArgs>;

template <UINT NotifyCode, UINT ControlID, typename TEventArgs = CNoEventArgs>
using CNotifyEvent = CGenericEvent<WM_NOTIFY, NotifyCode, ControlID, TEventArgs>;

template <UINT NotifyCode, typename TEventArgs = CNoEventArgs>
using CNotifyReflectEvent = CGenericEvent<WM_NOTIFY_REFLECT, NotifyCode, 0, TEventArgs>;

template <UINT ControlCode, typename TEventArgs = CNoEventArgs>
using CControlReflectEvent = CGenericEvent<WM_CONTROL_REFLECT, ControlCode, 0, TEventArgs>;

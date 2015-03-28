#pragma once
#include "Traits.h"

template <typename ClassType, typename HandleType>
class CDrawingObjectHelper : public ClassType
{
	bool m_bDelete = false;
	HandleType m_Handle = nullptr;
	non_copyable copy_trait;

	bool Attach(HandleType Handle) override
	{
		Detach();
		m_Handle = Handle;
		m_bDelete = m_Handle ? true : false;
		return m_Handle ? true : false;
	}

	void Detach() override
	{
		if (m_bDelete && m_Handle)
			::DeleteObject(m_Handle);
		m_Handle = nullptr;
		m_bDelete = false;
	}
public:
	typedef HandleType ObjectHandleType;
	CDrawingObjectHelper()
		: ClassType(m_Handle)
	{
	}

	CDrawingObjectHelper(HandleType Handle)
		: ClassType(m_Handle),
		m_Handle(Handle)
	{
	}

	CDrawingObjectHelper(CDrawingObjectHelper&& Other)
	{
		*this = move(Other);
	}

	~CDrawingObjectHelper()
	{
		Detach();
	}

	CDrawingObjectHelper& operator=(CDrawingObjectHelper&& Other)
	{
		Detach();
		swap(m_bDelete, Other.m_bDelete);
		swap(m_Handle, Other.m_Handle);
		return *this;
	}

	explicit operator HandleType() const
	{
		return m_Handle;
	}

	operator bool() const
	{
		return m_Handle ? true : false;
	}
};
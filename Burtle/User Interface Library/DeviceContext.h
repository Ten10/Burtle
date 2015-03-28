#pragma once
#include "Traits.h"
#include "HandleMap.h"
#include "Brush.h"

struct CRectangle;
class CDeviceContext
{
public:
	typedef CHandleMap<HDC, CDeviceContext> CDeviceContextMap;
private:
	non_copyable copy_trait;
protected:
	HDC m_hDC = nullptr;

	void Attach(HDC hDC);
	void Detach();
public:
	CDeviceContext();
	virtual ~CDeviceContext() { };

	void SetHandle(HDC hDC) { m_hDC = hDC; }
	operator HDC() const { return m_hDC; }

	void Draw(int X, int Y, HICON hIcon);
	void Draw(const ::std::wstring& Text, CRectangle& Rect, UINT Format);
	void Draw(const ::std::string& Text, CRectangle& Rect, UINT Format);
	void Fill(const CRectangle& Rect, CBrush& Brush);

	template <typename T>
	T SelectObject(T& NewObject)
	{
		return (T::ObjectHandleType)::SelectObject(m_hDC, (T::ObjectHandleType)NewObject);
	}

	static CDeviceContext* FromHandle(HDC hDC);
	static __declspec(thread) CDeviceContextMap* pThreadDeviceContextMap;
};

typedef CDeviceContext CDC;

class CWindow;
class CPaintDeviceContext : public CDC
{
	const CWindow& m_Window;
	PAINTSTRUCT m_PaintInfo;
public:
	CPaintDeviceContext(const CWindow& Window);
	~CPaintDeviceContext();
};

typedef CPaintDeviceContext CPaintDC;
#include "stdafx.h"
#include "DeviceContext.h"

#include "Window.h"

__declspec(thread) UINT nThreadDeviceContextMapRefCount = 0;
CDeviceContext::CDeviceContextMap* CDeviceContext::pThreadDeviceContextMap = nullptr;

void CDeviceContext::Attach(HDC hDC)
{
	if (!hDC)
		return;

	if (!pThreadDeviceContextMap)
		pThreadDeviceContextMap = new CDeviceContextMap();

	++nThreadDeviceContextMapRefCount;

	Detach();
	m_hDC = hDC;
	pThreadDeviceContextMap->Add(make_pair(m_hDC, this));
}

void CDeviceContext::Detach()
{
	if (!m_hDC)
		return;

	if (pThreadDeviceContextMap)
		pThreadDeviceContextMap->Remove(m_hDC);

	--nThreadDeviceContextMapRefCount;
	if (!nThreadDeviceContextMapRefCount)
	{
		delete pThreadDeviceContextMap;
		pThreadDeviceContextMap = nullptr;
	}

	m_hDC = nullptr;
}

CDeviceContext::CDeviceContext()
{

}

void CDeviceContext::Draw(int X, int Y, HICON hIcon)
{
	::DrawIcon(m_hDC, X, Y, hIcon);
}

void CDeviceContext::Draw(const ::std::wstring& Text, CRect& Rect, UINT Format)
{
	::DrawTextW(m_hDC, Text.c_str(), (int)Text.length(), &Rect, Format);
}

void CDeviceContext::Draw(const ::std::string& Text, CRect& Rect, UINT Format)
{
	::DrawTextA(m_hDC, Text.c_str(), (int)Text.length(), &Rect, Format);
}

void CDeviceContext::Fill(const CRectangle& Rect, CBrush& Brush)
{
	::FillRect(m_hDC, &Rect, static_cast<HBRUSH>(Brush));
}

CDeviceContext* CDeviceContext::FromHandle(HDC hDC)
{
	if (!pThreadDeviceContextMap)
		pThreadDeviceContextMap = new CDeviceContextMap();
	++nThreadDeviceContextMapRefCount;

	return pThreadDeviceContextMap->FromHandle(hDC);
}

CPaintDeviceContext::CPaintDeviceContext(const CWindow& Window)
	: m_Window(Window)
{
	m_hDC = ::BeginPaint(m_Window, &m_PaintInfo);
}

CPaintDeviceContext::~CPaintDeviceContext()
{
	::EndPaint(m_Window, &m_PaintInfo);
}
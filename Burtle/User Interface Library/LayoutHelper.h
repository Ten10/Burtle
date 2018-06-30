#pragma once
#include <map>
#include "Rectangle.h"
#include "Traits.h"

class CLayoutHelper
{
	struct CControlInfo
	{
		HWND ControlWindow;
		CRect ControlMarginRect;
	};

	HWND m_Window;
	CScreenRect m_WindowRect;
	::std::map<UINT, CControlInfo> m_ControlIDToControlInfo;
public:
	class CLayoutInitializer
	{
	public:
		enum class eLayoutFlags
		{
			None = 0,
			Left = (1 << 0),
			Right = (1 << 1),
			Top = (1 << 2),
			Bottom = (1 << 3),
			LeftRight = Left | Right,
			BottomRight = Bottom | Right,
			All = Left | Right | Top | Bottom
		};
	private:
		non_copyable m_CopyTrait;
		CLayoutHelper& m_LayoutHelper;
		::std::map<UINT, eLayoutFlags> m_ControlIDToLayoutFlags;
	public:
		CLayoutInitializer(CLayoutHelper& LayoutHelper);
		~CLayoutInitializer();
		CLayoutInitializer(CLayoutInitializer&& Other);

		void AddControl(UINT ControlID, eLayoutFlags LayoutFlags);
	};

	CLayoutInitializer InitializeLayout(HWND hWindow);
	bool OnLayoutChanged();
};
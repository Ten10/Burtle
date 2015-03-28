#include "stdafx.h"
#include "Window.h"
#include "Exceptions.h"
#include "DeviceContext.h"
#include "Rectangle.h"

#include <CommCtrl.h>

#pragma comment(lib,"Comctl32.lib")

__declspec(thread) UINT nThreadWindowMapRefCount = 0;
__declspec(thread) CWindow::CWindowMap* pThreadWindowMap = nullptr;

static ::std::map<UINT, ::std::wstring> MessageIDToMessageName = {
		{ 0x0000, _T("WM_NULL") },
		{ 0x0001, _T("WM_CREATE") },
		{ 0x0002, _T("WM_DESTROY") },
		{ 0x0003, _T("WM_MOVE") },
		{ 0x0005, _T("WM_SIZE") },
		{ 0x0006, _T("WM_ACTIVATE") },
		{ 0x0007, _T("WM_SETFOCUS") },
		{ 0x0008, _T("WM_KILLFOCUS") },
		{ 0x000a, _T("WM_ENABLE") },
		{ 0x000b, _T("WM_SETREDRAW") },
		{ 0x000c, _T("WM_SETTEXT") },
		{ 0x000d, _T("WM_GETTEXT") },
		{ 0x000e, _T("WM_GETTEXTLENGTH") },
		{ 0x000f, _T("WM_PAINT") },
		{ 0x0010, _T("WM_CLOSE") },
		{ 0x0011, _T("WM_QUERYENDSESSION") },
		{ 0x0012, _T("WM_QUIT") },
		{ 0x0013, _T("WM_QUERYOPEN") },
		{ 0x0014, _T("WM_ERASEBKGND") },
		{ 0x0015, _T("WM_SYSCOLORCHANGE") },
		{ 0x0016, _T("WM_ENDSESSION") },
		{ 0x0018, _T("WM_SHOWWINDOW") },
		{ 0x0019, _T("WM_CTLCOLOR") },
		{ 0x001a, _T("WM_WININICHANGE") },
		{ 0x001b, _T("WM_DEVMODECHANGE") },
		{ 0x001c, _T("WM_ACTIVATEAPP") },
		{ 0x001d, _T("WM_FONTCHANGE") },
		{ 0x001e, _T("WM_TIMECHANGE") },
		{ 0x001f, _T("WM_CANCELMODE") },
		{ 0x0020, _T("WM_SETCURSOR") },
		{ 0x0021, _T("WM_MOUSEACTIVATE") },
		{ 0x0022, _T("WM_CHILDACTIVATE") },
		{ 0x0023, _T("WM_QUEUESYNC") },
		{ 0x0024, _T("WM_GETMINMAXINFO") },
		{ 0x0026, _T("WM_PAINTICON") },
		{ 0x0027, _T("WM_ICONERASEBKGND") },
		{ 0x0028, _T("WM_NEXTDLGCTL") },
		{ 0x002a, _T("WM_SPOOLERSTATUS") },
		{ 0x002b, _T("WM_DRAWITEM") },
		{ 0x002c, _T("WM_MEASUREITEM") },
		{ 0x002d, _T("WM_DELETEITEM") },
		{ 0x002e, _T("WM_VKEYTOITEM") },
		{ 0x002f, _T("WM_CHARTOITEM") },
		{ 0x0030, _T("WM_SETFONT") },
		{ 0x0031, _T("WM_GETFONT") },
		{ 0x0032, _T("WM_SETHOTKEY") },
		{ 0x0033, _T("WM_GETHOTKEY") },
		{ 0x0037, _T("WM_QUERYDRAGICON") },
		{ 0x0039, _T("WM_COMPAREITEM") },
		{ 0x003d, _T("WM_GETOBJECT") },
		{ 0x0041, _T("WM_COMPACTING") },
		{ 0x0044, _T("WM_COMMNOTIFY") },
		{ 0x0046, _T("WM_WINDOWPOSCHANGING") },
		{ 0x0047, _T("WM_WINDOWPOSCHANGED") },
		{ 0x0048, _T("WM_POWER") },
		{ 0x0049, _T("WM_COPYGLOBALDATA") },
		{ 0x004a, _T("WM_COPYDATA") },
		{ 0x004b, _T("WM_CANCELJOURNAL") },
		{ 0x004e, _T("WM_NOTIFY") },
		{ 0x0050, _T("WM_INPUTLANGCHANGEREQUEST") },
		{ 0x0051, _T("WM_INPUTLANGCHANGE") },
		{ 0x0052, _T("WM_TCARD") },
		{ 0x0053, _T("WM_HELP") },
		{ 0x0054, _T("WM_USERCHANGED") },
		{ 0x0055, _T("WM_NOTIFYFORMAT") },
		{ 0x007b, _T("WM_CONTEXTMENU") },
		{ 0x007c, _T("WM_STYLECHANGING") },
		{ 0x007d, _T("WM_STYLECHANGED") },
		{ 0x007e, _T("WM_DISPLAYCHANGE") },
		{ 0x007f, _T("WM_GETICON") },
		{ 0x0080, _T("WM_SETICON") },
		{ 0x0081, _T("WM_NCCREATE") },
		{ 0x0082, _T("WM_NCDESTROY") },
		{ 0x0083, _T("WM_NCCALCSIZE") },
		{ 0x0084, _T("WM_NCHITTEST") },
		{ 0x0085, _T("WM_NCPAINT") },
		{ 0x0086, _T("WM_NCACTIVATE") },
		{ 0x0087, _T("WM_GETDLGCODE") },
		{ 0x0088, _T("WM_SYNCPAINT") },
		{ 0x00a0, _T("WM_NCMOUSEMOVE") },
		{ 0x00a1, _T("WM_NCLBUTTONDOWN") },
		{ 0x00a2, _T("WM_NCLBUTTONUP") },
		{ 0x00a3, _T("WM_NCLBUTTONDBLCLK") },
		{ 0x00a4, _T("WM_NCRBUTTONDOWN") },
		{ 0x00a5, _T("WM_NCRBUTTONUP") },
		{ 0x00a6, _T("WM_NCRBUTTONDBLCLK") },
		{ 0x00a7, _T("WM_NCMBUTTONDOWN") },
		{ 0x00a8, _T("WM_NCMBUTTONUP") },
		{ 0x00a9, _T("WM_NCMBUTTONDBLCLK") },
		{ 0x00ab, _T("WM_NCXBUTTONDOWN") },
		{ 0x00ac, _T("WM_NCXBUTTONUP") },
		{ 0x00ad, _T("WM_NCXBUTTONDBLCLK") },
		{ 0x00e0, _T("SBM_SETPOS") },
		{ 0x00e1, _T("SBM_GETPOS") },
		{ 0x00e2, _T("SBM_SETRANGE") },
		{ 0x00e3, _T("SBM_GETRANGE") },
		{ 0x00e4, _T("SBM_ENABLE_ARROWS") },
		{ 0x00e6, _T("SBM_SETRANGEREDRAW") },
		{ 0x00e9, _T("SBM_SETSCROLLINFO") },
		{ 0x00ea, _T("SBM_GETSCROLLINFO") },
		{ 0x00eb, _T("SBM_GETSCROLLBARINFO") },
		{ 0x00ff, _T("WM_INPUT") },
		{ 0x0100, _T("WM_KEYDOWN") },
		{ 0x0100, _T("WM_KEYFIRST") },
		{ 0x0101, _T("WM_KEYUP") },
		{ 0x0102, _T("WM_CHAR") },
		{ 0x0103, _T("WM_DEADCHAR") },
		{ 0x0104, _T("WM_SYSKEYDOWN") },
		{ 0x0105, _T("WM_SYSKEYUP") },
		{ 0x0106, _T("WM_SYSCHAR") },
		{ 0x0107, _T("WM_SYSDEADCHAR") },
		{ 0x0108, _T("WM_KEYLAST") },
		{ 0x0109, _T("WM_WNT_CONVERTREQUESTEX") },
		{ 0x010a, _T("WM_CONVERTREQUEST") },
		{ 0x010b, _T("WM_CONVERTRESULT") },
		{ 0x010c, _T("WM_INTERIM") },
		{ 0x010d, _T("WM_IME_STARTCOMPOSITION") },
		{ 0x010e, _T("WM_IME_ENDCOMPOSITION") },
		{ 0x010f, _T("WM_IME_COMPOSITION") },
		{ 0x010f, _T("WM_IME_KEYLAST") },
		{ 0x0110, _T("WM_INITDIALOG") },
		{ 0x0111, _T("WM_COMMAND") },
		{ 0x0112, _T("WM_SYSCOMMAND") },
		{ 0x0113, _T("WM_TIMER") },
		{ 0x0114, _T("WM_HSCROLL") },
		{ 0x0115, _T("WM_VSCROLL") },
		{ 0x0116, _T("WM_INITMENU") },
		{ 0x0117, _T("WM_INITMENUPOPUP") },
		{ 0x0118, _T("WM_SYSTIMER") },
		{ 0x011f, _T("WM_MENUSELECT") },
		{ 0x0120, _T("WM_MENUCHAR") },
		{ 0x0121, _T("WM_ENTERIDLE") },
		{ 0x0122, _T("WM_MENURBUTTONUP") },
		{ 0x0123, _T("WM_MENUDRAG") },
		{ 0x0124, _T("WM_MENUGETOBJECT") },
		{ 0x0125, _T("WM_UNINITMENUPOPUP") },
		{ 0x0126, _T("WM_MENUCOMMAND") },
		{ 0x0127, _T("WM_CHANGEUISTATE") },
		{ 0x0128, _T("WM_UPDATEUISTATE") },
		{ 0x0129, _T("WM_QUERYUISTATE") },
		{ 0x0132, _T("WM_CTLCOLORMSGBOX") },
		{ 0x0133, _T("WM_CTLCOLOREDIT") },
		{ 0x0134, _T("WM_CTLCOLORLISTBOX") },
		{ 0x0135, _T("WM_CTLCOLORBTN") },
		{ 0x0136, _T("WM_CTLCOLORDLG") },
		{ 0x0137, _T("WM_CTLCOLORSCROLLBAR") },
		{ 0x0138, _T("WM_CTLCOLORSTATIC") },
		{ 0x0200, _T("WM_MOUSEFIRST") },
		{ 0x0200, _T("WM_MOUSEMOVE") },
		{ 0x0201, _T("WM_LBUTTONDOWN") },
		{ 0x0202, _T("WM_LBUTTONUP") },
		{ 0x0203, _T("WM_LBUTTONDBLCLK") },
		{ 0x0204, _T("WM_RBUTTONDOWN") },
		{ 0x0205, _T("WM_RBUTTONUP") },
		{ 0x0206, _T("WM_RBUTTONDBLCLK") },
		{ 0x0207, _T("WM_MBUTTONDOWN") },
		{ 0x0208, _T("WM_MBUTTONUP") },
		{ 0x0209, _T("WM_MBUTTONDBLCLK") },
		{ 0x0209, _T("WM_MOUSELAST") },
		{ 0x020a, _T("WM_MOUSEWHEEL") },
		{ 0x020b, _T("WM_XBUTTONDOWN") },
		{ 0x020c, _T("WM_XBUTTONUP") },
		{ 0x020d, _T("WM_XBUTTONDBLCLK") },
		{ 0x0210, _T("WM_PARENTNOTIFY") },
		{ 0x0211, _T("WM_ENTERMENULOOP") },
		{ 0x0212, _T("WM_EXITMENULOOP") },
		{ 0x0213, _T("WM_NEXTMENU") },
		{ 0x0214, _T("WM_SIZING") },
		{ 0x0215, _T("WM_CAPTURECHANGED") },
		{ 0x0216, _T("WM_MOVING") },
		{ 0x0218, _T("WM_POWERBROADCAST") },
		{ 0x0219, _T("WM_DEVICECHANGE") },
		{ 0x0220, _T("WM_MDICREATE") },
		{ 0x0221, _T("WM_MDIDESTROY") },
		{ 0x0222, _T("WM_MDIACTIVATE") },
		{ 0x0223, _T("WM_MDIRESTORE") },
		{ 0x0224, _T("WM_MDINEXT") },
		{ 0x0225, _T("WM_MDIMAXIMIZE") },
		{ 0x0226, _T("WM_MDITILE") },
		{ 0x0227, _T("WM_MDICASCADE") },
		{ 0x0228, _T("WM_MDIICONARRANGE") },
		{ 0x0229, _T("WM_MDIGETACTIVE") },
		{ 0x0230, _T("WM_MDISETMENU") },
		{ 0x0231, _T("WM_ENTERSIZEMOVE") },
		{ 0x0232, _T("WM_EXITSIZEMOVE") },
		{ 0x0233, _T("WM_DROPFILES") },
		{ 0x0234, _T("WM_MDIREFRESHMENU") },
		{ 0x0280, _T("WM_IME_REPORT") },
		{ 0x0281, _T("WM_IME_SETCONTEXT") },
		{ 0x0282, _T("WM_IME_NOTIFY") },
		{ 0x0283, _T("WM_IME_CONTROL") },
		{ 0x0284, _T("WM_IME_COMPOSITIONFULL") },
		{ 0x0285, _T("WM_IME_SELECT") },
		{ 0x0286, _T("WM_IME_CHAR") },
		{ 0x0288, _T("WM_IME_REQUEST") },
		{ 0x0290, _T("WM_IMEKEYDOWN") },
		{ 0x0290, _T("WM_IME_KEYDOWN") },
		{ 0x0291, _T("WM_IMEKEYUP") },
		{ 0x0291, _T("WM_IME_KEYUP") },
		{ 0x02a0, _T("WM_NCMOUSEHOVER") },
		{ 0x02a1, _T("WM_MOUSEHOVER") },
		{ 0x02a2, _T("WM_NCMOUSELEAVE") },
		{ 0x02a3, _T("WM_MOUSELEAVE") },
		{ 0x0300, _T("WM_CUT") },
		{ 0x0301, _T("WM_COPY") },
		{ 0x0302, _T("WM_PASTE") },
		{ 0x0303, _T("WM_CLEAR") },
		{ 0x0304, _T("WM_UNDO") },
		{ 0x0305, _T("WM_RENDERFORMAT") },
		{ 0x0306, _T("WM_RENDERALLFORMATS") },
		{ 0x0307, _T("WM_DESTROYCLIPBOARD") },
		{ 0x0308, _T("WM_DRAWCLIPBOARD") },
		{ 0x0309, _T("WM_PAINTCLIPBOARD") },
		{ 0x030a, _T("WM_VSCROLLCLIPBOARD") },
		{ 0x030b, _T("WM_SIZECLIPBOARD") },
		{ 0x030c, _T("WM_ASKCBFORMATNAME") },
		{ 0x030d, _T("WM_CHANGECBCHAIN") },
		{ 0x030e, _T("WM_HSCROLLCLIPBOARD") },
		{ 0x030f, _T("WM_QUERYNEWPALETTE") },
		{ 0x0310, _T("WM_PALETTEISCHANGING") },
		{ 0x0311, _T("WM_PALETTECHANGED") },
		{ 0x0312, _T("WM_HOTKEY") },
		{ 0x0317, _T("WM_PRINT") },
		{ 0x0318, _T("WM_PRINTCLIENT") },
		{ 0x0319, _T("WM_APPCOMMAND") },
		{ 0x0358, _T("WM_HANDHELDFIRST") },
		{ 0x035f, _T("WM_HANDHELDLAST") },
		{ 0x0360, _T("WM_AFXFIRST") },
		{ 0x037f, _T("WM_AFXLAST") },
		{ 0x0380, _T("WM_PENWINFIRST") },
		{ 0x0381, _T("WM_RCRESULT") },
		{ 0x0382, _T("WM_HOOKRCRESULT") },
		{ 0x0383, _T("WM_GLOBALRCCHANGE") },
		{ 0x0383, _T("WM_PENMISCINFO") },
		{ 0x0384, _T("WM_SKB") },
		{ 0x0385, _T("WM_HEDITCTL") },
		{ 0x0385, _T("WM_PENCTL") },
		{ 0x0386, _T("WM_PENMISC") },
		{ 0x0387, _T("WM_CTLINIT") },
		{ 0x0388, _T("WM_PENEVENT") },
		{ 0x038f, _T("WM_PENWINLAST") },
		{ 0x0400, _T("DDM_SETFMT") },
		{ 0x0400, _T("DM_GETDEFID") },
		{ 0x0400, _T("NIN_SELECT") },
		{ 0x0400, _T("TBM_GETPOS") },
		{ 0x0400, _T("WM_PSD_PAGESETUPDLG") },
		{ 0x0400, _T("WM_USER") },
		{ 0x0401, _T("CBEM_INSERTITEMA") },
		{ 0x0401, _T("DDM_DRAW") },
		{ 0x0401, _T("DM_SETDEFID") },
		{ 0x0401, _T("HKM_SETHOTKEY") },
		{ 0x0401, _T("PBM_SETRANGE") },
		{ 0x0401, _T("RB_INSERTBANDA") },
		{ 0x0401, _T("SB_SETTEXTA") },
		{ 0x0401, _T("TB_ENABLEBUTTON") },
		{ 0x0401, _T("TBM_GETRANGEMIN") },
		{ 0x0401, _T("TTM_ACTIVATE") },
		{ 0x0401, _T("WM_CHOOSEFONT_GETLOGFONT") },
		{ 0x0401, _T("WM_PSD_FULLPAGERECT") },
		{ 0x0402, _T("CBEM_SETIMAGELIST") },
		{ 0x0402, _T("DDM_CLOSE") },
		{ 0x0402, _T("DM_REPOSITION") },
		{ 0x0402, _T("HKM_GETHOTKEY") },
		{ 0x0402, _T("PBM_SETPOS") },
		{ 0x0402, _T("RB_DELETEBAND") },
		{ 0x0402, _T("SB_GETTEXTA") },
		{ 0x0402, _T("TB_CHECKBUTTON") },
		{ 0x0402, _T("TBM_GETRANGEMAX") },
		{ 0x0402, _T("WM_PSD_MINMARGINRECT") },
		{ 0x0403, _T("CBEM_GETIMAGELIST") },
		{ 0x0403, _T("DDM_BEGIN") },
		{ 0x0403, _T("HKM_SETRULES") },
		{ 0x0403, _T("PBM_DELTAPOS") },
		{ 0x0403, _T("RB_GETBARINFO") },
		{ 0x0403, _T("SB_GETTEXTLENGTHA") },
		{ 0x0403, _T("TBM_GETTIC") },
		{ 0x0403, _T("TB_PRESSBUTTON") },
		{ 0x0403, _T("TTM_SETDELAYTIME") },
		{ 0x0403, _T("WM_PSD_MARGINRECT") },
		{ 0x0404, _T("CBEM_GETITEMA") },
		{ 0x0404, _T("DDM_END") },
		{ 0x0404, _T("PBM_SETSTEP") },
		{ 0x0404, _T("RB_SETBARINFO") },
		{ 0x0404, _T("SB_SETPARTS") },
		{ 0x0404, _T("TB_HIDEBUTTON") },
		{ 0x0404, _T("TBM_SETTIC") },
		{ 0x0404, _T("TTM_ADDTOOLA") },
		{ 0x0404, _T("WM_PSD_GREEKTEXTRECT") },
		{ 0x0405, _T("CBEM_SETITEMA") },
		{ 0x0405, _T("PBM_STEPIT") },
		{ 0x0405, _T("TB_INDETERMINATE") },
		{ 0x0405, _T("TBM_SETPOS") },
		{ 0x0405, _T("TTM_DELTOOLA") },
		{ 0x0405, _T("WM_PSD_ENVSTAMPRECT") },
		{ 0x0406, _T("CBEM_GETCOMBOCONTROL") },
		{ 0x0406, _T("PBM_SETRANGE32") },
		{ 0x0406, _T("RB_SETBANDINFOA") },
		{ 0x0406, _T("SB_GETPARTS") },
		{ 0x0406, _T("TB_MARKBUTTON") },
		{ 0x0406, _T("TBM_SETRANGE") },
		{ 0x0406, _T("TTM_NEWTOOLRECTA") },
		{ 0x0406, _T("WM_PSD_YAFULLPAGERECT") },
		{ 0x0407, _T("CBEM_GETEDITCONTROL") },
		{ 0x0407, _T("PBM_GETRANGE") },
		{ 0x0407, _T("RB_SETPARENT") },
		{ 0x0407, _T("SB_GETBORDERS") },
		{ 0x0407, _T("TBM_SETRANGEMIN") },
		{ 0x0407, _T("TTM_RELAYEVENT") },
		{ 0x0408, _T("CBEM_SETEXSTYLE") },
		{ 0x0408, _T("PBM_GETPOS") },
		{ 0x0408, _T("RB_HITTEST") },
		{ 0x0408, _T("SB_SETMINHEIGHT") },
		{ 0x0408, _T("TBM_SETRANGEMAX") },
		{ 0x0408, _T("TTM_GETTOOLINFOA") },
		{ 0x0409, _T("CBEM_GETEXSTYLE") },
		{ 0x0409, _T("CBEM_GETEXTENDEDSTYLE") },
		{ 0x0409, _T("PBM_SETBARCOLOR") },
		{ 0x0409, _T("RB_GETRECT") },
		{ 0x0409, _T("SB_SIMPLE") },
		{ 0x0409, _T("TB_ISBUTTONENABLED") },
		{ 0x0409, _T("TBM_CLEARTICS") },
		{ 0x0409, _T("TTM_SETTOOLINFOA") },
		{ 0x040a, _T("CBEM_HASEDITCHANGED") },
		{ 0x040a, _T("RB_INSERTBANDW") },
		{ 0x040a, _T("SB_GETRECT") },
		{ 0x040a, _T("TB_ISBUTTONCHECKED") },
		{ 0x040a, _T("TBM_SETSEL") },
		{ 0x040a, _T("TTM_HITTESTA") },
		{ 0x040a, _T("WIZ_QUERYNUMPAGES") },
		{ 0x040b, _T("CBEM_INSERTITEMW") },
		{ 0x040b, _T("RB_SETBANDINFOW") },
		{ 0x040b, _T("SB_SETTEXTW") },
		{ 0x040b, _T("TB_ISBUTTONPRESSED") },
		{ 0x040b, _T("TBM_SETSELSTART") },
		{ 0x040b, _T("TTM_GETTEXTA") },
		{ 0x040b, _T("WIZ_NEXT") },
		{ 0x040c, _T("CBEM_SETITEMW") },
		{ 0x040c, _T("RB_GETBANDCOUNT") },
		{ 0x040c, _T("SB_GETTEXTLENGTHW") },
		{ 0x040c, _T("TB_ISBUTTONHIDDEN") },
		{ 0x040c, _T("TBM_SETSELEND") },
		{ 0x040c, _T("TTM_UPDATETIPTEXTA") },
		{ 0x040c, _T("WIZ_PREV") },
		{ 0x040d, _T("CBEM_GETITEMW") },
		{ 0x040d, _T("RB_GETROWCOUNT") },
		{ 0x040d, _T("SB_GETTEXTW") },
		{ 0x040d, _T("TB_ISBUTTONINDETERMINATE") },
		{ 0x040d, _T("TTM_GETTOOLCOUNT") },
		{ 0x040e, _T("CBEM_SETEXTENDEDSTYLE") },
		{ 0x040e, _T("RB_GETROWHEIGHT") },
		{ 0x040e, _T("SB_ISSIMPLE") },
		{ 0x040e, _T("TB_ISBUTTONHIGHLIGHTED") },
		{ 0x040e, _T("TBM_GETPTICS") },
		{ 0x040e, _T("TTM_ENUMTOOLSA") },
		{ 0x040f, _T("SB_SETICON") },
		{ 0x040f, _T("TBM_GETTICPOS") },
		{ 0x040f, _T("TTM_GETCURRENTTOOLA") },
		{ 0x0410, _T("RB_IDTOINDEX") },
		{ 0x0410, _T("SB_SETTIPTEXTA") },
		{ 0x0410, _T("TBM_GETNUMTICS") },
		{ 0x0410, _T("TTM_WINDOWFROMPOINT") },
		{ 0x0411, _T("RB_GETTOOLTIPS") },
		{ 0x0411, _T("SB_SETTIPTEXTW") },
		{ 0x0411, _T("TBM_GETSELSTART") },
		{ 0x0411, _T("TB_SETSTATE") },
		{ 0x0411, _T("TTM_TRACKACTIVATE") },
		{ 0x0412, _T("RB_SETTOOLTIPS") },
		{ 0x0412, _T("SB_GETTIPTEXTA") },
		{ 0x0412, _T("TB_GETSTATE") },
		{ 0x0412, _T("TBM_GETSELEND") },
		{ 0x0412, _T("TTM_TRACKPOSITION") },
		{ 0x0413, _T("RB_SETBKCOLOR") },
		{ 0x0413, _T("SB_GETTIPTEXTW") },
		{ 0x0413, _T("TB_ADDBITMAP") },
		{ 0x0413, _T("TBM_CLEARSEL") },
		{ 0x0413, _T("TTM_SETTIPBKCOLOR") },
		{ 0x0414, _T("RB_GETBKCOLOR") },
		{ 0x0414, _T("SB_GETICON") },
		{ 0x0414, _T("TB_ADDBUTTONSA") },
		{ 0x0414, _T("TBM_SETTICFREQ") },
		{ 0x0414, _T("TTM_SETTIPTEXTCOLOR") },
		{ 0x0415, _T("RB_SETTEXTCOLOR") },
		{ 0x0415, _T("TB_INSERTBUTTONA") },
		{ 0x0415, _T("TBM_SETPAGESIZE") },
		{ 0x0415, _T("TTM_GETDELAYTIME") },
		{ 0x0416, _T("RB_GETTEXTCOLOR") },
		{ 0x0416, _T("TB_DELETEBUTTON") },
		{ 0x0416, _T("TBM_GETPAGESIZE") },
		{ 0x0416, _T("TTM_GETTIPBKCOLOR") },
		{ 0x0417, _T("RB_SIZETORECT") },
		{ 0x0417, _T("TB_GETBUTTON") },
		{ 0x0417, _T("TBM_SETLINESIZE") },
		{ 0x0417, _T("TTM_GETTIPTEXTCOLOR") },
		{ 0x0418, _T("RB_BEGINDRAG") },
		{ 0x0418, _T("TB_BUTTONCOUNT") },
		{ 0x0418, _T("TBM_GETLINESIZE") },
		{ 0x0418, _T("TTM_SETMAXTIPWIDTH") },
		{ 0x0419, _T("RB_ENDDRAG") },
		{ 0x0419, _T("TB_COMMANDTOINDEX") },
		{ 0x0419, _T("TBM_GETTHUMBRECT") },
		{ 0x0419, _T("TTM_GETMAXTIPWIDTH") },
		{ 0x041a, _T("RB_DRAGMOVE") },
		{ 0x041a, _T("TBM_GETCHANNELRECT") },
		{ 0x041a, _T("TB_SAVERESTOREA") },
		{ 0x041a, _T("TTM_SETMARGIN") },
		{ 0x041b, _T("RB_GETBARHEIGHT") },
		{ 0x041b, _T("TB_CUSTOMIZE") },
		{ 0x041b, _T("TBM_SETTHUMBLENGTH") },
		{ 0x041b, _T("TTM_GETMARGIN") },
		{ 0x041c, _T("RB_GETBANDINFOW") },
		{ 0x041c, _T("TB_ADDSTRINGA") },
		{ 0x041c, _T("TBM_GETTHUMBLENGTH") },
		{ 0x041c, _T("TTM_POP") },
		{ 0x041d, _T("RB_GETBANDINFOA") },
		{ 0x041d, _T("TB_GETITEMRECT") },
		{ 0x041d, _T("TBM_SETTOOLTIPS") },
		{ 0x041d, _T("TTM_UPDATE") },
		{ 0x041e, _T("RB_MINIMIZEBAND") },
		{ 0x041e, _T("TB_BUTTONSTRUCTSIZE") },
		{ 0x041e, _T("TBM_GETTOOLTIPS") },
		{ 0x041e, _T("TTM_GETBUBBLESIZE") },
		{ 0x041f, _T("RB_MAXIMIZEBAND") },
		{ 0x041f, _T("TBM_SETTIPSIDE") },
		{ 0x041f, _T("TB_SETBUTTONSIZE") },
		{ 0x041f, _T("TTM_ADJUSTRECT") },
		{ 0x0420, _T("TBM_SETBUDDY") },
		{ 0x0420, _T("TB_SETBITMAPSIZE") },
		{ 0x0420, _T("TTM_SETTITLEA") },
		{ 0x0421, _T("MSG_FTS_JUMP_VA") },
		{ 0x0421, _T("TB_AUTOSIZE") },
		{ 0x0421, _T("TBM_GETBUDDY") },
		{ 0x0421, _T("TTM_SETTITLEW") },
		{ 0x0422, _T("RB_GETBANDBORDERS") },
		{ 0x0423, _T("MSG_FTS_JUMP_QWORD") },
		{ 0x0423, _T("RB_SHOWBAND") },
		{ 0x0423, _T("TB_GETTOOLTIPS") },
		{ 0x0424, _T("MSG_REINDEX_REQUEST") },
		{ 0x0424, _T("TB_SETTOOLTIPS") },
		{ 0x0425, _T("MSG_FTS_WHERE_IS_IT") },
		{ 0x0425, _T("RB_SETPALETTE") },
		{ 0x0425, _T("TB_SETPARENT") },
		{ 0x0426, _T("RB_GETPALETTE") },
		{ 0x0427, _T("RB_MOVEBAND") },
		{ 0x0427, _T("TB_SETROWS") },
		{ 0x0428, _T("TB_GETROWS") },
		{ 0x0429, _T("TB_GETBITMAPFLAGS") },
		{ 0x042a, _T("TB_SETCMDID") },
		{ 0x042b, _T("RB_PUSHCHEVRON") },
		{ 0x042b, _T("TB_CHANGEBITMAP") },
		{ 0x042c, _T("TB_GETBITMAP") },
		{ 0x042d, _T("MSG_GET_DEFFONT") },
		{ 0x042d, _T("TB_GETBUTTONTEXTA") },
		{ 0x042e, _T("TB_REPLACEBITMAP") },
		{ 0x042f, _T("TB_SETINDENT") },
		{ 0x0430, _T("TB_SETIMAGELIST") },
		{ 0x0431, _T("TB_GETIMAGELIST") },
		{ 0x0432, _T("TB_LOADIMAGES") },
		{ 0x0432, _T("TTM_ADDTOOLW") },
		{ 0x0433, _T("TB_GETRECT") },
		{ 0x0433, _T("TTM_DELTOOLW") },
		{ 0x0434, _T("TB_SETHOTIMAGELIST") },
		{ 0x0434, _T("TTM_NEWTOOLRECTW") },
		{ 0x0435, _T("TB_GETHOTIMAGELIST") },
		{ 0x0435, _T("TTM_GETTOOLINFOW") },
		{ 0x0436, _T("TB_SETDISABLEDIMAGELIST") },
		{ 0x0436, _T("TTM_SETTOOLINFOW") },
		{ 0x0437, _T("TB_GETDISABLEDIMAGELIST") },
		{ 0x0437, _T("TTM_HITTESTW") },
		{ 0x0438, _T("TB_SETSTYLE") },
		{ 0x0438, _T("TTM_GETTEXTW") },
		{ 0x0439, _T("TB_GETSTYLE") },
		{ 0x0439, _T("TTM_UPDATETIPTEXTW") },
		{ 0x043a, _T("TB_GETBUTTONSIZE") },
		{ 0x043a, _T("TTM_ENUMTOOLSW") },
		{ 0x043b, _T("TB_SETBUTTONWIDTH") },
		{ 0x043b, _T("TTM_GETCURRENTTOOLW") },
		{ 0x043c, _T("TB_SETMAXTEXTROWS") },
		{ 0x043d, _T("TB_GETTEXTROWS") },
		{ 0x043e, _T("TB_GETOBJECT") },
		{ 0x043f, _T("TB_GETBUTTONINFOW") },
		{ 0x0440, _T("TB_SETBUTTONINFOW") },
		{ 0x0441, _T("TB_GETBUTTONINFOA") },
		{ 0x0442, _T("TB_SETBUTTONINFOA") },
		{ 0x0443, _T("TB_INSERTBUTTONW") },
		{ 0x0444, _T("TB_ADDBUTTONSW") },
		{ 0x0445, _T("TB_HITTEST") },
		{ 0x0446, _T("TB_SETDRAWTEXTFLAGS") },
		{ 0x0447, _T("TB_GETHOTITEM") },
		{ 0x0448, _T("TB_SETHOTITEM") },
		{ 0x0449, _T("TB_SETANCHORHIGHLIGHT") },
		{ 0x044a, _T("TB_GETANCHORHIGHLIGHT") },
		{ 0x044b, _T("TB_GETBUTTONTEXTW") },
		{ 0x044c, _T("TB_SAVERESTOREW") },
		{ 0x044d, _T("TB_ADDSTRINGW") },
		{ 0x044e, _T("TB_MAPACCELERATORA") },
		{ 0x044f, _T("TB_GETINSERTMARK") },
		{ 0x0450, _T("TB_SETINSERTMARK") },
		{ 0x0451, _T("TB_INSERTMARKHITTEST") },
		{ 0x0452, _T("TB_MOVEBUTTON") },
		{ 0x0453, _T("TB_GETMAXSIZE") },
		{ 0x0454, _T("TB_SETEXTENDEDSTYLE") },
		{ 0x0455, _T("TB_GETEXTENDEDSTYLE") },
		{ 0x0456, _T("TB_GETPADDING") },
		{ 0x0457, _T("TB_SETPADDING") },
		{ 0x0458, _T("TB_SETINSERTMARKCOLOR") },
		{ 0x0459, _T("TB_GETINSERTMARKCOLOR") },
		{ 0x045a, _T("TB_MAPACCELERATORW") },
		{ 0x045b, _T("TB_GETSTRINGW") },
		{ 0x045c, _T("TB_GETSTRINGA") },
		{ 0x0463, _T("TAPI_REPLY") },
		{ 0x0464, _T("ACM_OPENA") },
		{ 0x0464, _T("BFFM_SETSTATUSTEXTA") },
		{ 0x0464, _T("CDM_FIRST") },
		{ 0x0464, _T("CDM_GETSPEC") },
		{ 0x0464, _T("IPM_CLEARADDRESS") },
		{ 0x0464, _T("WM_CAP_UNICODE_START") },
		{ 0x0465, _T("ACM_PLAY") },
		{ 0x0465, _T("BFFM_ENABLEOK") },
		{ 0x0465, _T("CDM_GETFILEPATH") },
		{ 0x0465, _T("IPM_SETADDRESS") },
		{ 0x0465, _T("PSM_SETCURSEL") },
		{ 0x0465, _T("UDM_SETRANGE") },
		{ 0x0465, _T("WM_CHOOSEFONT_SETLOGFONT") },
		{ 0x0466, _T("ACM_STOP") },
		{ 0x0466, _T("BFFM_SETSELECTIONA") },
		{ 0x0466, _T("CDM_GETFOLDERPATH") },
		{ 0x0466, _T("IPM_GETADDRESS") },
		{ 0x0466, _T("PSM_REMOVEPAGE") },
		{ 0x0466, _T("UDM_GETRANGE") },
		{ 0x0466, _T("WM_CAP_SET_CALLBACK_ERRORW") },
		{ 0x0466, _T("WM_CHOOSEFONT_SETFLAGS") },
		{ 0x0467, _T("ACM_OPENW") },
		{ 0x0467, _T("BFFM_SETSELECTIONW") },
		{ 0x0467, _T("CDM_GETFOLDERIDLIST") },
		{ 0x0467, _T("IPM_SETRANGE") },
		{ 0x0467, _T("PSM_ADDPAGE") },
		{ 0x0467, _T("UDM_SETPOS") },
		{ 0x0467, _T("WM_CAP_SET_CALLBACK_STATUSW") },
		{ 0x0468, _T("BFFM_SETSTATUSTEXTW") },
		{ 0x0468, _T("CDM_SETCONTROLTEXT") },
		{ 0x0468, _T("IPM_SETFOCUS") },
		{ 0x0468, _T("PSM_CHANGED") },
		{ 0x0468, _T("UDM_GETPOS") },
		{ 0x0469, _T("CDM_HIDECONTROL") },
		{ 0x0469, _T("IPM_ISBLANK") },
		{ 0x0469, _T("PSM_RESTARTWINDOWS") },
		{ 0x0469, _T("UDM_SETBUDDY") },
		{ 0x046a, _T("CDM_SETDEFEXT") },
		{ 0x046a, _T("PSM_REBOOTSYSTEM") },
		{ 0x046a, _T("UDM_GETBUDDY") },
		{ 0x046b, _T("PSM_CANCELTOCLOSE") },
		{ 0x046b, _T("UDM_SETACCEL") },
		{ 0x046c, _T("EM_CONVPOSITION") },
		{ 0x046c, _T("PSM_QUERYSIBLINGS") },
		{ 0x046c, _T("UDM_GETACCEL") },
		{ 0x046d, _T("MCIWNDM_GETZOOM") },
		{ 0x046d, _T("PSM_UNCHANGED") },
		{ 0x046d, _T("UDM_SETBASE") },
		{ 0x046e, _T("PSM_APPLY") },
		{ 0x046e, _T("UDM_GETBASE") },
		{ 0x046f, _T("PSM_SETTITLEA") },
		{ 0x046f, _T("UDM_SETRANGE32") },
		{ 0x0470, _T("PSM_SETWIZBUTTONS") },
		{ 0x0470, _T("UDM_GETRANGE32") },
		{ 0x0470, _T("WM_CAP_DRIVER_GET_NAMEW") },
		{ 0x0471, _T("PSM_PRESSBUTTON") },
		{ 0x0471, _T("UDM_SETPOS32") },
		{ 0x0471, _T("WM_CAP_DRIVER_GET_VERSIONW") },
		{ 0x0472, _T("PSM_SETCURSELID") },
		{ 0x0472, _T("UDM_GETPOS32") },
		{ 0x0473, _T("PSM_SETFINISHTEXTA") },
		{ 0x0474, _T("PSM_GETTABCONTROL") },
		{ 0x0475, _T("PSM_ISDIALOGMESSAGE") },
		{ 0x0476, _T("MCIWNDM_REALIZE") },
		{ 0x0476, _T("PSM_GETCURRENTPAGEHWND") },
		{ 0x0477, _T("MCIWNDM_SETTIMEFORMATA") },
		{ 0x0477, _T("PSM_INSERTPAGE") },
		{ 0x0478, _T("MCIWNDM_GETTIMEFORMATA") },
		{ 0x0478, _T("PSM_SETTITLEW") },
		{ 0x0478, _T("WM_CAP_FILE_SET_CAPTURE_FILEW") },
		{ 0x0479, _T("MCIWNDM_VALIDATEMEDIA") },
		{ 0x0479, _T("PSM_SETFINISHTEXTW") },
		{ 0x0479, _T("WM_CAP_FILE_GET_CAPTURE_FILEW") },
		{ 0x047b, _T("MCIWNDM_PLAYTO") },
		{ 0x047b, _T("WM_CAP_FILE_SAVEASW") },
		{ 0x047c, _T("MCIWNDM_GETFILENAMEA") },
		{ 0x047d, _T("MCIWNDM_GETDEVICEA") },
		{ 0x047d, _T("PSM_SETHEADERTITLEA") },
		{ 0x047d, _T("WM_CAP_FILE_SAVEDIBW") },
		{ 0x047e, _T("MCIWNDM_GETPALETTE") },
		{ 0x047e, _T("PSM_SETHEADERTITLEW") },
		{ 0x047f, _T("MCIWNDM_SETPALETTE") },
		{ 0x047f, _T("PSM_SETHEADERSUBTITLEA") },
		{ 0x0480, _T("MCIWNDM_GETERRORA") },
		{ 0x0480, _T("PSM_SETHEADERSUBTITLEW") },
		{ 0x0481, _T("PSM_HWNDTOINDEX") },
		{ 0x0482, _T("PSM_INDEXTOHWND") },
		{ 0x0483, _T("MCIWNDM_SETINACTIVETIMER") },
		{ 0x0483, _T("PSM_PAGETOINDEX") },
		{ 0x0484, _T("PSM_INDEXTOPAGE") },
		{ 0x0485, _T("DL_BEGINDRAG") },
		{ 0x0485, _T("MCIWNDM_GETINACTIVETIMER") },
		{ 0x0485, _T("PSM_IDTOINDEX") },
		{ 0x0486, _T("DL_DRAGGING") },
		{ 0x0486, _T("PSM_INDEXTOID") },
		{ 0x0487, _T("DL_DROPPED") },
		{ 0x0487, _T("PSM_GETRESULT") },
		{ 0x0488, _T("DL_CANCELDRAG") },
		{ 0x0488, _T("PSM_RECALCPAGESIZES") },
		{ 0x048c, _T("MCIWNDM_GET_SOURCE") },
		{ 0x048d, _T("MCIWNDM_PUT_SOURCE") },
		{ 0x048e, _T("MCIWNDM_GET_DEST") },
		{ 0x048f, _T("MCIWNDM_PUT_DEST") },
		{ 0x0490, _T("MCIWNDM_CAN_PLAY") },
		{ 0x0491, _T("MCIWNDM_CAN_WINDOW") },
		{ 0x0492, _T("MCIWNDM_CAN_RECORD") },
		{ 0x0493, _T("MCIWNDM_CAN_SAVE") },
		{ 0x0494, _T("MCIWNDM_CAN_EJECT") },
		{ 0x0495, _T("MCIWNDM_CAN_CONFIG") },
		{ 0x0496, _T("IE_GETINK") },
		{ 0x0496, _T("IE_MSGFIRST") },
		{ 0x0496, _T("MCIWNDM_PALETTEKICK") },
		{ 0x0497, _T("IE_SETINK") },
		{ 0x0498, _T("IE_GETPENTIP") },
		{ 0x0499, _T("IE_SETPENTIP") },
		{ 0x049a, _T("IE_GETERASERTIP") },
		{ 0x049b, _T("IE_SETERASERTIP") },
		{ 0x049c, _T("IE_GETBKGND") },
		{ 0x049d, _T("IE_SETBKGND") },
		{ 0x049e, _T("IE_GETGRIDORIGIN") },
		{ 0x049f, _T("IE_SETGRIDORIGIN") },
		{ 0x04a0, _T("IE_GETGRIDPEN") },
		{ 0x04a1, _T("IE_SETGRIDPEN") },
		{ 0x04a2, _T("IE_GETGRIDSIZE") },
		{ 0x04a3, _T("IE_SETGRIDSIZE") },
		{ 0x04a4, _T("IE_GETMODE") },
		{ 0x04a5, _T("IE_SETMODE") },
		{ 0x04a6, _T("IE_GETINKRECT") },
		{ 0x04a6, _T("WM_CAP_SET_MCI_DEVICEW") },
		{ 0x04a7, _T("WM_CAP_GET_MCI_DEVICEW") },
		{ 0x04b4, _T("WM_CAP_PAL_OPENW") },
		{ 0x04b5, _T("WM_CAP_PAL_SAVEW") },
		{ 0x04b8, _T("IE_GETAPPDATA") },
		{ 0x04b9, _T("IE_SETAPPDATA") },
		{ 0x04ba, _T("IE_GETDRAWOPTS") },
		{ 0x04bb, _T("IE_SETDRAWOPTS") },
		{ 0x04bc, _T("IE_GETFORMAT") },
		{ 0x04bd, _T("IE_SETFORMAT") },
		{ 0x04be, _T("IE_GETINKINPUT") },
		{ 0x04bf, _T("IE_SETINKINPUT") },
		{ 0x04c0, _T("IE_GETNOTIFY") },
		{ 0x04c1, _T("IE_SETNOTIFY") },
		{ 0x04c2, _T("IE_GETRECOG") },
		{ 0x04c3, _T("IE_SETRECOG") },
		{ 0x04c4, _T("IE_GETSECURITY") },
		{ 0x04c5, _T("IE_SETSECURITY") },
		{ 0x04c6, _T("IE_GETSEL") },
		{ 0x04c7, _T("IE_SETSEL") },
		{ 0x04c8, _T("CDM_LAST") },
		{ 0x04c8, _T("IE_DOCOMMAND") },
		{ 0x04c8, _T("MCIWNDM_NOTIFYMODE") },
		{ 0x04c9, _T("IE_GETCOMMAND") },
		{ 0x04ca, _T("IE_GETCOUNT") },
		{ 0x04cb, _T("IE_GETGESTURE") },
		{ 0x04cb, _T("MCIWNDM_NOTIFYMEDIA") },
		{ 0x04cc, _T("IE_GETMENU") },
		{ 0x04cd, _T("IE_GETPAINTDC") },
		{ 0x04cd, _T("MCIWNDM_NOTIFYERROR") },
		{ 0x04ce, _T("IE_GETPDEVENT") },
		{ 0x04cf, _T("IE_GETSELCOUNT") },
		{ 0x04d0, _T("IE_GETSELITEMS") },
		{ 0x04d1, _T("IE_GETSTYLE") },
		{ 0x04db, _T("MCIWNDM_SETTIMEFORMATW") },
		{ 0x04dc, _T("EM_OUTLINE") },
		{ 0x04dc, _T("MCIWNDM_GETTIMEFORMATW") },
		{ 0x04dd, _T("EM_GETSCROLLPOS") },
		{ 0x04de, _T("EM_SETSCROLLPOS") },
		{ 0x04df, _T("EM_SETFONTSIZE") },
		{ 0x04e0, _T("MCIWNDM_GETFILENAMEW") },
		{ 0x04e1, _T("MCIWNDM_GETDEVICEW") },
		{ 0x04e4, _T("MCIWNDM_GETERRORW") },
		{ 0x0600, _T("FM_GETFOCUS") },
		{ 0x0601, _T("FM_GETDRIVEINFOA") },
		{ 0x0602, _T("FM_GETSELCOUNT") },
		{ 0x0603, _T("FM_GETSELCOUNTLFN") },
		{ 0x0604, _T("FM_GETFILESELA") },
		{ 0x0605, _T("FM_GETFILESELLFNA") },
		{ 0x0606, _T("FM_REFRESH_WINDOWS") },
		{ 0x0607, _T("FM_RELOAD_EXTENSIONS") },
		{ 0x0611, _T("FM_GETDRIVEINFOW") },
		{ 0x0614, _T("FM_GETFILESELW") },
		{ 0x0615, _T("FM_GETFILESELLFNW") },
		{ 0x0659, _T("WLX_WM_SAS") },
		{ 0x07e8, _T("SM_GETSELCOUNT") },
		{ 0x07e8, _T("UM_GETSELCOUNT") },
		{ 0x07e8, _T("WM_CPL_LAUNCH") },
		{ 0x07e9, _T("SM_GETSERVERSELA") },
		{ 0x07e9, _T("UM_GETUSERSELA") },
		{ 0x07e9, _T("WM_CPL_LAUNCHED") },
		{ 0x07ea, _T("SM_GETSERVERSELW") },
		{ 0x07ea, _T("UM_GETUSERSELW") },
		{ 0x07eb, _T("SM_GETCURFOCUSA") },
		{ 0x07eb, _T("UM_GETGROUPSELA") },
		{ 0x07ec, _T("SM_GETCURFOCUSW") },
		{ 0x07ec, _T("UM_GETGROUPSELW") },
		{ 0x07ed, _T("SM_GETOPTIONS") },
		{ 0x07ed, _T("UM_GETCURFOCUSA") },
		{ 0x07ee, _T("UM_GETCURFOCUSW") },
		{ 0x07ef, _T("UM_GETOPTIONS") },
		{ 0x07f0, _T("UM_GETOPTIONS2") },
		{ 0x2000, _T("OCMBASE") },
		{ 0x2019, _T("OCM_CTLCOLOR") },
		{ 0x202b, _T("OCM_DRAWITEM") },
		{ 0x202c, _T("OCM_MEASUREITEM") },
		{ 0x202d, _T("OCM_DELETEITEM") },
		{ 0x202e, _T("OCM_VKEYTOITEM") },
		{ 0x202f, _T("OCM_CHARTOITEM") },
		{ 0x2039, _T("OCM_COMPAREITEM") },
		{ 0x204e, _T("OCM_NOTIFY") },
		{ 0x2111, _T("OCM_COMMAND") },
		{ 0x2114, _T("OCM_HSCROLL") },
		{ 0x2115, _T("OCM_VSCROLL") },
		{ 0x2132, _T("OCM_CTLCOLORMSGBOX") },
		{ 0x2133, _T("OCM_CTLCOLOREDIT") },
		{ 0x2134, _T("OCM_CTLCOLORLISTBOX") },
		{ 0x2135, _T("OCM_CTLCOLORBTN") },
		{ 0x2136, _T("OCM_CTLCOLORDLG") },
		{ 0x2137, _T("OCM_CTLCOLORSCROLLBAR") },
		{ 0x2138, _T("OCM_CTLCOLORSTATIC") },
		{ 0x2210, _T("OCM_PARENTNOTIFY") },
		{ 0x8000, _T("WM_APP") },
		{ 0xcccd, _T("WM_RASDIALEVENT") }
};

void CWindow::InitializeEventMap()
{
	OnNonClientDestroy += CEventHandler<CNoEventArgs>([=](const CWindow*) { OnNcDestroy(); });
}

LRESULT CALLBACK CWindow::WindowSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	LRESULT result = -1;
	if (auto pWnd = reinterpret_cast<CWnd*>(dwRefData))
	{
		bool bHandled = false;
		result = pWnd->HandleMessage(hWnd, uMsg, wParam, lParam, bHandled);
		if (uMsg == WM_NCDESTROY)
			pWnd->UnsubclassWindow();

		if (!bHandled)
			result = DefSubclassProc(pWnd->m_hWnd, uMsg, wParam, lParam);
	}
	return result;
}

void CWindow::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CWindow::PumpMessages(const bool& bContinue)
{
	MSG msg = { 0 };
	while (bContinue)
	{
		BOOL bResult = GetMessage(&msg, 0, 0, 0);
		if (bResult <= 0)
			break;

		try
		{
			if (!IsDialogMessage(m_hWnd, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		catch (exception& e)
		{
			const char* pMessage = e.what();
			ASSERT(false);
		}
	}
}

LRESULT CWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	//auto it = MessageIDToMessageName.find(uMsg);

	m_WindowInfo.LastMessage.message = uMsg;
	m_WindowInfo.LastMessage.hwnd = hWnd;
	m_WindowInfo.LastMessage.wParam = wParam;
	m_WindowInfo.LastMessage.lParam = lParam;

	CWindow::CWindowMap::CTempMapHelper windowTempMapHelper;
	if (pThreadWindowMap)
		windowTempMapHelper = pThreadWindowMap->GetTempMapHelper();

	CDeviceContext::CDeviceContextMap::CTempMapHelper deviceContextTempMapHelper;
	if (CDeviceContext::pThreadDeviceContextMap)
		deviceContextTempMapHelper = CDeviceContext::pThreadDeviceContextMap->GetTempMapHelper();

	LRESULT result = 0;
	if (uMsg == WM_COMMAND)
	{
		if (lParam)
			bHandled = ::SendMessage((HWND)lParam, WM_CONTROL_REFLECT, HIWORD(wParam), 0) ? true : false;

		if (bHandled)
		{
			if (CWindowInfo* pInfo = CWindow::GetWindowInfo((HWND)lParam))
				result = pInfo->LastMessageResult;
		}
	}
	else if (uMsg == WM_NOTIFY)
	{
		NMHDR* pNMHDR = reinterpret_cast<NMHDR*>(lParam);
		ENSURE(pNMHDR);
		if (bHandled = ::SendMessage(pNMHDR->hwndFrom, WM_NOTIFY_REFLECT, wParam, lParam) ? true : false)
		{
			if (CWindowInfo* pInfo = CWindow::GetWindowInfo(pNMHDR->hwndFrom))
				result = pInfo->LastMessageResult;
		}
	}
	if (!bHandled)
	{
		auto& eventMap = GetEventMap();
		auto it = eventMap.find(CEventID::FromMessage(uMsg, wParam, lParam));
		if (it != eventMap.end() && it->second)
		{
			result = it->second->Call(*this, wParam, lParam);
			bHandled = true;
		}
	}
	if (bHandled)
		m_WindowInfo.LastMessageResult = result;
	return result;
}

CWindow* CWindow::FromPermenantHandle(HWND hWnd)
{
	if (!pThreadWindowMap)
		return nullptr;

	return pThreadWindowMap->FromPermenantHandle(hWnd);
}

CWindow::CWindowInfo* CWindow::GetWindowInfo(HWND hWnd)
{
	return reinterpret_cast<CWindowInfo*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

CWindow::CWindow()
	: m_bSubclassed(false)
{
	m_WindowInfo.pWindow = this;
}

bool CWindow::Attach(HWND hWnd)
{
	if (!hWnd || m_hWnd || CWindow::FromPermenantHandle(hWnd))
		return false;

	m_hWnd = hWnd;
	InitializeEventMap();
	if (!pThreadWindowMap)
		pThreadWindowMap = new CWindowMap();
	++nThreadWindowMapRefCount;
	pThreadWindowMap->Add(make_pair(m_hWnd, this));

	static_assert(sizeof(void*) == sizeof(LONG_PTR), "cannot use set window long ptr");
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)&m_WindowInfo);

	return true;
}

bool CWindow::Detach()
{
	if (!m_hWnd)
		return false;

	if (pThreadWindowMap)
		pThreadWindowMap->Remove(m_hWnd);

	--nThreadWindowMapRefCount;
	if (!nThreadWindowMapRefCount)
	{
		delete pThreadWindowMap;
		pThreadWindowMap = nullptr;
	}
	m_EventMap.clear();
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
	m_hWnd = nullptr;
	return true;
}

HWND CWindow::Create(const wstring& ClassName, const wstring& WindowName, DWORD dwStyle, const CRect& Rect, const CWindow& Parent, HMENU hMenu, HINSTANCE hInstance)
{
	return ::CreateWindow(ClassName.c_str(), WindowName.c_str(), dwStyle, Rect.left, Rect.top, Rect.Width(), Rect.Height(), Parent, hMenu, hInstance, nullptr);
}

void CWindow::DestroyWindow()
{
	bool bSuccess = ::DestroyWindow(m_hWnd) ? true : false;
	ASSERT(bSuccess);
}

bool CWindow::SubclassWindow(HWND hWnd)
{
	ASSERT(hWnd && !CWindow::FromPermenantHandle(hWnd));
	if (!hWnd || CWindow::FromPermenantHandle(hWnd))
		return false;

	Attach(hWnd);
	PreSubclassWindow();
	m_bSubclassed = ::SetWindowSubclass(hWnd, &WindowSubclassProc, (UINT_PTR)this, (DWORD_PTR)this) ? true : false;
	if (!m_bSubclassed)
		Detach();
	return m_bSubclassed;
}

void CWindow::PreSubclassWindow()
{

}

bool CWindow::UnsubclassWindow()
{
	if (!m_hWnd)
		return false;
	bool bSuccess = ::RemoveWindowSubclass(m_hWnd, &WindowSubclassProc, (UINT_PTR)this) ? true : false;
	if (bSuccess)
	{
		m_bSubclassed = false;
		Detach();
	}
	return bSuccess;
}

void CWindow::Default()
{
	ASSERT(m_WindowInfo.LastMessage.hwnd && m_WindowInfo.LastMessage.message);
	MSG currentMsg = m_WindowInfo.LastMessage;
	if (m_bSubclassed)
		DefSubclassProc(currentMsg.hwnd, currentMsg.message, currentMsg.wParam, currentMsg.lParam);
	m_WindowInfo.LastMessage = currentMsg;
}

void CWindow::SetIcon(HICON hIcon, bool bBigIcon)
{
	SendMessage(WM_SETICON, (WPARAM)hIcon, (LPARAM)bBigIcon);
}

bool CWindow::IsIconic() const
{
	return ::IsIconic(m_hWnd) ? true : false;
}

void CWindow::SetFocus()
{
	::SetFocus(m_hWnd);
}

void CWindow::Invalidate(bool bErase)
{
	::InvalidateRect(m_hWnd, NULL, bErase);
}

wstring CWindow::GetText() const
{
	DWORD dwLength = GetWindowTextLength(m_hWnd);
	vector<TCHAR> buffer;
	buffer.resize(dwLength + 1);
	::GetWindowText(m_hWnd, &buffer[0], (DWORD)buffer.size());
	return &buffer[0];
}

bool CWindow::SetText(const ::std::wstring& Text) const
{
	return SetWindowText(m_hWnd, Text.c_str()) ? true : false;
}

CWindow* CWindow::GetParent() const
{
	return CWindow::FromHandle(::GetParent(m_hWnd));
}

CRect CWindow::GetClientRect() const
{
	CRect rect;
	::GetClientRect(m_hWnd, &rect);
	return rect;
}

DWORD CWindow::GetStyle() const
{
	return (DWORD)GetWindowLongPtr(m_hWnd, GWL_STYLE);
}

void CWindow::SetStyle(DWORD dwStyle)
{
	SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);
}

DWORD CWindow::GetStyleEx() const
{
	return (DWORD)GetWindowLongPtr(m_hWnd, GWL_EXSTYLE);
}

void CWindow::SetStyleEx(DWORD dwStyleEx)
{
	SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, dwStyleEx);
}

int CWindow::GetDlgCtrlID() const
{
	return ::GetDlgCtrlID(m_hWnd);
}

void CWindow::SetFont(CFont Font, bool bRedraw)
{
	SendMessage(WM_SETFONT, (WPARAM)static_cast<HFONT>(Font), bRedraw);
}

CFont CWindow::GetFont() const
{
	return (HFONT)SendMessage(WM_GETFONT, 0, 0);
}

bool CWindow::GetScrollInfo(bool bHorizontal, SCROLLINFO& Info) const
{
	return ::GetScrollInfo(m_hWnd, bHorizontal ? SB_HORZ : SB_VERT, &Info) ? true : false;
}

void CWindow::Scroll(CSize Size)
{
	::SetScrollPos(m_hWnd, SB_HORZ, GetScrollPos(m_hWnd, SB_HORZ) + Size.cx, true);
	::SetScrollPos(m_hWnd, SB_VERT, GetScrollPos(m_hWnd, SB_VERT) + Size.cy, true);
}

void CWindow::ShowWindow(UINT nCmdShow)
{
	::ShowWindow(m_hWnd, nCmdShow);
}

LRESULT CWindow::SendMessage(UINT nMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::SendMessage(m_hWnd, nMsg, wParam, lParam);
}

bool CWindow::PostMessage(UINT nMsg, WPARAM wParam, LPARAM lParam) const
{
	return ::PostMessage(m_hWnd, nMsg, wParam, lParam) ? true : false;
}

void CWindow::OnNcDestroy()
{
	OnFinalMessage();
}

void CWindow::OnFinalMessage()
{
}

CWindow* CWindow::FromHandle(HWND hWnd)
{
	if (!pThreadWindowMap)
		return nullptr;

	return pThreadWindowMap->FromHandle(hWnd);
}
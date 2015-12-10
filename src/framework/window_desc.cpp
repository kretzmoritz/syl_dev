#include "window_desc.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

using namespace SylDev::Framework;

WindowClassDesc::WindowClassDesc()
	: style(CS_HREDRAW | CS_VREDRAW), hInstance(HINST_THISCOMPONENT), hIcon(nullptr), 
	hCursor(LoadCursor(nullptr, IDC_ARROW)), hbrBackground(reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))), 
	hIconSm(nullptr)
{
}

bool WindowClassDesc::operator==(WindowClassDesc const& _rhs)
{
	return style == _rhs.style && hInstance == _rhs.hInstance && hIcon == _rhs.hIcon 
		&& hCursor == _rhs.hCursor && hbrBackground == _rhs.hbrBackground && hIconSm == _rhs.hIconSm;
}

WindowDesc::WindowDesc()
	: dwExStyle(0), lpWindowName("SylWindow"), dwStyle(WS_OVERLAPPEDWINDOW), x(0), y(0), 
	nWidth(640), nHeight(480), nCmdShow(SW_SHOW)
{
}
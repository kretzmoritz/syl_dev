#include "window_desc.h"

using namespace SylDev::Framework;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

WindowDesc::WindowDesc()
	: style(CS_HREDRAW | CS_VREDRAW), hInstance(HINST_THISCOMPONENT), hIcon(nullptr), 
	hCursor(LoadCursor(nullptr, IDC_ARROW)), hbrBackground(reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))), 
	hIconSm(nullptr), dwExStyle(0), lpWindowName("window"), dwStyle(WS_OVERLAPPEDWINDOW), x(0), y(0), 
	nWidth(640), nHeight(480), nCmdShow(SW_SHOW)
{
}
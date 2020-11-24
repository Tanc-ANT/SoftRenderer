#include "Window.h"
#include "pch.h"

HWND Window::screen_handle = nullptr;
HDC Window::screen_dc = nullptr;
HBITMAP Window::screen_hb = nullptr;
HBITMAP Window::screen_ob = nullptr;
bool Window::exit_state = 0;
int Window::keys[512] = {0};

LRESULT Window::screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: exit_state = 1; break;
	case WM_KEYDOWN: keys[wParam & 511] = 1; break;
	case WM_KEYUP: keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int Window::Init(int w, int h, const TCHAR *title)
{
	try
	{
		// Init window 
		// CS_BYTEALIGNCLIENT: align client in top left area.
		WNDCLASS wc = { CS_BYTEALIGNCLIENT,(WNDPROC)screen_events,0,0,0,
		NULL,NULL,NULL,NULL,_T("SCREEN") };

		// Init bitmap
		// Because of top left is origin point 
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
			w * h * 4, 0, 0, 0, 0 } };

		RECT rect = { 0,0,w,h };
		int wx, wy, sx, sy;
		LPVOID ptr;
		HDC hDC;

		Close();

		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		if (!RegisterClass(&wc)) 
			throw "Register Class Faill!";

		// Create handle
		screen_handle = CreateWindow(_T("SCREEN"), title,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
		if (screen_handle == NULL)
			throw "Create Window Faill!";

		exit_state = 0;
		hDC = GetDC(screen_handle);
		screen_dc = CreateCompatibleDC(hDC);
		ReleaseDC(screen_handle, hDC);

		// Create DIP section
		screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
		if (screen_hb == NULL)
			throw "Create DIP Section Faill!";

		screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
		screen_fb = (unsigned char *)ptr;
		width = w;
		height = h;
		screen_pitch = w * 4;

		AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
		// window area
		wx = rect.right - rect.left;
		wy = rect.bottom - rect.top;
		// window position
		sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
		sy = (GetSystemMetrics(SM_CXSCREEN) - wy) / 4;
		if (sy < 0) sy = 0;
		SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
		SetForegroundWindow(screen_handle);

		ShowWindow(screen_handle, SW_NORMAL);

		Dispatch();

		memset(keys, 0, sizeof(int) * 512);
		memset(screen_fb, 0, w*h * 4);
		return 0;
	}
	catch (const char* msg)
	{
		std::cerr << msg << std::endl;
		return -1;
	}
}

int Window::Close(void)
{
	if (screen_dc)
	{
		if (screen_ob)
		{
			SelectObject(screen_dc, screen_ob);
			screen_ob = NULL;
		}
		DeleteDC(screen_dc);
		screen_dc = NULL;
	}
	if (screen_hb) {
		DeleteObject(screen_hb);
		screen_hb = NULL;
	}
	if (screen_handle) {
		CloseWindow(screen_handle);
		screen_handle = NULL;
	}
	return 0;
}

void Window::Dispatch(void)
{
	MSG msg;
	while (1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void Window::Update(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	Dispatch();
}

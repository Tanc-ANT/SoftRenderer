// C header
#include<windows.h>
#include<tchar.h>

// C++ header
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<string>
#include<cmath>
#include<cassert>
using namespace std;

int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];

static HWND screen_handle = NULL;
static HDC screen_dc = NULL;
static HBITMAP screen_hb = NULL;
static HBITMAP screen_ob = NULL;
unsigned char *screen_fb = NULL;
long screen_pitch = 0;

int screen_init(int width, int height, const TCHAR *title);
int screen_close(void);
void screen_dispatch(void);
void screen_update(void);


// Event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
	case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int screen_init(int width, int height,const TCHAR *title)
{
	// Init window 
	// CS_BYTEALIGNCLIENT: align client in top left area.
	WNDCLASS wc = { CS_BYTEALIGNCLIENT,(WNDPROC)screen_events,0,0,0,
	NULL,NULL,NULL,NULL,_T("SCREEN") };

	// Init bitmap
	// Because of top left is origin point 

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB,
		width * height * 4, 0, 0, 0, 0 } };

	RECT rect = { 0,0,width,height };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	screen_close();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	// Create handle
	screen_handle = CreateWindow(_T("SCREEN"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screen_handle == NULL) return -2;

	screen_exit = 0;
	hDC = GetDC(screen_handle);
	screen_dc = CreateCompatibleDC(hDC);
	ReleaseDC(screen_handle, hDC);

	// Create DIP section
	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screen_hb == NULL) return -3;

	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
	screen_fb = (unsigned char *)ptr;
	screen_w = width;
	screen_h = height;
	screen_pitch = width * 4;

	AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CXSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screen_handle);

	ShowWindow(screen_handle, SW_NORMAL);

	screen_dispatch();

	memset(screen_keys, 0, sizeof(int) * 512);
	memset(screen_fb, 0, width*height * 4);

	return 0;
}

int screen_close(void)
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

void screen_dispatch(void)
{
	MSG msg;
	while (1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void screen_update(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	screen_dispatch();
}


int main()
{
	TCHAR *title = _T("SoftRenderer");
	screen_init(800, 600, title);

	while (screen_exit == 0)
	{
		screen_update();
		Sleep(1);
	}

	return 0;
}
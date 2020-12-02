#include "Window.h"
#include "pch.h"
#include <WinUser.h>
#include <timeapi.h>

HWND Window::screen_handle = nullptr;
HDC Window::screen_dc = nullptr;
HBITMAP Window::screen_hb = nullptr;
HBITMAP Window::screen_ob = nullptr;
bool Window::exit_state = 0;
int Window::keys[512] = {0};
bool Window::left_button_state = false;
bool Window::right_button_state = false;

LRESULT Window::screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: exit_state = 1; break;
	case WM_KEYDOWN: keys[wParam & 511] = 1; break;
	case WM_KEYUP: keys[wParam & 511] = 0; break;
	case WM_LBUTTONDOWN:  left_button_state = true; break;
	case WM_LBUTTONUP: left_button_state = false; break;
	case WM_RBUTTONDOWN:  right_button_state = true; break;
	case WM_RBUTTONUP: right_button_state = false; break;
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

static float  fps = 0;
static int    frameCount = 0;
static float  currentTime = 0.0f;
static float  lastTime = 0.0f;

void Window::ShowFPS(void)
{	
	HDC hDC = GetDC(screen_handle);
	frameCount++;
	currentTime = timeGetTime()*0.001f;

	if (currentTime - lastTime > 1.0f)
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	char strFPS[20];
	sprintf_s(strFPS, 20, "FPS:%0.3f", fps);
	TextOutA(hDC, 0, 0, strFPS, 10);

	char strVert[20];
	int n;
	if (n_triangle < 10) n = 1;
	else if (n_triangle < 100) n = 2;
	else if (n_triangle < 1000) n = 3;
	else if (n_triangle < 10000) n = 4;
	else if (n_triangle < 100000) n = 4;
	sprintf_s(strVert, 20, "Triangle:%d", n_triangle);
	TextOutA(hDC, 0, 20, strVert, 9 + n);

	ReleaseDC(screen_handle, hDC);
}

Vector3 Window::GetMousePos()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(screen_handle, &point);
	return Vector3(point.x, point.y, 1.0f);
}

void Window::Update(void)
{
	HDC hDC = GetDC(screen_handle);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(screen_handle, hDC);
	Dispatch();
	ShowFPS();
}

#pragma once
#include<windows.h>
#include<tchar.h>

class Window
{

public:
	int Init(int w, int h, const TCHAR *title);
	void Update(void);
	bool GetClose() { return exit_state; }

public:
	static HWND screen_handle;
	static HDC screen_dc;
	static HBITMAP screen_hb;
	static HBITMAP screen_ob;

	// Event handler
	static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

private:
	int Close(void);
	void Dispatch(void);

private:
	static bool exit_state;
	static int keys[512];

	int width, height;
	long screen_pitch = 0;
	unsigned char *screen_fb;
};
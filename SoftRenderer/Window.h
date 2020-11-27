#pragma once
#include<windows.h>
#include<tchar.h>

class Window
{
public:
	Window() {};
	~Window() {};
	int Init(int w, int h, const TCHAR *title);
	void Update(void);
	
	void SetCloseState(bool e) { exit_state = e; }
	bool GetCloseState() {return exit_state;}
	
	int* GetKey() { return keys; }
	unsigned char* GetFrameBuffer() { return screen_fb; };

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
	unsigned char *screen_fb;
	long screen_pitch = 0;
	
};
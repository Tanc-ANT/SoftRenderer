#pragma once
#include<windows.h>
#include<wingdi.h>
#include<tchar.h>
#include "Vector3.h"

class Window
{
public:
	Window():screen_pitch(0),n_triangle(0){};
	~Window() {};
	int Init(int w, int h, const TCHAR *title);
	void Update(void);
	
	void SetCloseState(const bool& e) { exit_state = e; }
	bool GetCloseState() {return exit_state;}
	
	int* GetKey() { return keys; }
	unsigned char* GetFrameBuffer() { return screen_fb; };

	void SetNtri(int n) { n_triangle = n; }

	bool GetLeftButtonState() { return left_button_state; }
	bool GetRightButtonState() { return right_button_state; }
	Vector3 GetMousePos();

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
	void ShowFPS(void);

private:
	static bool exit_state;
	static int keys[512];
	static bool left_button_state;
	static bool right_button_state;
	int width, height;
	unsigned char *screen_fb;
	long screen_pitch;
	int n_triangle;
	
};
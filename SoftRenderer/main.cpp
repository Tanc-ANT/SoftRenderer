#include "pch.h"
#include "Window.h"
#include "Vector4.h"
#include "Matrix4.h"
using namespace std;


static int width = 800;
static int height = 600;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("Soft Renderer");
	window->Init(width, height, title);

	while (window->GetClose() == 0)
	{
		window->Update();
		Sleep(1);
	}

	return 0;
}
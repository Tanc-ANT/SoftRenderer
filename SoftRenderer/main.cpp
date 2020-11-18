#include "pch.h"
#include "Window.h"
using namespace std;

int main()
{
	Window *window = new Window();
	TCHAR *title = _T("SoftRenderer");
	window->Init(800, 600, title);

	while (window->GetClose() == 0)
	{
		window->Update();
		Sleep(1);
	}

	return 0;
}
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<windowsx.h>
#include<iostream>
#include<string>
using namespace std;

string title = "Soft Renderer";
wstring wtitle = std::wstring(title.begin(), title.end());
LPCWSTR lt = wtitle.c_str();

string content = "Hello world";
wstring wcontent = std::wstring(content.begin(), content.end());
LPCWSTR lc = wcontent.c_str();

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	MessageBox(nullptr, wtitle.c_str(), wcontent.c_str(), MB_OK | MB_ICONEXCLAMATION);
	
	return (0);
}
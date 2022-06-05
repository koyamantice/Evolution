#pragma once
#include<Windows.h>
#include "imgui.h"
class WinApp {
private:
	WNDCLASSEX w{};
	HWND hwnd = nullptr;
	static const bool FULL_SCREEN;
	bool fullScreen;
	RECT windowRect;
	int winSize_X;
	int winSize_Y;
	float windowRate_WIDTH;
	float windowRate_HEIGHT;

public:
	static const int window_width = 1280;//â°ïù
	static const int window_height = 720;//ècïù

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam);
	//getter
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }
public:
	int SetWinCenter();
	void Initialize();
	void Update();
	bool ProcessMessage();
	void Finalize();
};


#include "WinApp.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const bool WinApp::FULL_SCREEN = true;
#include "Resources/resource.h"
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


void WinApp::Initialize() {
	fullScreen = FULL_SCREEN;
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//ウィンドウプロシージャを設定
	w.lpszClassName = L"グノーメン";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定
	w.hIcon = LoadIcon(w.hInstance, MAKEINTRESOURCE(IDI_ICON1));	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	if (fullScreen) {
		// Get the settings of the primary display
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		//ウィンドウサイズ{ X座標　Y座標　横幅　縦幅 }
		windowRect = {
			devMode.dmPosition.x,
			devMode.dmPosition.y,
			devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
			devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
		};

		winSize_X = windowRect.right - windowRect.left;
		winSize_Y = windowRect.bottom - windowRect.top;

		windowRate_WIDTH = (float)winSize_X / window_width;
		windowRate_HEIGHT = (float)winSize_Y / window_height;
	} else {
		winSize_X = window_width;
		winSize_Y = window_height;

		//ウィンドウサイズ{ X座標　Y座標　横幅　縦幅 }
		windowRect = { 0,0,window_width,window_height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);	//自動でサイズ補正
	}

	//ウィンドウオブジェクトの生成
	/*HWND*/ hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"グノーメン",//タイトルバーの文字
		WS_OVERLAPPED | WS_SYSMENU,//標準的なウィンドウスタイル
		//WS_OVERLAPPEDWINDOW,//非固定なウィンドウスタイル
		CW_USEDEFAULT,//表示X座標(OSに任せる)
		CW_USEDEFAULT,//表示Y座標(OSに任せる)
		winSize_X,//ウィンドウ横幅
		winSize_Y,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//SetWinCenter();//ウィンドウを真ん中に持ってくる
	if (fullScreen) {
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX /*| WS_SYSMENU*/ | WS_THICKFRAME));

		SetWindowPos(
			hwnd,
			HWND_TOPMOST,
			windowRect.left,
			windowRect.top,
			windowRect.right,
			windowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		//ウィンドウ表示
		ShowWindow(hwnd, SW_MAXIMIZE);
	} else {
		//ウィンドウ表示
		ShowWindow(hwnd, SW_SHOW);
	}
}

void WinApp::Update() {
}

bool WinApp::ProcessMessage() {

	MSG msg{};//メッセージ
			//メッセージがある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);//キー入力メッセージの処理
		DispatchMessage(&msg);//プロシージャにメッセージを送る
	}
	//xボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize() {
	UnregisterClass(w.lpszClassName, w.hInstance);

}
int  WinApp::SetWinCenter() {
	HWND hDeskWnd;
	RECT deskrc{}, rc{};
	int x, y;

	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hwnd, (LPRECT)&rc);
	x = (deskrc.right - (rc.right - rc.left)) / 2;
	y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
	SetWindowPos(hwnd, HWND_TOPMOST, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
	return 0;
}
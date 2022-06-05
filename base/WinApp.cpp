#include "WinApp.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const bool WinApp::FULL_SCREEN = false;
#include "resource.h"
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
	w.lpfnWndProc = (WNDPROC)WindowProc;//�E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame";//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��
	w.hIcon = LoadIcon(w.hInstance, MAKEINTRESOURCE(IDI_ICON1));	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);
	if (fullScreen) {
		// Get the settings of the primary display
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		//�E�B���h�E�T�C�Y{ X���W�@Y���W�@�����@�c�� }
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

		//�E�B���h�E�T�C�Y{ X���W�@Y���W�@�����@�c�� }
		windowRect = { 0,0,window_width,window_height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);	//�����ŃT�C�Y�␳
	}

	//�E�B���h�E�I�u�W�F�N�g�̐���
	/*HWND*/ hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"�t�P�̃��b�`�[",//�^�C�g���o�[�̕���
		WS_OVERLAPPED | WS_SYSMENU,//�W���I�ȃE�B���h�E�X�^�C��
		//WS_OVERLAPPEDWINDOW,//��Œ�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,//�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,//�\��Y���W(OS�ɔC����)
		winSize_X,//�E�B���h�E����
		winSize_Y,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//SetWinCenter();//�E�B���h�E��^�񒆂Ɏ����Ă���
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

		//�E�B���h�E�\��
		ShowWindow(hwnd, SW_MAXIMIZE);
	} else {
		//�E�B���h�E�\��
		ShowWindow(hwnd, SW_SHOW);
	}
}

void WinApp::Update() {
}

bool WinApp::ProcessMessage() {

	MSG msg{};//���b�Z�[�W
			//���b�Z�[�W������?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}
	//x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
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
	RECT deskrc, rc;
	int x, y;

	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hwnd, (LPRECT)&rc);
	x = (deskrc.right - (rc.right - rc.left)) / 2;
	y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
	SetWindowPos(hwnd, HWND_TOPMOST, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
	return 0;
}
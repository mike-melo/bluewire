#include <windows.h>
#include <tchar.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "ObjFile.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

HWND hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HGLRC renderingContext;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow) {

	MSG msg = { 0 };
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"bluewire";
	wc.style = CS_OWNDC;

	if (!RegisterClass(&wc)) {
		return 1;
	}

	hWnd = CreateWindowW(wc.lpszClassName, L"bluewire", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 800, 600, 0, 0, hInstance, 0);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC deviceContext = GetDC(hWnd);

	int chosenPixelFormat;
	chosenPixelFormat = ChoosePixelFormat(deviceContext, &pfd);
	SetPixelFormat(deviceContext, chosenPixelFormat, &pfd);

	renderingContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, renderingContext);

	ShowWindow(hWnd, SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);

	ObjFile objFile;

	objFile.load("models\\triangle.obj");
	
	BOOL done = false;

	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = TRUE;				
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2i(0, 1);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2i(-1, -1);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2i(1, -1);
			glEnd();
			glFlush();
			SwapBuffers(deviceContext);
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CLOSE:
		wglDeleteContext(renderingContext);
		PostQuitMessage(0);
		break;
		
		default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
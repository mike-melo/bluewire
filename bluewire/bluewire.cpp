#include <windows.h>
#include <GL/GL.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	MSG msg = { 0 };
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = L"bluewire";
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
		return 1;
	CreateWindowW(wc.lpszClassName, L"bluewire", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0, hInstance, 0);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
		DispatchMessage(&msg);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,            
			32,                       
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                       
			8,                        
			0,                        
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC deviceContext = GetDC(hWnd);

		int  chosenPixelFormat;
		chosenPixelFormat = ChoosePixelFormat(deviceContext, &pfd);
		SetPixelFormat(deviceContext, chosenPixelFormat, &pfd);

		HGLRC renderingContext = wglCreateContext(deviceContext);
		wglMakeCurrent(deviceContext, renderingContext);

		MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);

		wglDeleteContext(renderingContext);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
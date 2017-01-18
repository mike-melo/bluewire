#include <windows.h>
#include <tchar.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>

#include <vmath/vmath.h>

#include <iostream>

#include "obj_file.hpp"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define BUFFER_OFFSET(i) ((void*)(i))

#define GLSL(version, shader)  "#version " #version "\n" #shader

const GLchar* vertexShader = GLSL(430,
	layout(location = 0) in vec4 vPosition;
	layout(location = 1) uniform mat4 modelViewProject;
	
	void main() {
		gl_Position = modelViewProject * vPosition;
	}
);

const GLchar* fragmentShader = GLSL(430,
	out vec4 fColor;

	void main() {
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
	}
);

HWND hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void init(vector<vector<GLfloat>> vertices);
void render(vector<vector<GLfloat>> vertices);

HGLRC renderingContext;

using namespace vmath;

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
	
	/* We should properly quit in the proper Win32 way */
	if (glewInit()) {
		cerr << "GLEW could not be initialized" << endl;
		exit(-1);
	}

	obj_file objFile;
	objFile.load("models\\triangle.obj");
	vector<vector<GLfloat>> vertices = objFile.vertices();

	init(vertices);

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
			render(vertices);
			SwapBuffers(deviceContext);
		}
	}

	return 0;
}

void init(vector<vector<GLfloat>> vertices) {
	GLuint buffer;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 3, NULL, GL_STATIC_DRAW);

	GLintptr offset = 0;

	for (size_t i = 0; i<vertices.size(); i++, offset += sizeof(GLfloat) * 3) {
		vector<GLfloat> vertex = vertices.at(i);
		GLfloat rawVertices[] = { vertex.at(0), vertex.at(1), vertex.at(2) };
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * 3, rawVertices);
	}

	GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderHandle, 1, (const GLchar **)&vertexShader, NULL);
	glCompileShader(vertexShaderHandle);

	GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderHandle, 1, (const GLchar **)&fragmentShader, NULL);
	glCompileShader(fragmentShaderHandle);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderHandle);
	glAttachShader(program, fragmentShaderHandle);
	glLinkProgram(program);

	glUseProgram(program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	mat4 projection = frustum( -3, 3, -3, 3, 5, 10 );
	glUniformMatrix4fv(1, 1, GL_TRUE, projection);
}

void render(vector<vector<GLfloat>> vertices) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
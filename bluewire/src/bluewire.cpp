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
	layout(location = 0) in vec3 vPosition;
	layout(location = 1) uniform mat4 MVP;
	
	void main() {
		gl_Position = MVP * vec4(vPosition, 1);
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

void init(vector<GLfloat> vertices, vector<GLushort> faces);
void render(vector<GLushort> faces);
void compileShader(GLuint shaderHandle);

HGLRC renderingContext;
GLuint indices;

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
	vector<GLfloat> vertices = objFile.vertices();
	vector<GLushort> faces = objFile.faces();

	init(vertices, faces);

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
			render(faces);
			SwapBuffers(deviceContext);
		}
	}

	return 0;
}

void init(vector<GLfloat> vertices, vector<GLushort> faces) {
	GLuint buffer;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * faces.size(), faces.data(), GL_STATIC_DRAW);

	GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderHandle, 1, (const GLchar **)&vertexShader, NULL);
	compileShader(vertexShaderHandle);

	GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderHandle, 1, (const GLchar **)&fragmentShader, NULL);
	compileShader(fragmentShaderHandle);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderHandle);
	glAttachShader(program, fragmentShaderHandle);
	glLinkProgram(program);

	glUseProgram(program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	GLuint width = 800, height = 600;
	
	mat4 model = mat4(1.0f);
		
	mat4 view = lookat(vec3(3, 0, -3), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 projection = perspective(60.0f, (float)width / (float)height, 1, 100);
	mat4 mvp = projection * view * model;

	glUniformMatrix4fv(1, 1, GL_FALSE, mvp);
}

void compileShader(GLuint shaderHandle) {
	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(shaderHandle); // Don't leak the shader.
		return;
	}
}

void render(vector<GLushort> faces) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, faces.size(),
		GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
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
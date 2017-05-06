#include <Windows.h>
#include <gl\GL.h>
#include "definitions.h"
#include "shader.h"
#include <glm/glm.hpp>

typedef HGLRC __stdcall wgl_create_context_attribs_arb(HDC hDC,
	HGLRC hShareContext, const int *attribList);

LRESULT CALLBACK WndProc(HWND WindowPtr, UINT msg, WPARAM wParam,
	LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int CommandShow)
{

	WNDCLASSEX WindowsClassStructure;
	WindowsClassStructure.cbSize = sizeof(WNDCLASSEX);
	WindowsClassStructure.style = CS_OWNDC;
	WindowsClassStructure.lpfnWndProc = WndProc;
	WindowsClassStructure.cbClsExtra = 0;
	WindowsClassStructure.cbWndExtra = 0;
	WindowsClassStructure.hInstance = hInstance;
	WindowsClassStructure.hIcon = LoadIcon(0, IDI_APPLICATION);
	WindowsClassStructure.hCursor = LoadCursor(0, IDC_ARROW);
	WindowsClassStructure.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	WindowsClassStructure.lpszMenuName = 0;
	WindowsClassStructure.lpszClassName = (const char*)"OpenGLPlayground";
	WindowsClassStructure.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassEx(&WindowsClassStructure))
	{
		MessageBox(0, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	HWND WindowPtr = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		(const char*)"OpenGLPlayground",
		"OpenGL Playground",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, 0, hInstance, 0);

	if (!WindowPtr)
	{
		MessageBox(0, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	UpdateWindow(WindowPtr);

	ShowWindow(WindowPtr, CommandShow);

	HDC WindowDeviceContext = GetDC(WindowPtr);

	PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
	DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
	DesiredPixelFormat.nVersion = 1;
	DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
	DesiredPixelFormat.cColorBits = 32;
	DesiredPixelFormat.cAlphaBits = 8;
	DesiredPixelFormat.cDepthBits = 16;
	DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

	int SuggestedPixelFormatIndex =
		ChoosePixelFormat(WindowDeviceContext,
			&DesiredPixelFormat);
	if (SuggestedPixelFormatIndex == 0)
	{
		MessageBox(WindowPtr, "Obtaining Suggested Pixel Format Index Failed!",
			"Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
	if (!DescribePixelFormat(WindowDeviceContext,
		SuggestedPixelFormatIndex,
		sizeof(SuggestedPixelFormat), &SuggestedPixelFormat))
	{
		MessageBox(WindowPtr,
			"Obtaining Description of a Possible Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	if (!SetPixelFormat(WindowDeviceContext,
		SuggestedPixelFormatIndex, &SuggestedPixelFormat))
	{
		MessageBox(WindowPtr, "Setting Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC OpenGLContext = wglCreateContext(WindowDeviceContext);
	if (OpenGLContext == 0)
	{
		MessageBox(WindowPtr, "Obtaining OpenGL Render Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//TODO: Assumes OpenGL right now. Make more generalized for DirectX also
	if (wglMakeCurrent(WindowDeviceContext, OpenGLContext))
	{
		//MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
		wgl_create_context_attribs_arb *wglCreateContextAttribsARB =
			(wgl_create_context_attribs_arb *)wglGetProcAddress(
				"wglCreateContextAttribsARB");

		if (wglCreateContextAttribsARB)
		{
			int Attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_FLAGS_ARB,
				WGL_CONTEXT_DEBUG_BIT_ARB, // TODO: Remove Debug for final
				WGL_CONTEXT_PROFILE_MASK_ARB,
				WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
				0
			};
			HGLRC ShareContext = 0;
			HGLRC ModernContext =
				wglCreateContextAttribsARB(WindowDeviceContext,
					ShareContext, Attribs);

			if (ModernContext)
			{
				if (wglMakeCurrent(WindowDeviceContext,
					ModernContext))
				{
					wglDeleteContext(OpenGLContext);
					OpenGLContext = ModernContext;
				}
			}
		}
		else
		{
			MessageBox(WindowPtr, "wglMakeCurrent Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}
	else
	{
		// TODO: Error
	}

	//glEnable(GL_DEPTH_TEST);
	OpenGL_InitializeFunctions();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, VertexShader_Source, NULL);
	glCompileShader(VertexShader);

	GLint ErrorResult = 0;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in compiling VertexShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(VertexShader, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	GLuint TessellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(TessellationControlShader, 1, TessellationControl_Source, NULL);
	glCompileShader(TessellationControlShader);

	glGetShaderiv(TessellationControlShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in compiling TessellationControlShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(TessellationControlShader, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	GLuint TessellationEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(TessellationEvalShader, 1, TessellationEval_Source, NULL);
	glCompileShader(TessellationEvalShader);

	glGetShaderiv(TessellationEvalShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in compiling TessellationEvalShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(TessellationEvalShader, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, FragmentShader_Source, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in compiling FragmentShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(FragmentShader, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in attaching VertexShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetProgramInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	glAttachShader(ShaderProgram, TessellationControlShader);
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in attaching TessellationControlShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetProgramInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	glAttachShader(ShaderProgram, TessellationEvalShader);
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in attaching TessellationEvalShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetProgramInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	glAttachShader(ShaderProgram, FragmentShader);
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in attaching FragmentShader\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetProgramInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(WindowPtr, "Error in linking ShaderProgram\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetProgramInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(WindowPtr, buffer, 0, 0);
	}

	glDeleteShader(VertexShader);
	glDeleteShader(TessellationControlShader);
	glDeleteShader(TessellationEvalShader);
	glDeleteShader(FragmentShader);

	GLuint VertexArrayObject;

	glCreateVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);


	MSG Message = {};

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		glClearBufferfv(GL_COLOR, 0,
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f).data);

		glUseProgram(ShaderProgram);

		GLfloat ColorAttrib[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat VertexAttrib[] = { 0.5f, 0.0f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, VertexAttrib);
		glVertexAttrib4fv(1, ColorAttrib);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, 3);

		if (!SwapBuffers(WindowDeviceContext))
		{
			MessageBox(WindowPtr, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteProgram(ShaderProgram);

	return Message.message;
}


LRESULT CALLBACK WndProc(HWND WindowPtr, UINT Message, WPARAM wParam, LPARAM lParam)
{

	switch (Message)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{

	} break;
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
	} break;

	default:
		return DefWindowProc(WindowPtr, Message, wParam, lParam);
	}

	return 0;
}

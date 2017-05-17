#include <Windows.h>
#include <gl\GL.h>
#include "memory.h"
#include "definitions.h"
#include "shader.h"
#include "collision_detection.h"
#include "debug.h"
#include "renderer.h"

typedef HGLRC __stdcall wgl_create_context_attribs_arb(HDC hDC,
	HGLRC hShareContext, const int *attribList);

LRESULT CALLBACK WndProc(HWND WindowPtr, UINT msg, WPARAM wParam,
	LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int CommandShow)
{
	Mem_Initialize();
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
	WindowsClassStructure.lpszClassName = 
		(const char*)"OpenGLPlayground";
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
		MessageBox(WindowPtr,
			"Obtaining Suggested Pixel Format Index Failed!",
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
		MessageBox(WindowPtr, 
			"Setting Pixel Format Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	HGLRC OpenGLContext = wglCreateContext(WindowDeviceContext);
	if (OpenGLContext == 0)
	{
		MessageBox(WindowPtr,
			 "Obtaining OpenGL Render Context Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//TODO: Assumes OpenGL right now. Make more generalized for DirectX also
	if (wglMakeCurrent(WindowDeviceContext, OpenGLContext))
	{
		wgl_create_context_attribs_arb *wglCreateContextAttribsARB =
			(wgl_create_context_attribs_arb *)wglGetProcAddress(
				"wglCreateContextAttribsARB");
		
		if (wglCreateContextAttribsARB)
		{
			char* OpenGLVersion = (char*)glGetString(GL_VERSION);
			int MajorVersion = (int)(OpenGLVersion[0] - 48);
			int Minor = (int)(OpenGLVersion[2] - 48);
			if (MajorVersion < 3)
			{
				MessageBox(WindowPtr, 
					"OpenGL Version is too outdated to run this application.", 0, 0);
				return 1;
			}
			
			int Attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, MajorVersion,
				WGL_CONTEXT_MINOR_VERSION_ARB, Minor,
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
#if TESSELLATION_CONTROL_SHADER
	GLuint TessellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(TessellationControlShader, 1, TessellationControl_Source, NULL);
	glCompileShader(TessellationControlShader);
#endif
#if TESSELLATION_EVAL_SHADER
	GLuint TessellationEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(TessellationEvalShader, 1, TessellationEval_Source, NULL);
	glCompileShader(TessellationEvalShader);
#endif
#if GEOMETRY_SHADER
	GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(GeometryShader, 1, Geometry_Source, NULL);
	glCompileShader(GeometryShader);
#endif
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, FragmentShader_Source, NULL);
	glCompileShader(FragmentShader);
#if DEBUG_MODE
	Debug_ShaderCompile(VertexShader);
	Debug_ShaderCompile(FragmentShader);
#if TESSELLATION_CONTROL_SHADER
	Debug_ShaderCompile(TessellationControlShader);
#endif
#if TESSELLATION_EVAL_SHADER
	Debug_ShaderCompile(TessellationEvalShader);
#endif
#if GEOMETRY_SHADER
	Debug_ShaderCompile(GeometryShader);
#endif
#endif

	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif
#if TESSELLATION_CONTROL_SHADER
	glAttachShader(ShaderProgram, TessellationControlShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif
#endif
#if TESSELLATION_EVAL_SHADER
	glAttachShader(ShaderProgram, TessellationEvalShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif
#endif
#if GEOMETRY_SHADER
	glAttachShader(ShaderProgram, GeometryShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif
#endif
	glAttachShader(ShaderProgram, FragmentShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif

	glLinkProgram(ShaderProgram);
#if DEBUG_MODE
	Debug_ShaderLink(ShaderProgram);
#endif

	glDeleteShader(VertexShader);
#if TESSELLATION_CONTROL_SHADER
	glDeleteShader(TessellationControlShader);
#endif
#if TESSELLATION_EVAL_SHADER
	glDeleteShader(TessellationEvalShader);
#endif
#if GEOMETRY_SHADER
	glDeleteShader(GeometryShader);
#endif
	glDeleteShader(FragmentShader);

	float VerticeData[] = 
	{
		0.25f, -0.25f, 0.5f,
		-0.25f, -0.25f, 0.5f,
		0.25f, 0.25f, 0.5f
	};

	float ColorData[] =
	{
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	char* CharPtr = 0;
	CharPtr = Mem_Allocate(CharPtr, 2);
	*CharPtr = 'A';

	Model Triangle;
	Triangle.Data = 0;
	Triangle.ArraySize = 0;
	Triangle.NumAttribs = 2;
	Triangle.Data = Mem_Allocate(Triangle.Data, Triangle.NumAttribs);
	Triangle.ArraySize = Mem_Allocate(Triangle.ArraySize, Triangle.NumAttribs);

	Triangle.Data[0] = VerticeData;
	Triangle.ArraySize[0] = 3 * 3 * sizeof(float);
	Triangle.Data[1] = ColorData;
	Triangle.ArraySize[1] = 3 * 3 * sizeof(float);

	RenderObj MyTriangle;
	RenderObj_CreateRenderObject(&MyTriangle, &Triangle);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

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

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#if TESSELLATION_CONTROL_SHADER
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, 3);
#else
		Render_Draw(&MyTriangle);
#endif

#if DEBUG_MODE
		if (!SwapBuffers(WindowDeviceContext))
		{
			MessageBox(WindowPtr, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
#else
		SwapBuffers(WindowDeviceContext);
#endif
	}

	//glDisableVertexArrayAttrib(VertexArrayObject, 0);
	//glDisableVertexArrayAttrib(VertexArrayObject, 1);
	glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteProgram(ShaderProgram);

	glDeleteVertexArrays(1, &MyTriangle.VertexArrayID);

	Mem_Shutdown();

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

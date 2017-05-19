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
			"Obtaining Description of a Possible Pixel Format Failed!",
			"Error!",
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
					"OpenGL Version is too outdated to run this application.", 
					0, 0);
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

#if OPENGL2_0
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
#endif // OpenGL 2.0

#if OPENGL2_0
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, VertexShader_Source, NULL);
	glCompileShader(VertexShader);
#if OPENGL4_0
#if TESSELLATION_CONTROL_SHADER
	GLuint TessellationControlShader = 
		glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(TessellationControlShader, 1, 
		TessellationControl_Source, NULL);
	glCompileShader(TessellationControlShader);
#endif // TESSELLATION_CONTROL_SHADER
#if TESSELLATION_EVAL_SHADER
	GLuint TessellationEvalShader = 
		glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(TessellationEvalShader, 1, 
		TessellationEval_Source, NULL);
	glCompileShader(TessellationEvalShader);
#endif // TESSELLATION_EVAL_SHADER
#endif // OpenGL 4.0
#if OPENGL3_2	
#if GEOMETRY_SHADER
	GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(GeometryShader, 1, Geometry_Source, NULL);
	glCompileShader(GeometryShader);
#endif // GEOMETRY_SHADER
#endif // OpenGL 3.2
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, FragmentShader_Source, NULL);
	glCompileShader(FragmentShader);
#if DEBUG_MODE
	Debug_ShaderCompile(VertexShader);
	Debug_ShaderCompile(FragmentShader);
#if OPENGL4_0
#if TESSELLATION_CONTROL_SHADER
	Debug_ShaderCompile(TessellationControlShader);
#endif // TESSELLATION_CONTROL_SHADER
#if TESSELLATION_EVAL_SHADER
	Debug_ShaderCompile(TessellationEvalShader);
#endif // TESSELLATION_EVAL_SHADER
#endif // OpenGL 4.0
#if OPENGL3_2	
#if GEOMETRY_SHADER
	Debug_ShaderCompile(GeometryShader);
#endif // GEOMETRY_SHADER
#endif // OpenGL 3.2
#endif // DEBUG_MODE
	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif // DEBUG_MODE
#if OPENGL4_0
#if TESSELLATION_CONTROL_SHADER
	glAttachShader(ShaderProgram, TessellationControlShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif // DEBUG_MODE
#endif // TESSELLATION_CONTROL_SHADER
#if TESSELLATION_EVAL_SHADER
	glAttachShader(ShaderProgram, TessellationEvalShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif // DEBUG_MODE
#endif // TESSELLATION_EVAL_SHADER
#endif // OpenGL 4.0
#if OPENGL3_2
#if GEOMETRY_SHADER
	glAttachShader(ShaderProgram, GeometryShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif // DEBUG_MODE
#endif // GEOMETRY_SHADER
#endif // OpenGL 3.2
	glAttachShader(ShaderProgram, FragmentShader);
#if DEBUG_MODE
	Debug_ShaderAttach(ShaderProgram);
#endif // DEBUG_MODE
	glLinkProgram(ShaderProgram);
#if DEBUG_MODE
	Debug_ShaderLink(ShaderProgram);
#endif // DEBUG_MODE

	glDeleteShader(VertexShader);
#if OPENGL4_0
#if TESSELLATION_CONTROL_SHADER
	glDeleteShader(TessellationControlShader);
#endif // TESSELLATION_CONTROL_SHADER
#if TESSELLATION_EVAL_SHADER
	glDeleteShader(TessellationEvalShader);
#endif // TESSELLATION_EVAL_SHADER
#endif // OpenGL 4.0
#if OPENGL3_2
#if GEOMETRY_SHADER
	glDeleteShader(GeometryShader);
#endif // GEOMETRY_SHADER
#endif // OpenGL 3.2
	glDeleteShader(FragmentShader);
#endif // OpenGL 2.0


	char* CharPtr = 0;
	CharPtr = Mem_Allocate(CharPtr, 2);
	*CharPtr = 'A';

	Model Triangle;
	Triangle.NumAttribs = 2;
	Triangle.Data = 0;
	Triangle.Data = Mem_Allocate(Triangle.Data, Triangle.NumAttribs);
	Triangle.ArraySize = 0;
	Triangle.ArraySize = Mem_Allocate(Triangle.ArraySize, 
		Triangle.NumAttribs);

	Triangle.ArraySize[0] = 3 * 3 * sizeof(float);
	// NOTE: Vertices
	Triangle.Data[0] = Mem_Allocate(Triangle.Data[0], 9);
	Triangle.Data[0][0] = 0.25f; Triangle.Data[0][1] = 
		-0.25f; Triangle.Data[0][2] = 0.5f;
	Triangle.Data[0][3] = -0.25f; Triangle.Data[0][4] =
		-0.25f; Triangle.Data[0][5] = 0.5f;
	Triangle.Data[0][6] = 0.25f; Triangle.Data[0][7] = 
		0.25f; Triangle.Data[0][8] = 0.5f;

	Triangle.ArraySize[1] = 3 * 3 * sizeof(float);
	// NOTE: Color
	Triangle.Data[1] = Mem_Allocate(Triangle.Data[1], 9);
	Triangle.Data[1][0] = 0.0f; Triangle.Data[1][1] =
		1.0f; Triangle.Data[1][2] = 0.0f;
	Triangle.Data[1][3] = 0.0f; Triangle.Data[1][4] =
		1.0f; Triangle.Data[1][5] = 0.0f;
	Triangle.Data[1][6] = 0.0f; Triangle.Data[1][7] =
		1.0f; Triangle.Data[1][8] = 0.0f;

	RenderObj MyTriangle;
	RenderObj_CreateRenderObject(&MyTriangle, &Triangle);

	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	MSG Message = {};
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

#if OPENGL3_0
		glClearBufferfv(GL_COLOR, 0,
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f).data);
#endif // OpenGL 3.0
#if OPENGL2_0
		glUseProgram(ShaderProgram);
#endif // OpenGL 2.0
		// OpenGL 1.1
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#if TESSELLATION_CONTROL_SHADER
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, 3);
#else
		Render_Draw(&MyTriangle);
#endif // TESSELLATION_CONTROL_SHADER

#if DEBUG_MODE
		if (!SwapBuffers(WindowDeviceContext))
		{
			MessageBox(WindowPtr, "Swapping Buffers Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
#else
		SwapBuffers(WindowDeviceContext);
#endif // DEBUG_MODE
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


LRESULT CALLBACK WndProc(HWND WindowPtr, UINT Message, 
	WPARAM wParam, LPARAM lParam)
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

#include <Windows.h>
#include <gl\GL.h>
#include "memory.h"
#include "definitions.h"
#include "shader.h"
#include "collision_detection.h"
#include "debug.h"
#include "model.h"
#include "renderer.h"
#include "bmp.h"

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

	//Model Triangle;
	//Triangle.NumAttribs = 2;
	//Triangle.Data = 0;
	//Triangle.Data = Mem_Allocate(Triangle.Data, Triangle.NumAttribs);
	//Triangle.ArraySize = 0;
	//Triangle.ArraySize = Mem_Allocate(Triangle.ArraySize, 
	//	Triangle.NumAttribs);
	//
	//Triangle.ArraySize[0] = 3 * 3 * sizeof(float);
	//// NOTE: Vertices
	//Triangle.Data[0] = Mem_Allocate(Triangle.Data[0], 9);
	//Triangle.Data[0][0] = 0.25f; Triangle.Data[0][1] = 
	//	-0.25f; Triangle.Data[0][2] = 0.5f;
	//Triangle.Data[0][3] = -0.25f; Triangle.Data[0][4] =
	//	-0.25f; Triangle.Data[0][5] = 0.5f;
	//Triangle.Data[0][6] = 0.25f; Triangle.Data[0][7] = 
	//	0.25f; Triangle.Data[0][8] = 0.5f;
	//
	//Triangle.ArraySize[1] = 3 * 3 * sizeof(float);
	//// NOTE: Color
	//Triangle.Data[1] = Mem_Allocate(Triangle.Data[1], 9);
	//Triangle.Data[1][0] = 0.0f; Triangle.Data[1][1] =
	//	1.0f; Triangle.Data[1][2] = 0.0f;
	//Triangle.Data[1][3] = 0.0f; Triangle.Data[1][4] =
	//	1.0f; Triangle.Data[1][5] = 0.0f;
	//Triangle.Data[1][6] = 0.0f; Triangle.Data[1][7] =
	//	1.0f; Triangle.Data[1][8] = 0.0f;
	//Model* MyBox = new Model{};
	//Model_CreateBox(MyBox, 0.5f, 0.5f, 0.5f);
	//
	//RenderObj* BoxRenderObj = new RenderObj{};
	//RenderObj_CreateRenderObject(BoxRenderObj, MyBox, 3 * 6 * 6);
	Model* MyRectangle = new Model{};
	ModelObj_CreateRectangle(MyRectangle, 2.0f, 2.0f);

	RenderObj* RectRenderObj = new RenderObj{};
	RenderObj_CreateRenderObject(RectRenderObj, MyRectangle, 6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		RectRenderObj->IndiceID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		(GLsizei*)&MyRectangle->IndiceCount,
		MyRectangle->IndiceData,
		GL_STATIC_DRAW);
#if STD140
	const char* UniformNames[4] = 
	{
		"TestManualBlock.scale",
		"TestManualBlock.translation",
		"TestManualBlock.rotation",
		"TestManualBlock.projection_matrix"
	};

	GLuint UniformIndices[4];
	glGetUniformIndices(ShaderProgram, 1, UniformNames, UniformIndices);

	GLint UniformOffsets[4];
	GLint ArrayStrides[4];
	GLint MatrixStrides[4];
	glGetActiveUniformsiv(ShaderProgram, 4, UniformIndices,
		GL_UNIFORM_OFFSET, UniformOffsets);
	glGetActiveUniformsiv(ShaderProgram, 4, UniformIndices,
		GL_UNIFORM_ARRAY_STRIDE, ArrayStrides);
	glGetActiveUniformsiv(ShaderProgram, 4, UniformIndices,
		GL_UNIFORM_MATRIX_STRIDE, MatrixStrides);

	unsigned char* Buffer = (unsigned char*)malloc(4096);
	*((float *)(Buffer + UniformOffsets[0])) = 3.0f;
	((float *)(Buffer + UniformOffsets[1]))[0] = 1.0f;
	((float *)(Buffer + UniformOffsets[1]))[1] = 2.0f;
	((float *)(Buffer + UniformOffsets[1]))[2] = 3.0f;

	const GLfloat Rotations[] = {30.0f, 40.0f, 60.0f};
	unsigned int Offset = UniformOffsets[2];

	for (int n = 0; n < 3; n++)
	{
		*((float *)(Buffer + Offset)) = Rotations[n];
		Offset += ArrayStrides[2];
	}

	const GLfloat Matrix[] =
	{
		1.0f, 2.0f, 3.0f, 4.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		2.0f, 4.0f, 6.0f, 8.0f,
		1.0f, 3.0f, 5.0f, 7.0f
	};
	for (int i = 0; i < 4; i++)
	{
		GLuint offset = UniformOffsets[3] + MatrixStrides[3] * i;
		for (int j = 0; j < 4; j++)
		{
			*((float *)(Buffer + Offset)) = Matrix[i * 4 + j];
			offset += sizeof(GLfloat);
		}
	}
#endif

	glm::vec3 EyePosition(0.0f, 0.0f, 10.0f);
	glm::vec3 UpVector(0.0f, 1.0f, 0.0f);
	glm::vec3 LookAtPos(0.0f, 0.0f, 0.0f);
	glm::mat4 ViewMatrix = glm::lookAt(EyePosition, LookAtPos, UpVector);
	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT , 0.01f, 1000.0f);

	glm::mat4 ModelMatrix = glm::mat4();
	//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.5f, 0.0f, 0.0f));
	//ModelMatrix = glm::rotate(ModelMatrix, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	
	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGBA8, 256, 256);
	glBindTexture(GL_TEXTURE_2D, texture);

	HANDLE ImageFile;
	ImageFile = CreateFile("Images/container2.bmp", GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD FileSize = GetFileSize(ImageFile, 0);
	unsigned char* Buffer = new unsigned char[FileSize] {};
	LPDWORD BytesRead = 0;
	ReadFile(ImageFile, (LPVOID)Buffer, FileSize, BytesRead, 0);
	BmpDimensions ImageDimensions;
	Buffer = GetBmpData(Buffer, ImageDimensions);

	glTextureSubImage2D(texture, 0, 0, 0,
		ImageDimensions.Width, ImageDimensions.Height,
		GL_RGBA, GL_UNSIGNED_BYTE, Buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	///////////////////
	//char* FileName = "test.bmp";
	//struct stat buffer;
	//stat(FileName, &buffer);
	//
	//unsigned char* ReturnValues = new unsigned char[buffer.st_size]();
	//
	//FILE* OpenFile;
	//// NOTE: rb = Read Binary
	//fopen_s(&OpenFile, FileName, "rb");
	//
	//if (OpenFile != 0)
	//{
	//	size_t NumberOfElementsRead = fread(ReturnValues,
	//		sizeof(uint8), buffer.st_size, OpenFile);
	//
	//	fclose(OpenFile);
	//}
	//else
	//{
	//	// TODO: Error
	//	Platform_TemporaryError("Failed to open file");
	//	return 0;
	//}
	//////////////

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

		glUniformMatrix4fv(2, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(3, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(4, 1, GL_FALSE, &ProjectionMatrix[0][0]);
		glUniform1i(5, 0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// OpenGL 1.1
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#if TESSELLATION_CONTROL_SHADER
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawArrays(GL_PATCHES, 0, 3);
#else
		Render_Draw(RectRenderObj);
#endif // TESSELLATION_CONTROL_SHADER
		glBindTexture(GL_TEXTURE_2D, 0);
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

#if STD140
	free(Buffer);
#endif
	delete[] Buffer;
	//glDisableVertexArrayAttrib(VertexArrayObject, 0);
	//glDisableVertexArrayAttrib(VertexArrayObject, 1);
	glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VertexArrayObject);
	glDeleteProgram(ShaderProgram);

	//glDeleteVertexArrays(1, &BoxRenderObj->VertexArrayID);
	//Model_Delete(MyBox);
	//RenderObj_Delete(BoxRenderObj);
	glDeleteVertexArrays(1, &RectRenderObj->VertexArrayID);
	Model_Delete(MyRectangle);
	RenderObj_Delete(RectRenderObj);

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

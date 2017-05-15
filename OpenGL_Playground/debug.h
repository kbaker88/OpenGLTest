#ifndef DEBUG_H
#define DEBUG_H
#if DEBUG_MODE

void Debug_ShaderCompile(unsigned int Shader)
{
	GLint ErrorResult = 0;
	HWND Window = GetActiveWindow();
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in compiling shader.\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(Shader, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
}

void Debug_ShaderAttach(unsigned int Shader)
{
	GLint ErrorResult = 0;
	HWND Window = GetActiveWindow();
	glGetProgramiv(Shader, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in attaching shader.\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(Shader, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
}

void Debug_ShaderLink(unsigned int Shader)
{
	GLint ErrorResult = 0;
	HWND Window = GetActiveWindow();
	glGetProgramiv(Shader, GL_LINK_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in linking shader.\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(Shader, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
}


#endif
#endif
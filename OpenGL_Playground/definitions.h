#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef char GLchar;

// Accepted values for attribute names for the OpenGL Context(WGL)
#define WGL_CONTEXT_MAJOR_VERSION_ARB			  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB			  0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB				  0x2093
#define WGL_CONTEXT_FLAGS_ARB					  0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB			  0x9126

// Accepted values for flag bits for the OpenGL Context(WGL)
#define WGL_CONTEXT_DEBUG_BIT_ARB				  0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	  0x0002

// Accepted values for attribute bits for the OpenGL Context(WLG)
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB		  0X00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0X00000002

// OpenGL 2.0
#define GL_VERTEX_SHADER					0x8B31
#define GL_FRAGMENT_SHADER					0x8B30
#define GL_DELETE_STATUS					0x8B80
#define GL_COMPILE_STATUS					0x8B81
#define GL_LINK_STATUS					    0x8B82
#define GL_VALIDATE_STATUS                  0x8B83
#define GL_INFO_LOG_LENGTH                  0x8B84
#define GL_ATTACHED_SHADERS                 0x8B85
#define GL_ACTIVE_UNIFORMS					0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH		0x8B87
#define GL_SHADER_TYPE						0x8B4F
#define GL_INFO_LOG_LENGTH					0x8B84
#define GL_SHADER_SOURCE_LENGTH				0x8B88
#define GL_ACTIVE_ATTRIBUTES				0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH		0x8B8A

typedef GLuint __stdcall gl_create_shader(GLenum shaderType);
typedef void __stdcall gl_shader_source(GLuint shader, GLsizei count,
	const GLchar **string, const GLint *length);
typedef void __stdcall gl_compile_shader(GLuint shader);
typedef GLuint __stdcall gl_create_program(void);
typedef void __stdcall gl_attach_shader(GLuint program, GLuint shader);
typedef void __stdcall gl_link_program(GLuint program);
typedef void __stdcall gl_delete_shader(GLuint shader);
typedef void __stdcall gl_delete_program(GLuint program);
typedef void __stdcall gl_use_program(GLuint program);
typedef void __stdcall gl_get_shader_iv(GLuint shader, GLenum pname, GLint* params);
typedef void __stdcall gl_get_shader_info_log(GLuint shader, GLsizei maxLength,
	GLsizei* length, GLchar* infoLog);
typedef void __stdcall gl_get_program_iv(GLuint program, GLenum pname, GLint* params);
typedef void __stdcall gl_get_program_info_log(GLuint program, GLsizei maxLength,
	GLsizei* length, GLchar* infoLog);
typedef void __stdcall gl_vertex_attrib_4fv(GLuint index, const GLfloat* v);

static gl_create_shader* glCreateShader;
static gl_shader_source* glShaderSource;
static gl_compile_shader* glCompileShader;
static gl_create_program* glCreateProgram;
static gl_attach_shader* glAttachShader;
static gl_link_program* glLinkProgram;
static gl_delete_shader* glDeleteShader;
static gl_delete_program* glDeleteProgram;
static gl_use_program* glUseProgram;
static gl_get_shader_iv* glGetShaderiv;
static gl_get_shader_info_log* glGetShaderInfoLog;
static gl_get_program_iv* glGetProgramiv;
static gl_get_program_info_log* glGetProgramInfoLog;

static gl_vertex_attrib_4fv* glVertexAttrib4fv;

// OpenGL 3.0
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76

typedef void __stdcall gl_clear_buffer_fv(GLenum Buffer, GLint drawBuffer,
	const GLfloat *value);
typedef void __stdcall gl_delete_vertex_arrays(GLsizei n, const GLuint* arrays);

static gl_clear_buffer_fv* glClearBufferfv;
static gl_delete_vertex_arrays* glDeleteVertexArrays;
// OpenGL 3.2
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_GEOMETRY_VERTICES_OUT          0x8916
#define GL_GEOMETRY_INPUT_TYPE            0x8917
#define GL_GEOMETRY_OUTPUT_TYPE           0x8918

// OpenGL 4.0
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_PATCHES                        0x000E
#define GL_PATCH_VERTICES                 0x8E72

typedef void __stdcall gl_patch_parameter_i(GLenum pname, GLint value);

static gl_patch_parameter_i* glPatchParameteri;

// OpenGL 4.1
#define GL_PROGRAM_BINARY_LENGTH          0x8741

// OpenGL 4.2
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9

// OpenGL 4.3
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267

// OpenGL 4.5
typedef void __stdcall gl_create_vertex_arrays(GLsizei n, GLuint* arrays);
typedef void __stdcall gl_bind_vertex_array(GLuint array);

static gl_create_vertex_arrays* glCreateVertexArrays;
static gl_bind_vertex_array* glBindVertexArray;


static void OpenGL_InitializeFunctions()
{
	glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
	glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
	glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
	glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
	glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
	glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
	glDeleteShader = (gl_delete_shader *)wglGetProcAddress("glDeleteShader");
	glDeleteProgram = (gl_delete_program *)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");
	glGetShaderiv = (gl_get_shader_iv *)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (gl_get_program_info_log *)wglGetProcAddress("glGetProgramInfoLog");
	glVertexAttrib4fv = (gl_vertex_attrib_4fv *)wglGetProcAddress("glVertexAttrib4fv");

	glClearBufferfv = (gl_clear_buffer_fv *)wglGetProcAddress("glClearBufferfv");
	glDeleteVertexArrays = (gl_delete_vertex_arrays *)wglGetProcAddress("glDeleteVertexArrays");

	glPatchParameteri = (gl_patch_parameter_i *)wglGetProcAddress("glPatchParameteri");

	glCreateVertexArrays = (gl_create_vertex_arrays *)wglGetProcAddress("glCreateVertexArrays");
	glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
}

#endif
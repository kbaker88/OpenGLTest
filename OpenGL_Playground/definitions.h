#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define DEBUG_MODE 1

typedef char GLchar;

#define OPENGL2_0 1
#define OPENGL2_1 1
#define OPENGL3_0 1
#define OPENGL3_1 1
#define OPENGL3_2 1
#define OPENGL4_0 1
#define OPENGL4_1 1
#define OPENGL4_2 1
#define OPENGL4_3 1
#define OPENGL4_4 1
#define OPENGL4_5 1

#if OPENGL2_0
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

// OpenGL 1.5
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893

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
typedef void __stdcall gl_get_shader_iv(GLuint shader, GLenum pname, 
	GLint* params);
typedef void __stdcall gl_get_shader_info_log(GLuint shader, 
	GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void __stdcall gl_get_program_iv(GLuint program, GLenum pname,
	GLint* params);
typedef void __stdcall gl_get_program_info_log(GLuint program,
	GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void __stdcall gl_gen_buffers(GLsizei n, GLuint* buffers);
typedef void __stdcall gl_bind_buffer(GLenum target, GLuint buffer);
typedef void __stdcall gl_buffer_sub_data(GLenum target, GLint* offset, 
	GLsizei* size, const GLvoid * data);
typedef void* __stdcall gl_map_buffer(GLenum target, GLenum access);
typedef void* __stdcall gl_map_buffer_range(GLenum target, GLint* offset,
	GLsizei* length, GLbitfield access);
typedef GLboolean __stdcall gl_unmap_buffer(GLenum target);
typedef void __stdcall gl_enable_vertex_attrib_array(GLuint index);
typedef void __stdcall gl_disable_vertex_attrib_array(GLuint index);
typedef void __stdcall gl_vertex_attrib_3fv(GLuint index,
	const GLfloat* v);
typedef void __stdcall gl_vertex_attrib_4fv(GLuint index,
	const GLfloat* v);

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

static gl_gen_buffers* glGenBuffers;
static gl_bind_buffer* glBindBuffer;
static gl_buffer_sub_data* glBufferSubData;
static gl_map_buffer* glMapBuffer;
static gl_map_buffer_range* glMapBufferRange;
static gl_unmap_buffer* glUnmapBuffer;
static gl_enable_vertex_attrib_array* glEnableVertexAttribArray;
static gl_disable_vertex_attrib_array* glDisableVertexAttribArray;
static gl_vertex_attrib_3fv* glVertexAttrib3fv;
static gl_vertex_attrib_4fv* glVertexAttrib4fv;

#if OPENGL2_1
// OpenGL 2.1
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC

#if OPENGL3_0
// OpenGL 3.0
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506

typedef void __stdcall gl_clear_buffer_fv(GLenum Buffer, 
	GLint drawBuffer, const GLfloat *value);
typedef void __stdcall gl_delete_vertex_arrays(GLsizei n, 
	const GLuint* arrays);
typedef void __stdcall gl_bind_vertex_array(GLuint array);

static gl_clear_buffer_fv* glClearBufferfv;
static gl_delete_vertex_arrays* glDeleteVertexArrays;
static gl_bind_vertex_array* glBindVertexArray;

#if OPENGL3_1
// OpenGL 3.1
#define GL_TEXTURE_BUFFER                 0x8C2A
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37

typedef void __stdcall gl_copy_buffer_sub_data(GLenum readTarget,
	GLenum writeTarget, GLint* readOffset, GLint* writeOffset,
	GLsizei* size);
typedef void __stdcall gl_get_uniform_indices(GLuint program,
	GLsizei uniformCount, const GLchar** uniformNames,
	GLuint *uniformIndices);
typedef void __stdcall gl_get_active_uniforms_iv(GLuint program,
	GLsizei uniformCount, const GLuint* uniformIndices,
	GLenum pname, GLint* params);
typedef GLuint __stdcall gl_get_uniform_block_index(GLuint program,
	const GLchar* uniformBlockName);
typedef GLuint __stdcall gl_uniform_block_binding(GLuint program,
	GLuint uniformBlockIndex, GLuint uniformBlockBinding);

static gl_copy_buffer_sub_data* glCopyBufferSubData;
static gl_get_uniform_indices* glGetUniformIndices;
static gl_get_active_uniforms_iv* glGetActiveUniformsiv;
static gl_get_uniform_block_index* glGetUniformBlockIndex;
static gl_uniform_block_binding* glUniformBlockBinding;

#if OPENGL3_2
// OpenGL 3.2
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_GEOMETRY_VERTICES_OUT          0x8916
#define GL_GEOMETRY_INPUT_TYPE            0x8917
#define GL_GEOMETRY_OUTPUT_TYPE           0x8918

#if OPENGL4_0
// OpenGL 4.0
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_PATCHES                        0x000E
#define GL_PATCH_VERTICES                 0x8E72
#define GL_DRAW_INDIRECT_BUFFER           0x8F3F

typedef void __stdcall gl_patch_parameter_i(GLenum pname, GLint value);

static gl_patch_parameter_i* glPatchParameteri;

#if OPENGL4_1
// OpenGL 4.1
#define GL_PROGRAM_BINARY_LENGTH          0x8741

#if OPENGL4_2
// OpenGL 4.2
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9
#define GL_ATOMIC_COUNTER_BUFFER          0x92C0

#if OPENGL4_3
// OpenGL 4.3
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_DISPATCH_INDIRECT_BUFFER       0x90EE

typedef void __stdcall gl_clear_buffer_sub_data(GLenum target,
	GLenum internalformat, GLint* offset, GLsizei* size,
	GLenum format, GLenum type, const void* data);
typedef void __stdcall gl_vertex_attrib_binding(GLuint attribindex,
	GLuint bindingindex);
typedef void __stdcall gl_bind_vertex_buffer(GLuint bindingindex,
	GLuint buffer, GLint* offset, GLint* stride);
typedef void __stdcall gl_vertex_attrib_ib_format(GLuint attribindex,
	GLint size, GLenum type, GLboolean normalized,
	GLuint relativeoffset);
typedef void __stdcall gl_vertex_attrib_ibi_format(GLuint attribindex,
	GLint size, GLenum type, GLuint relativeoffset);
typedef void __stdcall gl_vertex_attrib_ibl_format(GLuint attribindex,
	GLint size, GLenum type, GLuint relativeoffset);

static gl_clear_buffer_sub_data* glClearBufferSubData;
static gl_vertex_attrib_binding* glVertexAttribBinding;
static gl_bind_vertex_buffer* glBindVertexBuffer;
static gl_vertex_attrib_ib_format*  glVertexAttribFormat;
static gl_vertex_attrib_ibi_format* glVertexAttribIFormat;
static gl_vertex_attrib_ibl_format* glVertexAttribLFormat;

#if OPENGL4_4
// OpenGL 4.4
#define GL_QUERY_BUFFER                   0x9192
#define GL_DYNAMIC_STORAGE_BIT            0x0100
#define GL_MAP_PERSISTENT_BIT             0x0040
#define GL_MAP_COHERENT_BIT               0x0080
#define GL_CLIENT_STORAGE_BIT             0x0200

typedef void __stdcall gl_buffer_storage(GLenum target, GLsizei* size,
	const GLvoid* data, GLbitfield flags);

static gl_buffer_storage* glBufferStorage;

#if OPENGL4_5
// OpenGL 4.5
typedef void __stdcall gl_create_vertex_arrays(GLsizei n,
	GLuint* arrays);
typedef void __stdcall gl_create_buffers(GLsizei n, GLuint *buffers);
typedef void __stdcall gl_named_buffer_storage(GLuint buffer, 
	GLsizei size, const void* data, GLbitfield flags);
typedef void* __stdcall gl_map_named_buffer(GLuint buffer,
	GLenum access);
typedef GLboolean __stdcall gl_unmap_named_buffer(GLuint buffer);
typedef void* __stdcall gl_map_named_buffer_range(GLuint buffer,
	GLint* offset, GLsizei length, GLbitfield access);
typedef void __stdcall gl_clear_named_buffer_sub_data(GLuint buffer,
	GLenum internalformat, GLint* offset, GLsizei size,
	GLenum format, GLenum type, const void* data);
typedef void __stdcall gl_copy_named_buffer_sub_data(GLuint readBuffer,
	GLuint writeBuffer, GLint* readOffset, GLint* writeOffset,
	GLsizei size);
typedef void __stdcall gl_named_buffer_sub_data(GLuint buffer,
	GLint* offset, GLsizei size, const void *data);
typedef void __stdcall gl_vertex_array_attrib_binding(GLuint vaobj,
	GLuint attribindex, GLuint bindingindex);
typedef void __stdcall gl_vertex_array_vertex_buffer(GLuint vaobj,
	GLuint bindingindex, GLuint buffer, GLint* offset, GLsizei stride);
typedef void __stdcall gl_vertex_array_attrib_ib_format( GLuint vaobj,
	GLuint attribindex, GLint size, GLenum type, GLboolean normalized,
	GLuint relativeoffset);
typedef void __stdcall gl_vertex_array_attrib_ibi_format( GLuint vaobj,
	GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void __stdcall gl_vertex_array_attrib_ibl_format( GLuint vaobj,
	GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void __stdcall gl_enable_vertex_array_attrib(GLuint vaobj, 
	GLuint index);
typedef void __stdcall gl_disable_vertex_array_attrib(GLuint vaobj, 
	GLuint index);

static gl_create_vertex_arrays* glCreateVertexArrays;
static gl_create_buffers* glCreateBuffers;
static gl_named_buffer_storage* glNamedBufferStorage;
static gl_map_named_buffer* glMapNamedBuffer;
static gl_unmap_named_buffer* glUnmapNamedBuffer;
static gl_map_named_buffer_range* glMapNamedBufferRange;
static gl_clear_named_buffer_sub_data* glClearNamedBufferSubData;
static gl_copy_named_buffer_sub_data* glCopyNamedBufferSubData;
static gl_named_buffer_sub_data* glNamedBufferSubData;
static gl_vertex_array_attrib_binding* glVertexArrayAttribBinding;
static gl_vertex_array_vertex_buffer* glVertexArrayVertexBuffer;
static gl_vertex_array_attrib_ib_format* glVertexArrayAttribFormat;
static gl_vertex_array_attrib_ibi_format* glVertexArrayAttribIFormat;
static gl_vertex_array_attrib_ibl_format* glVertexArrayAttribLFormat;
static gl_enable_vertex_array_attrib* glEnableVertexArrayAttrib;
static gl_disable_vertex_array_attrib* glDisableVertexArrayAttrib;
#endif // OpenGL 4.5
#endif // OpenGL 4.4
#endif // OpenGL 4.3
#endif // OpenGL 4.2
#endif // OpenGL 4.1
#endif // OpenGL 4.0
#endif // OpenGL 3.2
#endif // OpenGL 3.1
#endif // OpenGL 3.0
#endif // OpenGL 2.1
#endif // OpenGL 2.0

static void OpenGL_InitializeFunctions()
{
#if OPENGL2_0
	// OpenGL 2.0
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
	glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
	glBufferSubData = (gl_buffer_sub_data *)wglGetProcAddress("glBufferSubData");
	glMapBuffer = (gl_map_buffer *)wglGetProcAddress("glMapBuffer");
	glMapBufferRange = (gl_map_buffer_range *)wglGetProcAddress("glMapBufferRange");
	glUnmapBuffer = (gl_unmap_buffer *)wglGetProcAddress("glUnmapBuffer");
	glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (gl_disable_vertex_attrib_array *)wglGetProcAddress("glDisableVertexAttribArray");
	glVertexAttrib3fv = (gl_vertex_attrib_3fv *)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (gl_vertex_attrib_4fv *)wglGetProcAddress("glVertexAttrib4fv");              
#if OPENGL2_1

#if OPENGL3_0
	// OpenGL 3.0
	glClearBufferfv = (gl_clear_buffer_fv *)wglGetProcAddress("glClearBufferfv");
	glDeleteVertexArrays = (gl_delete_vertex_arrays *)wglGetProcAddress("glDeleteVertexArrays");
	glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");

#if OPENGL3_1
	// OpenGL 3.1
	glCopyBufferSubData = (gl_copy_buffer_sub_data *)wglGetProcAddress("glCopyBufferSubData");
	glGetUniformIndices = (gl_get_uniform_indices *)wglGetProcAddress("glGetUniformIndices");
	glGetActiveUniformsiv = (gl_get_active_uniforms_iv *)wglGetProcAddress("glGetActiveUniformsiv");
	glGetUniformBlockIndex = (gl_get_uniform_block_index *)wglGetProcAddress("glGetUniformBlockIndex");
	glUniformBlockBinding = (gl_uniform_block_binding *)wglGetProcAddress("glUniformBlockBinding");

#if OPENGL3_2

#if OPENGL4_0
	// OpenGL 4.0
	glPatchParameteri = (gl_patch_parameter_i *)wglGetProcAddress("glPatchParameteri");

#if OPENGL4_1

#if OPENGL4_2

#if OPENGL4_3
	// OpenGL 4.3
	glClearBufferSubData = (gl_clear_buffer_sub_data *)wglGetProcAddress("glClearBufferSubData");
	glVertexAttribBinding = (gl_vertex_attrib_binding *)wglGetProcAddress("glVertexAttribBinding");
	glBindVertexBuffer = (gl_bind_vertex_buffer *)wglGetProcAddress("glBindVertexBuffer");
	glVertexAttribFormat  = (gl_vertex_attrib_ib_format *)wglGetProcAddress("glVertexAttribFormat");
	glVertexAttribIFormat = (gl_vertex_attrib_ibi_format *)wglGetProcAddress("glVertexAttribIFormat");
	glVertexAttribLFormat = (gl_vertex_attrib_ibl_format *)wglGetProcAddress("glVertexAttribLFormat0o9");
	
#if OPENGL4_4
	// OpenGL 4.4
	glBufferStorage = (gl_buffer_storage *)wglGetProcAddress("glBufferStorage");

#if OPENGL4_5
	// OpenGL 4.5
	glCreateVertexArrays = (gl_create_vertex_arrays *)wglGetProcAddress("glCreateVertexArrays");
	glCreateBuffers = (gl_create_buffers*)wglGetProcAddress("glCreateBuffers");
	glNamedBufferStorage = (gl_named_buffer_storage *)wglGetProcAddress("glNamedBufferStorage");
	glMapNamedBuffer = (gl_map_named_buffer *)wglGetProcAddress("glMapNamedBuffer");
	glUnmapNamedBuffer = (gl_unmap_named_buffer *)wglGetProcAddress("glUnmapNamedBuffer");
	glMapNamedBufferRange = (gl_map_named_buffer_range *)wglGetProcAddress("glMapNamedBufferRange");
	glClearNamedBufferSubData = (gl_clear_named_buffer_sub_data *)wglGetProcAddress("glClearNamedBufferSubData");
	glCopyNamedBufferSubData = (gl_copy_named_buffer_sub_data *)wglGetProcAddress("glCopyNamedBufferSubData");
	glNamedBufferSubData = (gl_named_buffer_sub_data *)wglGetProcAddress("glNamedBufferSubData");
	glVertexArrayAttribBinding = (gl_vertex_array_attrib_binding *)wglGetProcAddress("glVertexArrayAttribBinding");
	glVertexArrayVertexBuffer = (gl_vertex_array_vertex_buffer *)wglGetProcAddress("glVertexArrayVertexBuffer");
	glVertexArrayAttribFormat = (gl_vertex_array_attrib_ib_format *)wglGetProcAddress("glVertexArrayAttribFormat");
	glVertexArrayAttribIFormat = (gl_vertex_array_attrib_ibi_format *)wglGetProcAddress("glVertexArrayAttribIFormat");
	glVertexArrayAttribLFormat = (gl_vertex_array_attrib_ibl_format *)wglGetProcAddress("glVertexArrayAttribLFormat");
	glEnableVertexArrayAttrib = (gl_enable_vertex_array_attrib *)wglGetProcAddress("glEnableVertexArrayAttrib");
	glDisableVertexArrayAttrib = (gl_disable_vertex_array_attrib *)wglGetProcAddress("glDisableVertexArrayAttrib");
#endif // OpenGL 4.5
#endif // OpenGL 4.4
#endif // OpenGL 4.3
#endif // OpenGL 4.2
#endif // OpenGL 4.1
#endif // OpenGL 4.0
#endif // OpenGL 3.2
#endif // OpenGL 3.1
#endif // OpenGL 3.0
#endif // OpenGL 2.1
#endif // OpenGL 2.0
}

#endif
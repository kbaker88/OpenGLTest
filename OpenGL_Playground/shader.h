#ifndef SHADER_H
#define SHADER_H

#define STD140 0
#define DEFAULT_UNIFORM 0

#define TESSELLATION_CONTROL_SHADER 0
#define TESSELLATION_EVAL_SHADER 0
#define GEOMETRY_SHADER 0

#define VERTEX_TO_FRAGMENT 1 //TODO: Remove, temporary

static const char* VertexShader_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	"layout (location = 0) in vec3 position;						\n"
	"layout (location = 1) in vec3 color;							\n"
	"																\n"
#if VERTEX_TO_FRAGMENT
	"out VS_OUT														\n"
	"{																\n"
	"	vec3 color;													\n"
	"} vs_out;														\n"
	"																\n"
#endif // VERTEX_TO_FRAGMENT
#if STD140
	"layout (std140) uniform TestManualBlock						\n"
	"{																\n"
	"	float scale;												\n"
	"	vec3 translation;											\n"
	"	float rotation[3];											\n"
	"	mat4 projection_matrix										\n"
	"}																\n"
#elif DEFAULT_UNIFORM
	"uniform TransformBlock											\n"
	"{																\n"
	"	float scale;												\n"
	"	vec3 translation;											\n"
	"	float rotation[3];											\n"
	"	mat4 projection_matrix										\n"
	"}																\n"
#endif // STD140 || DEFAULT_UNIFORM
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"																\n"
#if VERTEX_TO_FRAGMENT
	"	vs_out.color = color;										\n"
#endif // VERTEX_TO_FRAGMENT
	"gl_Position = vec4(position, 1.0);								\n"
	"}																\n"
};

#if TESSELLATION_CONTROL_SHADER
static const char* TessellationControl_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	"layout (vertices = 3) out;										\n"
	"																\n"
	"void main (void)												\n"
	"{																\n"
	"	if (gl_InvocationID == 0)									\n"
	"	{															\n"
	"		gl_TessLevelInner[0] = 5.0;								\n"
	"		gl_TessLevelOuter[0] = 5.0;								\n"
	"		gl_TessLevelOuter[1] = 5.0;								\n"
	"		gl_TessLevelOuter[2] = 5.0;								\n"
	"	}															\n"
	"																\n"
	"	gl_out[gl_InvocationID].gl_Position =						\n"
	"		gl_in[gl_InvocationID].gl_Position;						\n"
	"																\n"
	"}																\n"
};
#endif // TESSELLATION_CONTROL_SHADER

#if TESSELLATION_EVAL_SHADER
static const char* TessellationEval_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	"layout (triangles, equal_spacing, cw) in;						\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +		\n"
	"				   gl_TessCoord.y * gl_in[1].gl_Position +		\n"
	"				   gl_TessCoord.z * gl_in[2].gl_Position);		\n"
	"}																\n"
};
#endif // TESSELLATION_EVAL_SHADER

#if GEOMETRY_SHADER
static const char* Geometry_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	"layout (triangles) in;											\n"
	"layout (points, max_vertices = 3) out;							\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"	int i;														\n"
	"	for (i = 0; i < gl_in.length(); i++)						\n"
	"	{															\n"
	"		gl_Position = gl_in[i].gl_Position;						\n"
	"		EmitVertex();											\n"
	"	}															\n"
	"}																\n"
};
#endif // GEOMETRY_SHADER

static const char* FragmentShader_Source[] =
{
	"#version 450 core												\n"
	"																\n"
#if VERTEX_TO_FRAGMENT
	"in VS_OUT														\n"
	"{																\n"
	"	vec3 color;													\n"
	"} fs_in;														\n"
#endif // VERTEX_TO_FRAGMENT
	"																\n"
	"out vec4 color;												\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
#if VERTEX_TO_FRAGMENT
	"	color = vec4(fs_in.color, 1.0);								\n"
#else 
	"	color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,		\n"
	"			cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,				\n"
	"			sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),\n"
	"			1.0);												\n"
#endif // VERTEX_TO_FRAGMENT
	"}																\n"
};

#endif
#ifndef SHADER_H
#define SHADER_H

static const char* VertexShader_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	"layout (location = 0) in vec4 offset;							\n"
	"layout (location = 1) in vec4 color;							\n"
	"																\n"
	//"out VS_OUT														\n"
	//"{																\n"
	//"	vec4 color;													\n"
	//"} vs_out;														\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"const vec4 vertices[3] = vec4[3] (vec4(0.25, -0.25, 0.5, 1.0),	\n"
	"vec4(-0.25, -0.25, 0.5, 1.0),									\n"
	"vec4(0.25, 0.25, 0.5, 1.0));									\n"
	"																\n"
	"	gl_Position = vertices[gl_VertexID] + offset;				\n"
	//"	vs_out.color = color;										\n"
	"}																\n"
};

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

static const char* FragmentShader_Source[] =
{
	"#version 450 core												\n"
	"																\n"
	//	"in VS_OUT														\n"
	//	"{																\n"
	//	"	vec4 color;													\n"
	//	"} fs_in;														\n"
	"																\n"
	"out vec4 color;												\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	//	"	color = fs_in.color;										\n"
	"	color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,		\n"
	"			cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,				\n"
	"			sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),	\n"
	"			1.0);												\n"
	"}																\n"
};

#endif
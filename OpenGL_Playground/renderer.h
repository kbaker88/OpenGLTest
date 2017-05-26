#ifndef RENDERER_H
#define RENDERER_H

struct RenderObj  // Byte Total: 26
{
	// TODO: Is it better to pass as one big buffer?
	unsigned int* BufferID;					// Bytes 4
	unsigned int VertexArrayID, NumVertices,  // Bytes 12
		IndiceID;
};

void
RenderObj_Delete(RenderObj* Object)
{
	if (Object)
	{
		if (Object->BufferID)
		{
			delete[] Object->BufferID;
			Object->BufferID = 0;
		}
		delete Object;
		Object = 0;
	}
	else
	{
		// TODO: Error
	}
}

void
Render_CreateVertexArrays(unsigned int Amount,
	unsigned int* IDArray)
{
#if OPENGL4_5
	glCreateVertexArrays(Amount, IDArray);
#endif // OpenGL 4.5
}

void
Render_CreateBuffers(unsigned int Amount, 
	unsigned int* BufferIDArray)
{
#if OPENGL4_5
	glCreateBuffers(Amount, BufferIDArray);
#elif OPENGL4_3
	glGenBuffers(Amount, BufferIDArray);
#endif// OpenGL 4.5
}

void
Render_FillBuffer(unsigned int BufferID, int Size,
	float* Data, unsigned int Flags)
{
#if OPENGL4_5
	// NOTE: Immutable storage. The data buffer cannot be resized.
	glNamedBufferStorage(BufferID, Size, Data, Flags);
	//NOTE: Resizable.
	//glNamedBufferData();
#elif OPENGL2_0
	glBindBuffer(GL_ARRAY_BUFFER, BufferID);
	glBufferData(GL_ARRAY_BUFFER, &Size, Data, Flags);
#endif // OpenGL 4.5
}

void
Render_FillVertexArrayData(unsigned int VertexArrayObject,
	unsigned int BindingIndex, unsigned int AttribIndex,
	unsigned int Buffer, unsigned int SpaceOffset,
	unsigned int StartOffset)
{
#if OPENGL4_5
	glVertexArrayVertexBuffer(VertexArrayObject, BindingIndex,
		Buffer, 0, sizeof(float) * SpaceOffset);
	glVertexArrayAttribFormat(VertexArrayObject, AttribIndex,
		SpaceOffset, GL_FLOAT, GL_FALSE, StartOffset);
	glVertexArrayAttribBinding(VertexArrayObject, AttribIndex,
		BindingIndex);
	// TODO: Find when to disable or if it is even worth doing.
	glEnableVertexArrayAttrib(VertexArrayObject, BindingIndex);
#elif OPENGL2_0
	
#endif // OpenGL 4.5
}

void
Render_FillVertexArrayIndices(RenderObj* RenderObject)
{
#if OPENGL2_0
#if OPENGL3_0
	glBindVertexArray(RenderObject->VertexArrayID);
#endif // OpenGL 3.0
	glGenBuffers(1, &RenderObject->IndiceID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderObject->IndiceID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	RenderObject->IndiceCount * sizeof(unsigned int),
	//	RenderObject->IndiceData,
	//	GL_STATIC_DRAW);
#if OPENGL3_0
	glBindVertexArray(0);
#endif // OpenGL 3.0
#endif // OpenGL 2.0
}

void
Render_FillVetexArrayObject(RenderObj* RenderObject,
	unsigned int NumberAttribs, unsigned int* Offsets)
{
	//glBindVertexArray(RenderObject->VertexArrayID);
	for (unsigned int i = 0; i < NumberAttribs; i++)
	{
		Render_FillVertexArrayData(RenderObject->VertexArrayID,
			i, i, RenderObject->BufferID[i], Offsets[i], 0);
	}
	//glBindVertexArray(0);
}

void
Render_BindVertexArray(unsigned int VertexArrayObject)
{
#if OPENGL3_0
	glBindVertexArray(VertexArrayObject);
#endif // OpenGL 3.0
}

void
Render_Draw(RenderObj* RenderObject, GLuint TextureID)
{
#if OPENGL2_0
#if OPENGL3_0
	glBindVertexArray(RenderObject->VertexArrayID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
#endif // OpenGL 3.0
	glDrawArrays(GL_TRIANGLES, 0, RenderObject->NumVertices);
#if OPENGL3_0
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
#endif // OpenGL 3.0
#endif // OpenGL 2.0
}

void
RenderObj_CreateRenderObject(RenderObj* RenderObject, Model* ModelObj,
	unsigned int NumVertices)
{
	RenderObject->BufferID = new unsigned int[ModelObj->NumAttribs];
	//RenderObject->BufferID = Mem_Allocate(RenderObject->BufferID,
	//	ModelObj->NumAttribs);
	RenderObject->NumVertices = NumVertices;
	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObject->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs,
		RenderObject->BufferID);
	for (unsigned int Index = 0; Index < ModelObj->NumAttribs; Index++)
	{
		Render_FillBuffer(RenderObject->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObject, ModelObj->NumAttribs,
		ModelObj->ArrayOffset);
}

#endif
#ifndef RENDERER_H
#define RENDERER_H

struct Model
{
	float** Data;
	float* IndiceData;
	unsigned int* ArraySize;
	unsigned int NumAttribs;
};

struct RenderObj  // Byte Total: 26
{
	// TODO: Is it better to pass as one big buffer?
	unsigned int* BufferID;					// Bytes 4
	unsigned int VertexArrayID, NumVertices,  // Bytes 12
		IndiceID;
};

void
Render_CreateVertexArrays(unsigned int Amount,
	unsigned int* IDArray)
{

	glCreateVertexArrays(Amount, IDArray);
}

void
Render_CreateBuffers(unsigned int Amount, 
	unsigned int* BufferIDArray)
{
	glCreateBuffers(Amount, BufferIDArray);
}

void
Render_FillBuffer(unsigned int BufferID, unsigned int Size,
	float* Data, unsigned int Flags)
{
	glNamedBufferStorage(BufferID, Size, Data, Flags);
}

void
Render_FillVertexArrayData(unsigned int VertexArrayObject,
	unsigned int BindingIndex, unsigned int AttribIndex,
	unsigned int Buffer, unsigned int SpaceOffset,
	unsigned int StartOffset)
{
	glVertexArrayVertexBuffer(VertexArrayObject, BindingIndex,
		Buffer, 0, sizeof(float) * SpaceOffset);
	glVertexArrayAttribFormat(VertexArrayObject, AttribIndex,
		SpaceOffset, GL_FLOAT, GL_FALSE, StartOffset);
	glVertexArrayAttribBinding(VertexArrayObject, AttribIndex,
		BindingIndex);
	// TODO: Find when to disable or if it is even worth doing.
	glEnableVertexArrayAttrib(VertexArrayObject, BindingIndex);
}

void
Render_FillVertexArrayIndices(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderObject->IndiceID);
	glBindVertexArray(0);
}

void
Render_FillVetexArrayObject(RenderObj* RenderObject,
	unsigned int NumberAttribs)
{
	for (unsigned int i = 0; i < NumberAttribs; i++)
	{
		Render_FillVertexArrayData(RenderObject->VertexArrayID,
			0, 0, RenderObject->BufferID[0], 3, 0);
	}
}

void
Render_BindVertexArray(unsigned int VertexArrayObject)
{
	glBindVertexArray(VertexArrayObject);
}

void
Render_Draw(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawArrays(GL_TRIANGLES, 0, RenderObject->NumVertices);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void
RenderObj_CreateRenderObject(RenderObj* RenderObject, Model* ModelObj)
{
	//RenderObject->BufferID = new unsigned int[ModelObj->NumAttribs];
	RenderObject->BufferID = Mem_Allocate(RenderObject->BufferID,
		ModelObj->NumAttribs);
	RenderObject->NumVertices = 3;
	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObject->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs,
		RenderObject->BufferID);
	for (unsigned int Index = 0; Index < ModelObj->NumAttribs; Index++)
	{
		Render_FillBuffer(RenderObject->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObject, ModelObj->NumAttribs);
}

#endif
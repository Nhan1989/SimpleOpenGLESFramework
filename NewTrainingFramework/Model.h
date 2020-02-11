#pragma once
#include "../Utilities/utilities.h"
#include "Vertex.h"
#include <vector>
#include "Heighmap.h"
class Model
{
public:
	Model(const char* filePath);
	~Model();

	GLuint GetVertexBuffer();
	GLuint GetIndicesBuffer();
	int GetIndicesCount();
	void UpdateHeightMap(std::vector<float> heightMap,int row, int colum);

private:
	void LoadModel(const char* filePath);
	void GenBuffer();
	void UpdateVertexBuffer();

	GLuint m_VertexBuffer;
	GLuint m_IndicesBuffer;
	Vertex* m_VerticesData;
	int m_VertexCount;
	int* m_IndicesData;
	int m_IndicesCount;
};
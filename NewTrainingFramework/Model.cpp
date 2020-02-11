#include "stdafx.h"
#include "Model.h"
#include "Globals.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
Model::Model(const char* filePath):m_IndicesBuffer(0)
				, m_VertexBuffer(0)
				, m_IndicesCount(0)
				, m_VertexCount(0)
{
	LoadModel(filePath);
	GenBuffer();
}

Model::~Model()
{
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_IndicesBuffer);

	SAFE_DEL_ARRAY(m_VerticesData);
	SAFE_DEL_ARRAY(m_IndicesData);
}

void Model::UpdateHeightMap(std::vector<float> heightMap, int row, int col)
{
	for (int i = 0; i < m_VertexCount; i++)
	{
		glm::vec2 uv = m_VerticesData[i].uv;

		int wUnit = (int)((row - 1) * uv.x);
		int hUnit = (int)((col - 1) * uv.y);

		int k = col * ((row - 1) - hUnit) + wUnit;

		m_VerticesData[i].pos.y = heightMap.at(k);
	}

	UpdateVertexBuffer();
}

void Model::LoadModel(const char* filePath)
{
	std::fstream myFile;

	std::string str(filePath);
	str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
	myFile.open(str.c_str());

	if (myFile.is_open())
	{
		std::string line;
		char* verticesFormat = "%*s pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];";
		char* indicesFormat = "   %*s    %d,    %d,    %d";

		std::getline(myFile, line);

		
		sscanf(line.c_str(), "NrVertices: %d", &m_VertexCount);
		m_VerticesData = new Vertex[m_VertexCount];
		for (int i = 0; i < m_VertexCount; i++)
		{
			Vertex v;// = new Vertex();

			std::getline(myFile, line);
			sscanf(line.c_str(), verticesFormat
				, &v.pos.x
				, &v.pos.y
				, &v.pos.z
				, &v.norm.x
				, &v.norm.y
				, &v.norm.z
				, &v.binorm.x
				, &v.binorm.y
				, &v.binorm.z
				, &v.tgt.x
				, &v.tgt.y
				, &v.tgt.z
				, &v.uv.x
				, &v.uv.y);
			m_VerticesData[i] = v;
		}

		std::getline(myFile, line);

		sscanf(line.c_str(), "NrIndices: %d", &m_IndicesCount);
		
		m_IndicesData = new int[m_IndicesCount];
		for (int i = 0; i < m_IndicesCount / 3; i++)
		{
			int v1, v2, v3;
			std::getline(myFile, line);
			int index = i * 3;
			sscanf(line.c_str(), indicesFormat
				, &v1
				, &v2
				, &v3);
			m_IndicesData[index] = v1;
			m_IndicesData[index + 1] = v2;
			m_IndicesData[index + 2] = v3;
		}
	}
	myFile.close();
}

void Model::GenBuffer()
{
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_VertexCount, m_VerticesData,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	glGenBuffers(1, &m_IndicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_IndicesCount, m_IndicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

void Model::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_VertexCount, m_VerticesData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Model::GetVertexBuffer()
{
	return m_VertexBuffer;
}

GLuint Model::GetIndicesBuffer()
{
	return m_IndicesBuffer;
}
int Model::GetIndicesCount()
{
	return m_IndicesCount;
}
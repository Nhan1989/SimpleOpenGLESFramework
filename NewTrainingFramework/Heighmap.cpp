#include "stdafx.h"
#include "Heighmap.h"
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
Heightmap::Heightmap()
{

}

int Heightmap::numRows()const
{
	return 128;
}

int Heightmap::numCols()const
{
	return 128;
}

std::vector<float> Heightmap::GetHeightMap()
{
	return mHeightMap;
}

void Heightmap::loadRAW(int m, int n, char* filename,
	float heightScale, float heightOffset)
{

	std::string tmpStr;
	tmpStr = filename;

	tmpStr.erase(std::remove(tmpStr.begin(), tmpStr.end(), '"'), tmpStr.end());

	mHeightMapFilename = filename;
	mHeightScale = heightScale;
	mHeightOffset = heightOffset;
	printf("height map  %s \n", filename);
	// A height for each vertex
	std::vector<unsigned char> in(m * n);

	// Open the file.
	std::ifstream inFile;
	inFile.open(tmpStr.c_str(), std::ios_base::binary);
	//if (!inFile) HR(std::E_FAIL);

	// Read all the RAW bytes in one go.
	inFile.read((char*)&in[0], (std::streamsize)in.size());

	// Done with file.
	inFile.close();

	// Copy the array data into a float table format, and scale
	// and offset the heights.

	
	
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int k = i * n + j;
			float heigh = (float)in[k] *
				heightScale +
				heightOffset;
			//printf("height=%.2f  -- ", heigh);
			mHeightMap.push_back(heigh);
		}
	}
	//printf("size of heightmap %d\n", mHeightMap.size());
	//filter3x3();
	
}
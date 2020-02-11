#pragma once
#include <string>
#include <vector>
class Heightmap
{
public:
	Heightmap();
	Heightmap(int m, int n);
	Heightmap(int m, int n,
		const std::string& filename,
		float heightScale, float heightOffset);

	void recreate(int m, int n);

	void loadRAW(int m, int n,
		char *filename,
		float heightScale, float heightOffset);

	int numRows()const;
	int numCols()const;

	// For non-const objects
	float& operator()(int i, int j);

	// For const objects
	const float& operator()(int i, int j)const;

	std::vector<float> GetHeightMap();

private:
	bool  inBounds(int i, int j);
	float sampleHeight3x3(int i, int j);
	void  filter3x3();
private:
	char*  mHeightMapFilename;
	//char**     mHeightMap;
	std::vector<float> mHeightMap;
	float        mHeightScale;
	float        mHeightOffset;
};
/*
Here lies the code responsible for the generation fo the map and the textures.
it uses Perlin Noise to create a natural heightmap. 
we then look at the heightmap and mark heights up to a certain point to be water.
and heights above a certain point to be mountiantops covered in snow.
Somewhere in the middle lies fertile land.
*/


#pragma once
#include <raylib.h>
#include <vector>
#include "GridCell.h"
#include <stb_perlin.h>
#include "MouseInput.h"

#define GRID_SIZE_Y 300
#define GRID_SIZE_X 300
class GridMap
{
public:
	GridMap() //constructor should initialize a new gridmap for the new game/ area and procedurally generate the gridcells using random values. 
	{
		ResetPerlinTexture();
		ResetGridMapArray();
		//resize the grid because the grid might change in later itereations of the code. current there is no need for this
	
		
	};

	~GridMap() //destructor should free the memory used by the gridmap. 
	{
		UnloadImage(PerlinNoiseImage);
		UnloadTexture(PerlinNoiseTexture);
	};

	void ResetPerlinTexture();
	void ResetGridMapArray();
	void DrawPerlinTexture();

	void ExportMapTexture();

	void DrawMap(int tileWidth, int tileHeight,MouseInput& mouse);
	void IsometricDrawMap(int tileWidth, int tileHeight, MouseInput& mouse);
	const Texture2D* GetPerlinTexture() const;


private:
	std::vector<GridCell> GridMapArray;;// 30 by 30 map of GridCells
	Image PerlinNoiseImage;
	Texture PerlinNoiseTexture;
};

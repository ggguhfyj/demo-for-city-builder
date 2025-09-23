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
#include "MouseInput.h"

#define GRID_SIZE_Y 30
#define GRID_SIZE_X 30
#define TILE_WIDTH 64
#define TILE_HEIGHT 32
#define TILE_WIDTH_HALF 32
#define TILE_HEIGHT_HALF 16


class GridMap
{
public:
	GridMap() //constructor should initialize a new gridmap for the new game/ area and procedurally generate the gridcells using random values. 
	{
		ResetPerlinTexture();
		ResetGridMapArray();
		ResetIsoGridMapArray();
		

		//resize the grid because the grid might change in later itereations of the code. current there is no need for this
		PlainsTexture = LoadTexture("resources/textures/IsoPlains.png");
		WaterTexture = LoadTexture("resources/textures/IsoWater.png");
		MountainTexture = LoadTexture("resources/textures/IsoMountains.png");
		SnowTexture = LoadTexture("resources/textures/IsoPeak.png");
		Max_Height = LoadTexture("resources/textures/Max_Height.png");
		IsoGridTexture[0] = WaterTexture;
		IsoGridTexture[1] = PlainsTexture;
		IsoGridTexture[2] = MountainTexture;
		IsoGridTexture[3] = SnowTexture;



		IsometricExportMap();
		IsoMapTexture = LoadTexture("IsoMap.png");
		
	};

	~GridMap() //destructor should free the memory used by the gridmap. 
	{
		UnloadImage(PerlinNoiseImage);
		UnloadTexture(PerlinNoiseTexture);
	};
	void IsometricExportMap();
	void ResetIsoGridMapArray();
	void IsometricDrawMap();

	void ResetPerlinTexture();
	void ResetGridMapArray();
	void DrawPerlinTexture();

	void ExportMapTexture();

	

	void DrawMap(int tileWidth, int tileHeight,MouseInput& mouse);
	//void IsometricDrawMap(int tileWidth, int tileHeight, MouseInput& mouse);
	const Texture2D* GetPerlinTexture() const;


private:
	
	std::vector<GridCell> IsoGridMapArray;
	Texture IsoGridTexture[4];
	std::vector<GridCell> GridMapArray;// 30 by 30 map of GridCells
	Image PerlinNoiseImage;
	Texture PerlinNoiseTexture;

	Texture PlainsTexture;
	Texture WaterTexture;
	Texture MountainTexture;
	Texture SnowTexture;
	Texture Max_Height;
	Texture IsoMapTexture;
};

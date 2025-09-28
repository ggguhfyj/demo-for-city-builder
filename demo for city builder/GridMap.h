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


#define GRID_SIZE_Y 31
#define GRID_SIZE_X 31

#define GRID_SIZE_Y_HALF (15)
#define GRID_SIZE_X_HALF (15)

#define TILE_WIDTH 64
#define TILE_HEIGHT 32
#define TILE_WIDTH_HALF 32
#define TILE_HEIGHT_HALF 16


class GridMap
{
public:
	GridMap() //constructor should initialize a new gridmap for the new game/ area and procedurally generate the gridcells using random values. 
	{
		

		//resize the grid because the grid might change in later itereations of the code. current there is no need for this
		PlainsTexture = LoadTexture("resources/textures/IsoPlains.png");
		WaterTexture = LoadTexture("resources/textures/IsoWater.png");
		MountainTexture = LoadTexture("resources/textures/IsoMountains.png");
		SnowTexture = LoadTexture("resources/textures/IsoPeak.png");
		SelectTexture = LoadTexture("resources/textures/IsoSelect.png");
		Max_Height = LoadTexture("resources/textures/Max_Height.png");
		IsoGridTexture[0] = WaterTexture;
		IsoGridTexture[1] = PlainsTexture;
		IsoGridTexture[2] = MountainTexture;
		IsoGridTexture[3] = SnowTexture;



		ResetPerlinTexture();
		ResetIsoGridMapArray();

		IsometricExportOriginalMap();

		InitializeIsoView();

		IsometricExportMap(1); // generate initial map
		IsometricExportMap(2); // generate initial map
		IsometricExportMap(3); // generate initial map
		IsometricExportMap(4); // generate initial map
		
		
		IsoMapTexture[0] = IsoMap1Texture;
		IsoMapTexture[1] = IsoMap2Texture;
		IsoMapTexture[2] = IsoMap3Texture;
		IsoMapTexture[3] = IsoMap4Texture;


		
	};

	~GridMap() //destructor should free the memory used by the gridmap. 
	{
		UnloadImage(PerlinNoiseImage);
		UnloadTexture(PerlinNoiseTexture);
	};




	void IsometricExportOriginalMap();
	void IsometricExportMap(int view); 

	void ResetIsoGridMapArray();
	void InitializeIsoView();


	void IsometricDrawMap(int index); // with max height as default






	void ResetPerlinTexture();

	
	const Texture2D* GetPerlinTexture() const;


private:
	void rotatePoint90(float x, float y,
		float tx, float ty,
		int angleDeg,
		float& outX, float& outY);

	std::vector<GridCell> IsoGridMapArray; // value to read and shi

	GridCell* view1[GRID_SIZE_X * GRID_SIZE_Y]; // so that we can manipulate the values stored in all the other arrays when we edit the map.
	GridCell* view2[GRID_SIZE_X * GRID_SIZE_Y];// we also use these arrays to draw the map when needed. -> schedule the map redraws to be an interval of maybe every 1 - 2 minutes 
	GridCell* view3[GRID_SIZE_X * GRID_SIZE_Y];
	GridCell* view4[GRID_SIZE_X * GRID_SIZE_Y];



	Texture IsoGridTexture[4];





	std::vector<GridCell> GridMapArray;// 30 by 30 map of GridCells
	Image PerlinNoiseImage;
	Texture PerlinNoiseTexture;

	Texture PlainsTexture;
	Texture WaterTexture;
	Texture MountainTexture;
	Texture SnowTexture;
	Texture SelectTexture;
	Texture Max_Height;


	Texture IsoMapTexture[4];
	Texture IsoMap1Texture;
	Texture IsoMap2Texture;
	Texture IsoMap3Texture;
	Texture IsoMap4Texture;
};

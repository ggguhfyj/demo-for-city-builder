#include "GridMap.h"

#include <algorithm> // for std::max
#include <vector>
#include <raymath.h>
#include <raylib.h>
#include <string>


void GridMap::rotatePoint90(float x, float y,float tx, float ty,int angleDeg,float& oX, float& oY)
{
	float dx = x - tx;
	float dy = y - ty;

	int rot = ((angleDeg % 360) + 360) % 360;

	switch (rot)
	{
	case 0: 
		oX = tx + dx;
		oY = ty + dy;
		break;
	case 90:  
		oX = tx - dy;
		oY = ty + dx;
		break;
	case 180:
		oX = tx - dx;
		oY = ty - dy;
		break;
	case 270: 
		oX = tx + dy;
		oY = ty - dx;
		break;
	case 360:
		oX = tx + dx;
		oY = ty + dy;
		break;
	}
}

void GridMap::IsometricExportOriginalMap()
{

	int offsetupper = static_cast<int>(IsoGridMapArray[0].getHeight());
	RenderTexture2D target = LoadRenderTexture(TILE_WIDTH * GRID_SIZE_X, TILE_HEIGHT * GRID_SIZE_Y + offsetupper + 225);

	BeginTextureMode(target);

	int originX = ((TILE_WIDTH / 2) * GRID_SIZE_X) - TILE_WIDTH / 2;
	int originY = offsetupper;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {

		for (int x = 0; x <= sum; ++x) {
			int y = sum - x;

			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y) {
				int screenX = originX + (x - y) * TILE_WIDTH_HALF;
				int screenY = originY + (x + y) * TILE_HEIGHT_HALF;

				int count = y * GRID_SIZE_X + x;
				double height = IsoGridMapArray[count].getHeight();

				int current_texture = 3;
				if (height < 205) current_texture = 2;
				if (height < 150) current_texture = 1;
				if (height < 75)  current_texture = 0;

				DrawTexture(IsoGridTexture[current_texture], screenX, screenY - height, WHITE);
				DrawTexture(Max_Height, screenX, screenY - height, WHITE);
			}
		}
	}

	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	ImageFlipVertical(&image);
	
	ExportImage(image, "original.png");


}

void GridMap::IsometricExportMap(int view)
{
	GridCell** selectedView = nullptr;
	Texture2D* outputTexture = nullptr;
	const char* filename = nullptr;

	switch (view) {
	case 1: selectedView = view1; outputTexture = &IsoMap1Texture; break;
	case 2: selectedView = view2; outputTexture = &IsoMap2Texture; break;
	case 3: selectedView = view3; outputTexture = &IsoMap3Texture; break;
	case 4: selectedView = view4; outputTexture = &IsoMap4Texture; break;
	default: return;
	}

	int offsetupper = static_cast<int>(selectedView[0]->getHeight());
	RenderTexture2D target = LoadRenderTexture(TILE_WIDTH * GRID_SIZE_X, TILE_HEIGHT * GRID_SIZE_Y + offsetupper + 225);

	BeginTextureMode(target);

	int originX = ((TILE_WIDTH / 2) * GRID_SIZE_X) - TILE_WIDTH / 2;
	int originY = offsetupper;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {

		for (int x = 0; x <= sum; ++x) {
			int y = sum - x;

			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y) {
				int screenX = originX + (x - y) * TILE_WIDTH_HALF;
				int screenY = originY + (x + y) * TILE_HEIGHT_HALF;

				int count = y * GRID_SIZE_X + x;
				double height = selectedView[count]->getHeight();

				int current_texture = 3;
				if (height < 205) current_texture = 2;
				if (height < 150) current_texture = 1;
				if (height < 75)  current_texture = 0;

				DrawTexture(IsoGridTexture[current_texture], screenX, screenY-height, WHITE);
				DrawTexture(Max_Height, screenX, screenY - height, WHITE);
			}
		}
	}

	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	ImageFlipVertical(&image);
	if (outputTexture->id != 0) {
		UnloadTexture(*outputTexture);
	}
	*outputTexture = LoadTextureFromImage(image);
}


//reads the x and y values and stores them in the array in such a way that you could simple walk down the array using count and draw then in the order of painters perspective
void GridMap::ResetIsoGridMapArray()
{

	IsoGridMapArray.resize(GRID_SIZE_X * GRID_SIZE_Y);

	int x = 0;
	int y = 0;
	int row = 1;
	int last_row = 0;
	int count = 0;	
	
	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {

		for (int x = 0; x <= sum; ++x) {

			int y = sum - x;

			if (x < GRID_SIZE_X && y < GRID_SIZE_Y) {

				int count = y * GRID_SIZE_X + x;

				unsigned char Height = GetImageColor(PerlinNoiseImage, x, y).r;

				IsoGridMapArray[count] = GridCell{ (double)Height,x,y };

				//std::cout << "(" << x << "," << y << ")" << " " << static_cast<int>(Height) << std::endl;
			}

		}

	}

}
// this should produce something very similar to reset iso grid 
// read the values in count. -> push them onto the new array in correct positions so that painters perspective will be kept
// the way to do this is to 1. get x/y value of count = 0
//void GridMap::InitializeIsoView()
//{
//	int i = 0;
//	float new_x = 0;
//	float new_y = 0;
//	int count = 0;
//
//	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {
//		for (int x = 0; x <= sum; ++x) {
//
//			int y = sum - x;
//			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y) {
//				 
//				new_x = x;
//				new_y = y;
//				count = (int)(new_y * GRID_SIZE_X + new_x);
//				view1[count] = &IsoGridMapArray[i];
//				//std::cout << "view 1 (" << x << "," << y << ")" << "translated to  " << "(" << view1[i]->x << ", " << view1[i]->y<< ")" << std::endl;
//
//				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 90, new_x, new_y);
//				count = (int)new_y * GRID_SIZE_X + (int)new_x;
//				view2[count] = &IsoGridMapArray[i];
//				std::cout << "original (" << x << "," << y << ")" << "translated to  " << "(" << view2[count]->x << ", " << view2[count]->y << ") at " << count << std::endl;
//
//				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 180, new_x, new_y);
//				count = (int)new_y * GRID_SIZE_X + (int)new_x;
//				view3[count] = &IsoGridMapArray[i];
//				//std::cout << "view 3 (" << x << "," << y << ")" << "translated to  " << "(" << view3[i]->x << ", " << view3[i]->y << ")" << std::endl;
//
//				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 270, new_x, new_y);
//				count = (int)new_y * GRID_SIZE_X + (int)new_x;
//				view4[count] = &IsoGridMapArray[i];
//				//std::cout << "view 4 (" << x << "," << y << ")" << "translated to  " << "(" << view4[i]->x << ", " << view4[i]->y << ")" << std::endl;
//
//				i++;
//			}
//			
//		}
//	}
//
//
//}
void GridMap::InitializeIsoView() // chat gpt version, based on the version above, I dunno what I did wrong lol
{
	float new_x = 0;
	float new_y = 0;

	for (int sum = 0; sum < GRID_SIZE_X + GRID_SIZE_Y - 1; ++sum) {
		for (int x = 0; x <= sum; ++x) {
			int y = sum - x;

			if (x >= 0 && x < GRID_SIZE_X && y >= 0 && y < GRID_SIZE_Y) {

				// row-major index of original cell
				int baseIndex = y * GRID_SIZE_X + x;

				// --- 0° view (original) ---
				view1[baseIndex] = &IsoGridMapArray[baseIndex];

				// --- 90° rotation ---
				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 90, new_x, new_y);
				int rotIndex90 = static_cast<int>(new_y) * GRID_SIZE_X + static_cast<int>(new_x);
				if (rotIndex90 >= 0 && rotIndex90 < GRID_SIZE_X * GRID_SIZE_Y)
					view2[rotIndex90] = &IsoGridMapArray[baseIndex];

				// --- 180° rotation ---
				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 180, new_x, new_y);
				int rotIndex180 = static_cast<int>(new_y) * GRID_SIZE_X + static_cast<int>(new_x);
				if (rotIndex180 >= 0 && rotIndex180 < GRID_SIZE_X * GRID_SIZE_Y)
					view3[rotIndex180] = &IsoGridMapArray[baseIndex];

				// --- 270° rotation ---
				rotatePoint90(x, y, GRID_SIZE_X_HALF, GRID_SIZE_Y_HALF, 270, new_x, new_y);
				int rotIndex270 = static_cast<int>(new_y) * GRID_SIZE_X + static_cast<int>(new_x);
				if (rotIndex270 >= 0 && rotIndex270 < GRID_SIZE_X * GRID_SIZE_Y)
					view4[rotIndex270] = &IsoGridMapArray[baseIndex];
			}
		}
	}
}


void GridMap::IsometricDrawMap(int index)
{
	switch (index % 4)
	{
	case 0:
		if (IsoMap1Texture.id != 0)
		{
			DrawTexture(IsoMap1Texture, 0, 0, WHITE);
			
		}
		break;
	case 1:
		if (IsoMap2Texture.id != 0)
		{
			DrawTexture(IsoMap2Texture, 0, 0, WHITE);
		}
		break;
	case 2:
		if (IsoMap3Texture.id != 0)
		{
			DrawTexture(IsoMap3Texture, 0, 0, WHITE);
		}
		break;
	case 3:
		if (IsoMap4Texture.id != 0)
		{
			DrawTexture(IsoMap4Texture, 0, 0, WHITE);
		}
		break;

	}
	
}


void GridMap::ResetPerlinTexture()
{
	int seedX = GetRandomValue(0, 10000);
	int seedY = GetRandomValue(0, 10000);
	//std::cout << seedX << " ," << seedY << std::endl;
	PerlinNoiseImage = GenImagePerlinNoise(GRID_SIZE_X, GRID_SIZE_Y, seedX, seedY, 1.0f);
	PerlinNoiseTexture = LoadTextureFromImage(PerlinNoiseImage);


	ExportImage(PerlinNoiseImage, "PerlinNoise.png");

}
const Texture2D* GridMap::GetPerlinTexture() const// so we dont copy the texture
{
	return &PerlinNoiseTexture;
}


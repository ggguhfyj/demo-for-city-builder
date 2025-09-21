#include "GridMap.h"

#include <algorithm> // for std::max
#include <vector>
#include <raymath.h>
#include <raylib.h>
void GridMap::DrawMap(int tileWidth, int tileHeight, MouseInput& mouse)
{

	mouse.MouseDrag();
	mouse.MouseScroll();

	Math::vec2 pan = mouse.GetPan();
	float zoom = mouse.GetZoom();

	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{             
			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			unsigned char r = 0, g = 0, b = 0;

			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
			else r = 255, g = 255, b = 255;

			Color color = { r, g, b, 255 };


				float normalX = (x * tileWidth) * zoom + pan.x;
				float normalY = (y * tileHeight) * zoom + pan.y;

				DrawRectangleV({ normalX, normalY }, { tileWidth * zoom, tileHeight * zoom }, color);
			
				
		}
	}

}
void GridMap::IsometricDrawMap(int tileWidth, int tileHeight, MouseInput& mouse)
{

	mouse.MouseDrag();
	mouse.MouseScroll();

	Math::vec2 pan = mouse.GetPan();
	float zoom = mouse.GetZoom();

	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			unsigned char r = 0, g = 0, b = 0;

			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
			else r = 255, g = 255, b = 255;

			Color color = { r, g, b, 255 };


			float isoX = (x - y) * (tileWidth / 2.0f) * zoom + pan.x;
			float isoY = (x + y) * (tileHeight / 2.0f) * zoom + pan.y;

		
			

			// draw as a diamond (or rectangle if you’re just testing)
			DrawRectangleV({ isoX , isoY },
				{ tileWidth * zoom, tileHeight * zoom }, color);
		


		}
	}

}
void GridMap::ExportMapTexture()
{

	float tileWidth = 10;
	float tileHeight = 10;
		
	RenderTexture2D target = LoadRenderTexture(GRID_SIZE_X* tileWidth, GRID_SIZE_Y* tileHeight);
	BeginTextureMode(target); 

	for (int y = 0; y < GRID_SIZE_Y; y++)
	{
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			double h = GridMapArray[y * GRID_SIZE_X + x].getHeight();
			unsigned char r = 0, g = 0, b = 0;

			if (h < 0.3) r = 0, g = 0, b = static_cast<unsigned char>(128 + 127 * (h / 0.3));
			else if (h < 0.6) r = 0, g = static_cast<unsigned char>(128 + 127 * ((h - 0.3) / 0.3)), b = 0;
			else if (h < 0.8) r = static_cast<unsigned char>(70 + 69 * ((h - 0.6) / 0.2)), g = 69, b = 19;
			else r = 255, g = 255, b = 255;

			Color color = { r, g, b, 255 };


		
				float normalX = (x * tileWidth);
				float normalY = (y * tileHeight);

				DrawRectangleV({ normalX, normalY }, { tileWidth, tileHeight}, color);

		}
	}
	EndTextureMode();
	Image image = LoadImageFromTexture(target.texture);
	ExportImage(image, "CreatedMap.png");
}

void GridMap::ResetPerlinTexture()
{
	int seedX = GetRandomValue(0, 10000);
	int seedY = GetRandomValue(0, 10000);
	std::cout << seedX << " ," << seedY << std::endl;
	PerlinNoiseImage = GenImagePerlinNoise(GRID_SIZE_X, GRID_SIZE_Y, seedX, seedY, 1.0f);
	PerlinNoiseTexture = LoadTextureFromImage(PerlinNoiseImage);

}

void GridMap::ResetGridMapArray()
{
	GridMapArray.resize(GRID_SIZE_X * GRID_SIZE_Y);

	for (int y = 0; y < GRID_SIZE_Y; y++)
		for (int x = 0; x < GRID_SIZE_X; x++)
		{
			unsigned char Height = GetImageColor(PerlinNoiseImage, y, x).r;
			double normalizedHeightValue = (double)Height / 255.0;
			GridMapArray[y * GRID_SIZE_X + x] = GridCell{ normalizedHeightValue }; // initialize the GridMap Array by calling the constructor of the gridcell with random but coherent values. 
		}
}

void GridMap::DrawPerlinTexture()
{
	// get mouse scroll to scale the texture
	static float scale = 1.0f;
	float scroll = GetMouseWheelMove(); // +1 or -1 per scroll tick
	scale += scroll * 0.1f;             // adjust scaling speed
	if (scale < 0.1f) scale = 0.1f;     // minimum scale
	if (scale > 10.0f) scale = 10.0f;   // maximum scale

	// original texture size
	int texWidth = PerlinNoiseTexture.width;
	int texHeight = PerlinNoiseTexture.height;

	// calculate destination rectangle centered on screen
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	Rectangle destRec;
	destRec.width = texWidth * scale;
	destRec.height = texHeight * scale;
	destRec.x = screenWidth / 2.0f - destRec.width / 2.0f;
	destRec.y = screenHeight / 2.0f - destRec.height / 2.0f;

	Rectangle sourceRec = { 0, 0, (float)texWidth, (float)texHeight };
	Vector2 origin = { 0, 0 }; // top-left origin

	DrawTexturePro(PerlinNoiseTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}


const Texture2D* GridMap::GetPerlinTexture() const// so we dont copy the texture
{
	return &PerlinNoiseTexture;
}

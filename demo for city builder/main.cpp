#include <raylib.h>

#include <iostream>

#include "GridMap.h"

#include "Mode7.h"

int main()
{


	const int screenWidth = 1000;
	const int screenHeight = 1000;
	InitWindow(screenWidth, screenHeight, "I.G.Y");
	GridMap map;
	MouseInput mouse;
	SetTargetFPS(60);  
	srand(time(NULL));
	//map.ExportMapTexture();
	Texture exportedmap = LoadTexture("CreatedMap.png");
	Mode7 mode7drawer{ exportedmap };

	bool isometric = false;
	bool Mode7Draw	 = false;
	while (!WindowShouldClose()) 
	{BeginDrawing();
		ClearBackground(RAYWHITE);

		if (IsKeyPressed(KEY_I)) isometric = !isometric;
		if (IsKeyPressed(KEY_M)) Mode7Draw = !Mode7Draw;

		if (!Mode7Draw)
		{
			if (!isometric)map.DrawMap(2, 2, mouse);
			else map.IsometricDrawMap(2, 1, mouse);
		}
		
		
		if (IsKeyPressed(KEY_SPACE))
		{

			map.ResetPerlinTexture();
			map.ResetGridMapArray();
		}
		if(Mode7Draw)
		mode7drawer.Draw();
		

		DrawText("Press I to toggle isometric view/Press M to toggle Mode7 view", 10, 10, 20, DARKGRAY);
		DrawText("Press SPACE to regenerate map", 10, 40, 20, DARKGRAY);
		DrawText("BLUE = ocean, GREEN = plains, BROW = mountains, WHITE = mountaintop snow", 10, 70, 20, DARKGRAY);
		DrawText("right click and scroll to pan and zoom", 10, 100, 20, DARKGRAY);
		DrawFPS(900, 10);
		
		
		EndDrawing();
	}
	CloseWindow();  
	return 0;
}
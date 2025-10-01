#include <raylib.h>
#include <iostream>
#include "GridMap.h"
#include "Mode7.h"
#include "CustomCell.h"
int main()
{
    const int screenWidth = 2800;
    const int screenHeight = 1400;
    InitWindow(screenWidth, screenHeight, "I.G.Y");
    SetTargetFPS(540);  
    srand(time(NULL));

    GridMap map;
    MouseInput mouse;

	int index = 4; // index for changing perspective


    while (!WindowShouldClose()) 
    {
        // change perspective with arrow keys
        

        // regenerate map with new Perlin noise
        if (IsKeyPressed(KEY_SPACE))
        {
            map.ResetPerlinTexture();
            map.ResetIsoGridMapArray();
            map.InitializeIsoView();
            map.IsometricExportMap(1);
            map.IsometricExportMap(2);
            map.IsometricExportMap(3);
            map.IsometricExportMap(4);
        }
        if (IsKeyPressed(KEY_RIGHT)) index++;
		if (IsKeyPressed(KEY_LEFT)) index--;
        map.IsometricDrawMap(index);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        


        DrawFPS(1800, 10);
       

        EndDrawing();
    }

    CloseWindow();  
    return 0;
}

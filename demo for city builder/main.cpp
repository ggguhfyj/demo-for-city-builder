#include <raylib.h>
#include <iostream>
#include "GridMap.h"
#include "Mode7.h"
#include "CustomCell.h"
int main()
{
    const int screenWidth = 2000;
    const int screenHeight = 1500;
    InitWindow(screenWidth, screenHeight, "I.G.Y");
    SetTargetFPS(60);  
    srand(time(NULL));

    GridMap map;
    MouseInput mouse;

    // Initial generation
    map.ResetPerlinTexture();
    map.ResetGridMapArray();
    map.ResetIsoGridMapArray();

 
    map.IsometricExportMap(); // generate initial map

    while (!WindowShouldClose()) 
    {
        // change perspective with arrow keys
        

        // regenerate map with new Perlin noise
        if (IsKeyPressed(KEY_SPACE))
        {
            map.ResetPerlinTexture();
            map.ResetGridMapArray();
            map.ResetIsoGridMapArray();
            map.IsometricExportMap();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        


        DrawFPS(1800, 10);

        map.IsometricDrawMap(); // draw the cached texture

        EndDrawing();
    }

    CloseWindow();  
    return 0;
}

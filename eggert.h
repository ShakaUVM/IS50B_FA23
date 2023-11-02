#include <stdbool.h>
#include "raylib.h"

void eggert(bool enabled) {
    double size = 32.0;
    
    // Define Origin
    Vector3 origin = (Vector3){ 0.0, 0.0, -size };
    
    // Draw Origin
    DrawCylinder(origin, 2.3, 1.8
    , 0.2, 20, PINK);
        
    // Draw Ground
    DrawPlane(origin, (Vector2){ size, size }, BLACK);
    
    // All Walls
    double wallHeight = 10.0;
    double wallThickness = 2.0;
    
    // South Wall
    Vector3 southWall_orig = (Vector3){0.0, wallHeight / 2.0, -(size + size / 2.0)};
    // West Wall
    Vector3 eastWall_orig = (Vector3){-size + size / 2.0, wallHeight / 2.0, -size};
    // East Wall
    Vector3 westWall_orig = (Vector3){size - size / 2.0, wallHeight / 2.0, -size};
    
    // Draw Walls
    DrawCube(southWall_orig, size, wallHeight, wallThickness, DARKGRAY);
    DrawCube(westWall_orig, wallThickness, wallHeight, size, GRAY);
    DrawCube(eastWall_orig, wallThickness, wallHeight, size, GRAY);
    
    // Random Cubes
    float maxPos_Z = 10.0f;
    float minPos_Z = 0.0f;
    

                
}
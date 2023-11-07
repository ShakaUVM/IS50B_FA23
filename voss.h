#pragma once
#include <stdbool.h>
#include "raylib.h"

void voss(bool enabled)
{
    float size = 32.0;

    // Define Origin
    Vector3 origin = (Vector3){-size, 0.0f, 0.0f};

    // Draw a cylinder shape in the middle of the floor (origin + (half size, 0, half size)) with a radius of 1.0 and a height of 2.0
    DrawCylinder(origin, 1.0f, 2.0f, 10.0f, 7, RED);

    // Draw Ground
    DrawPlane(origin, (Vector2){size, size}, BLUE);

    // Draw Walls on the sides
    float wallHeight = 10.0f;
    float wallThickness = 2.0f;

    // North Wall
    Vector3 northWall_orig = (Vector3){-size, wallHeight / 2.0, size - size / 2.0};
    // South Wall
    Vector3 southWall_orig = (Vector3){-size, wallHeight / 2.0, -(size / 2.0)};
    // East Wall
    Vector3 eastWall_orig = (Vector3){-((size / 2.0) * 3), wallHeight / 2.0, 0.0};

    // Draw Walls
    DrawCube(northWall_orig, size, wallHeight, wallThickness, GRAY);
    DrawCube(southWall_orig, size, wallHeight, wallThickness, GRAY);
    DrawCube(eastWall_orig, wallThickness, wallHeight, size, GREEN);
}
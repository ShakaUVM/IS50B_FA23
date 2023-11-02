void xiong(bool enabled) {
    DrawPlane((Vector3){ 0.0f, 0.0f, 32.0f }, (Vector2){ 32.0f, 32.0f }, DARKBLUE); //North Wing Floor
    DrawCube((Vector3){ -16.5f, 2.5f, 32.0f }, 1.0f, 5.0f, 32.0f, BLUE); //Right Wall
    DrawCube((Vector3){ 16.5f, 2.5f, 32.0f }, 1.0f, 5.0f, 32.0f, BLUE); //Left Wall
    DrawCube((Vector3){ 0.0f, 2.5f, 48.5f }, 32.0f, 5.0f, 1.0f, BLUE); //Back Wall
    
    //float heights[10] = { 0 };
    //Vector3 positions[10] = { 0 };
    
    //DrawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); Draw a cylinder/cone wires
    //DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); cylinder function
    //DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color); sphere wires function
    
    //DrawCube((Vector3){ 0.0f, 0.0f, 32.0f }, 5.0f, 1.0f, 5.0f, LIGHTGRAY); //box
    DrawCylinder((Vector3){ 0.0f, 0.0f, 32.0f } , 3.0f, 3.0f, 1.0f, 20, LIGHTGRAY);
    DrawCylinder((Vector3){ 0.0f, 0.0f, 32.0f } , 2.8f, 2.8f, 1.2f, 20, SKYBLUE);
    DrawCylinderWires((Vector3){0.0f, 0.0f, 32.0f} , 2.8f, 2.8f, 1.3f, 20, BLUE); // Draw a cylinder wires   
    DrawSphereWires(Vector3{ 0.0f, 3.5f, 32.0f }, 2.0f, 10, 10, GOLD); 
    
    /*for (int i = 0; i < 10; i++) {
        DrawCube(positions[i], 2.0f, heights[i], 2.0f, LIGHTGRAY);
        //DrawCubeWires(positions[32], 2.0f, heights[i], 2.0f, MAROON);
    }*/
 
}
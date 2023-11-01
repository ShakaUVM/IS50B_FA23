void xiong(bool enabled) {
    DrawPlane((Vector3){ 0.0f, 0.0f, 32.0f }, (Vector2){ 32.0f, 32.0f }, DARKBLUE); //North Wing Floor
    DrawCube((Vector3){ -16.5f, 2.5f, 32.0f }, 1.0f, 5.0f, 32.0f, BLUE); //Right Wall
    DrawCube((Vector3){ 16.5f, 2.5f, 32.0f }, 1.0f, 5.0f, 32.0f, BLUE); //Left Wall
    DrawCube((Vector3){ 0.0f, 2.5f, 48.5f }, 32.0f, 5.0f, 1.0f, BLUE); //Back Wall
    
    //float heights[10] = { 0 };
    //Vector3 positions[10] = { 0 };
    
    DrawCube((Vector3){ 0.0f, 0.0f, 32.0f }, 5.0f, 1.0f, 5.0f, LIGHTGRAY); //box
    
    /*for (int i = 0; i < 10; i++) {
        DrawCube(positions[i], 2.0f, heights[i], 2.0f, LIGHTGRAY);
        //DrawCubeWires(positions[32], 2.0f, heights[i], 2.0f, MAROON);
    }*/
 
}
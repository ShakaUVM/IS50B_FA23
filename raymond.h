void raymond(bool enabled, vector<Cube>& cubes, vector<Sphere>& spheres, vector<Plane>& planes) {
    
    //ORIGIN OF THE ROOM
    Vector3 origin = {32.0f, 2.5f, 0.0f};
    //OFFSETS FROM THE CENTER OF THE ROOM
    Vector3 wallOffset = {16, 0.0f, 16};
    //UNIVERSAL SCALE OF THE WALLS FOR THIS ROOM
    Vector3 wallScale = (Vector3){32.0f, 5.0f, 1.0f};
    
    //FLOOR
    Plane floor;
    floor.position = origin;
    floor.position.y -= 2.5f;
    floor.size = (Vector2){32.0f, 32.0f};
    floor.color = GOLD;

    //WALLS
    Cube wall1, wall2, wall3;
    wall1.position = origin;
    wall1.position.x += wallOffset.x;
    wall1.size = wallScale;
    float temp = wall1.size.x;
    wall1.size.x = wall1.size.z;
    wall1.size.z = temp;
    wall1.color = GREEN;
    
    wall2.position = origin;
    wall2.position.z += wallOffset.z;
    wall2.position.x += .5f;
    wall2.size = wallScale;
    wall2.color = RED;
    
    wall3.position = origin;
    wall3.position.z -= wallOffset.z;
    wall3.position.x += .5f;
    wall3.size = wallScale;
    wall3.color = BLUE;
    
    //BOXES
    Cube box1, box2;
    box1.position = origin;
    box1.size = (Vector3) {2.5f, 2.5f, 2.5f};
    box1.position.y -= origin.y - (box1.size.y / 2);
    box1.position.z -= 2;
    box1.position.x += 5;
    box1.color = GRAY;
    
    box2.position = origin;
    box2.size = (Vector3) {1, 1, 1};
    box2.position.y -= origin.y - (box2.size.y / 2);
    box2.position.z += 5;
    box2.position.x -= 4;
    box2.color = WHITE;
    
    cubes.push_back(wall1);
    cubes.push_back(wall2);
    cubes.push_back(wall3);
    cubes.push_back(box1);
    cubes.push_back(box2);
    planes.push_back(floor);
    
    
    // floor.Draw();
    // wall1.Draw();
    // wall2.Draw();
    // wall3.Draw();
    // box1.Draw();
    // box2.Draw();
    
}
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
    floor.color = GRAY;

    //WALLS
    Cube wall1, wall2, wall3;
    wall1.position = origin;
    wall1.position.x += wallOffset.x;
    wall1.size = wallScale;
    float temp = wall1.size.x;
    wall1.size.x = wall1.size.z;
    wall1.size.z = temp;
    wall1.color = DARKGRAY;
    
    wall2.position = origin;
    wall2.position.z += wallOffset.z;
    //wall2.position.x += .5f;
    wall2.size = wallScale;
    wall2.color = DARKGRAY;
    
    wall3.position = origin;
    wall3.position.z -= wallOffset.z;
    //wall3.position.x += .5f;
    wall3.size = wallScale;
    wall3.color = DARKGRAY;
    
    // BOXES
    // COMMENTED OUT FOR SIMPLICATION - MEG
    // Cube box1, box2;
    // box1.position = origin;
    // box1.size = (Vector3) {2.5f, 2.5f, 2.5f};
    // box1.position.y = box1.size.y;
    // box1.position.z -= 2;
    // box1.position.x += 5;
    // box1.color = BLACK;
    
    // box2.position = origin;
    // box2.size = (Vector3) {1, 1, 1};
    // box2.position.y = box2.size.y;
    // box2.position.z += 5;
    // box2.position.x -= 4;
    // box2.color = BLACK;

    //SPHERES - MEAGAN
    Sphere sphere1;
    sphere1.position = Vector3 {origin.x + 1, origin.y + 2, origin.z - 4};
    sphere1.radius = 1.5;
    sphere1.color = WHITE; 
    Sphere sphere2;
    sphere2.position = Vector3 {origin.x - 2, origin.y - 2, origin.z + 2};
    sphere2.radius = 2.0;
    sphere2.color = WHITE; 
    
    cubes.push_back(wall1);
    cubes.push_back(wall2);
    cubes.push_back(wall3);
    // cubes.push_back(box1); // Meg - Commented out for clarity
    // cubes.push_back(box2); // Meg - Commented out for clarity
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    planes.push_back(floor);
}
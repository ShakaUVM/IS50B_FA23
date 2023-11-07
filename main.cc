/*
    F6 to compile and run it in Notepad++
    IS50B_FA23 Project
    William Kerney
    Sean Raymond
    Meagan Eggert
    Robert Voss
    Bruce Xiong
*/

// Template Code Copyright (c) 2015-2023 Ramon Santamaria (@raysan5)

#include "main.h"



const int MAX_COLUMNS = 20;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Monitor Correction - Voss
    const int screenWidth = 3840;
    const int screenHeight = 2160;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");
    int MONITOR = GetMonitorCount() == 3 ? 2 : 0;

    SetWindowMonitor(MONITOR);
    SetWindowSize(GetMonitorWidth(MONITOR), GetMonitorHeight(MONITOR));
    
    // BEGIN MAIN CHARACTER SET UP - MEG
    float mainChar_radius = 0.25;
    Vector3 mainChar_orig = (Vector3){ 0.0, mainChar_radius, 0.0};
    Vector3 mainChar_center = mainChar_orig;
    float mainChar_speed = 0.25;
    bool hitSomething = false;
    float heightBeforeJump = mainChar_center.y;
    
    // END MAIN CHARACTER SET UP 


    // BEGIN MODEL LOADING - MEG
    // Improved Model Loading?? - Voss
    const string MODEL_OBJ_PATH = "spaceBits\\Assets\\obj\\";
    const string MODEL_TEXTURE_PATH = "spaceBits\\Assets\\textures\\";
    const string CHARACTER_PATH = "adventurers\\Characters\\gltf\\";

    // Set up vectors to store:
    // Available .obj file-names as strings to load as models
    // Available .glb file-names as strings to load as models (characters)
    vector<string> model_names = {"basemodule_A.obj", "basemodule_C.obj"};
    vector<string> character_names = {"barbarian.glb"};
    // Vector to store all of our models, character or otherwise
    vector<Model> models;

    // Load in models for every name in model_names vector
    for (size_t i = 0; i < model_names.size(); i++)
    {
        models.push_back(LoadModel((MODEL_OBJ_PATH + model_names[i]).c_str()));
    }

    // Load in models for every character in character_names vector
    for (size_t i = 0; i < character_names.size(); i++)
    {
        models.push_back(LoadModel((CHARACTER_PATH + character_names[i]).c_str()));
    }

    // Set up vectors to store:
    // Available .png file-names to load as texture files
    vector<string> model_texture_names = {"spacebits_texture.png"};
    vector<string> character_texture_names = {"barbarian_texture.png"};
    // Vector to store all of our textures
    vector<Texture2D> textures;

    // Load necessary textures for models
    for (size_t i = 0; i < model_texture_names.size(); i++)
    {
        textures.push_back(LoadTexture((MODEL_TEXTURE_PATH + model_texture_names[i]).c_str()));
    }

    // Load necessary textures for characters
    for (size_t i = 0; i < character_texture_names.size(); i++)
    {
        textures.push_back(LoadTexture((CHARACTER_PATH + character_texture_names[i]).c_str()));
    }

    // SPACE BITS
    // Space Model 1 (space1)
    float space1_scale = 0.5;
    Model space1 = models.at(0); // Voss    
    Texture2D space1_tex = textures.at(0); // Voss
    space1.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = space1_tex;
    
    // Set space1 position
    Vector3 space1_pos = {0.0f, 0.1f, -4.0f};
    // Find the edges of the model
    BoundingBox space1_bounds = GetMeshBoundingBox(space1.meshes[0]);
    // Calculate size of the model
    Vector3 space1_size = (Vector3){space1_scale * (space1_bounds.max.x - space1_bounds.min.x), space1_scale * (space1_bounds.max.y - space1_bounds.min.y), space1_scale * (space1_bounds.max.z - space1_bounds.min.z)};
    // Create bounding box based on the size and position of the model
    BoundingBox space1_bBox = (BoundingBox){(Vector3){space1_pos.x - space1_size.x / 2, space1_pos.y - space1_size.y / 2, space1_pos.z - space1_size.z / 2}, (Vector3){space1_pos.x + space1_size.x / 2, space1_pos.y + space1_size.y, space1_pos.z + space1_size.z / 2}};

    // Space Model 2 (space2)
    float space2_scale = 0.75;
    Model space2 = models.at(1); // Voss
    Texture2D space2_tex = textures.at(0); // Voss
    space2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = space1_tex;
    
    // Set space2 position
    Vector3 space2_pos = {4.5f, 0.1f, -6.0f};
    // Find the edges of the model
    BoundingBox space2_bounds = GetMeshBoundingBox(space2.meshes[0]);
    // Calculate size of the model
    Vector3 space2_size = (Vector3){space2_scale * (space2_bounds.max.x - space2_bounds.min.x), space2_scale * (space2_bounds.max.y - space2_bounds.min.y), space2_scale * (space2_bounds.max.z - space2_bounds.min.z)};
    // Create bounding box based on the size and position of the model
    BoundingBox space2_bBox = (BoundingBox){(Vector3){space2_pos.x - space2_size.x / 2, space2_pos.y - space2_size.y / 2, space2_pos.z - space2_size.z / 2}, (Vector3){space2_pos.x + space2_size.x / 2, space2_pos.y + space2_size.y, space2_pos.z + space2_size.z / 2}};

    // Barbarian (For Testing)
    // Model barbarian = LoadModel ("adventurers\\Characters\\gltf\\Barbarian.glb");
    Model barbarian = models.at(2); // Voss
    Texture2D barbarian_tex = LoadTexture("adventurers\\Characters\\gltf\\barbarian_texture.png");
    //Texture2D barbarian_tex = textures.at(1); // Voss
    Vector3 barbarian_pos = {0.0, 0.1, -10.0};

    // END MODEL LOADING

    // Initialize audio device, so you can play sounds - Meg
    InitAudioDevice();
    // This is a cool laser sound
    Sound laser = LoadSound("laser.wav");
    Music bgMusic = LoadMusicStream("Space Music2.mp3");
    PlayMusicStream(bgMusic); // Play Background Music

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = {0};
    //camera.position = (Vector3){0.0f, 2.0f, 4.0f}; // Camera position
    camera.position = (Vector3){mainChar_center.x, mainChar_center.y + mainChar_radius * 2, mainChar_center.z + 2}; // Camera position
    camera.target = (Vector3){mainChar_center.x, mainChar_center.y, mainChar_center.z - 2};   // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                           // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera projection type

    int cameraMode = CAMERA_THIRD_PERSON;
    
    // stolen from a github repo. doesn't currently work
    // setup initial camera data
    /*
    rlTPCamera orbitCam;
    rlTPCameraInit(&orbitCam, 45, (Vector3){ 1, 0 ,0 });
    orbitCam.ViewAngles.y = -15 * DEG2RAD;
    */
    
    // Generates some random columns
    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};

    /* Making a Random Map
    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){(float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15)};
        colors[i] = (Color){GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255};
    }
    */

    DisableCursor(); // Limit cursor to relative movement inside the window
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------



// BEGIN MAIN LOOP 
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        hitSomething = false;
        
        UpdateMusicStream(bgMusic);
        //rlTPCameraBeginMode3D(&orbitCam);

        // Press 'k' to play a laser sound
        if (IsKeyPressed(KEY_K))
        {
            PlaySound(laser);
        }
        
        // WASD Control for the Main Character
        if(IsKeyDown(KEY_W)) {
            mainChar_center.z -= mainChar_speed;
            hitSomething = CheckCollisionBoxSphere(space1_bBox, mainChar_center, mainChar_radius);
            if (hitSomething){
                mainChar_center.z += mainChar_speed;
            }
        }
        if(IsKeyDown(KEY_A)) {
            mainChar_center.x -= mainChar_speed;
            hitSomething = CheckCollisionBoxSphere(space1_bBox, mainChar_center, mainChar_radius);
            if (hitSomething){
                mainChar_center.x += mainChar_speed;
            }
        }
        if(IsKeyDown(KEY_S)) {
            mainChar_center.z += mainChar_speed;
            hitSomething = CheckCollisionBoxSphere(space1_bBox, mainChar_center, mainChar_radius);
            if (hitSomething){
                mainChar_center.z -= mainChar_speed;
            }
        }
        if(IsKeyDown(KEY_D)) {
            mainChar_center.x += mainChar_speed;
            hitSomething = CheckCollisionBoxSphere(space1_bBox, mainChar_center, mainChar_radius);
            if (hitSomething){
                mainChar_center.x -= mainChar_speed;
            }
        }
        // Jumping for Main Character
        // Very Basic
        // Only works for the ground level and actually lets you stay in flight if you hold SPACE
        if(IsKeyPressed(KEY_SPACE)){
            mainChar_center.y += 1.0;
        }
        if(IsKeyReleased(KEY_SPACE)){
            while (mainChar_center.y > mainChar_radius){
                mainChar_center.y -= 0.10;
            }
        }
        
        // Check to see if the player runs into space1
        hitSomething = CheckCollisionBoxSphere(space1_bBox, mainChar_center, mainChar_radius);
        if (hitSomething){
            DrawText("YOU HIT SOMETHING", 500, 50, 30, BLACK);
        }



        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's recommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode); // Update camera

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        bool xiong_enabled = true;
        bool eggert_enabled = true;
        bool voss_enabled = true; // voss
        const bool kerney_enabled = true;
        bool raymond_enabled = false;
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // TODO:
        // Raymond will implement collision
        // Voss will implement a puzzle
        // Meg will git lfs migrate
        // Bruce will do a skybox
        // git lfs migrate
        // Add in a skybox
        // Add in models for the spaceship
        // Add in monsters
        // Raymond can add jumping and sprinting and collision
        // Add in lighting/electricity puzzle
        // Weapons
        BeginMode3D(camera);

        xiong(xiong_enabled);
        eggert(eggert_enabled);
        voss(voss_enabled);
        kerney(kerney_enabled);
        raymond(raymond_enabled);
        
        
        // target point
        //DrawSphere(orbitCam.CameraPosition, 0.25f, RED);


        // Draw "character" sphere at the origin
        DrawSphere(mainChar_center, 0.25, MAROON);
        DrawSphereWires(mainChar_center, mainChar_radius, 10, 20, BLACK);   

        // Draw Space Bits
        DrawModel(space1, space1_pos, space1_scale, WHITE);
        DrawBoundingBox(space1_bBox, PINK);
        DrawModel(space2, space2_pos, space2_scale, WHITE);
        DrawBoundingBox(space2_bBox, DARKGREEN);

        // Draw Barbarian
        // DrawModel(barbarian, barbarian_pos, 1.0f, WHITE);

        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, GOLD); // Draw ground
       

        // Draw some cubes around
        /*
        for (int i = 0; i < MAX_COLUMNS; i++)
        {
            DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
            DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
        }
        */
        
            
        EndMode3D();
        //rlTPCameraEndMode3D();

        // Draw info boxes (HUD)
        DrawRectangle(5, 5, 330, 100, Fade(BLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLACK);
        DrawText("Press 'K' to Laser", 15, 30, 10, WHITE);

        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();

    // Unload Sounds - Meg
    UnloadSound(laser);
    UnloadMusicStream(bgMusic);
    
    // Unload All Models
    for (size_t i = 0; i < models.size(); i++)
    {
        UnloadModel(models.at(i));
    }
    
    // Unload All Textures
    for (size_t i = 0; i < textures.size(); i++)
    {
        UnloadTexture(textures.at(i));
    }
    //--------------------------------------------------------------------------------------
}

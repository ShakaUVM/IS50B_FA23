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
#include <unordered_map>

// For the skybox - Bruce Xiong
#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

// Generate cubemap (6 faces) from equirectangular (panorama) texture - Bruce Xiong
static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);

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

    // These vectors are for storing the shape "models" and the data needed to draw them.
    vector<Cube> cubes;
    vector<Sphere> spheres;
    vector<Plane> planes;
    // These cubes can be picked up, they will be used for testing. Eventually will be subbed out for models or something
    vector<Cube> pickups;
    vector<Pickup> pickupTimers;
    
    
    // MEAGAN - TESTING TO SEE IF THIS HELPS WITH SPECIFIC COLLISION
    vector<Vector3> cubeLocations;
    vector<Vector3> sphereLocations;
    vector<Vector3> planeLocations;
    bool hitACube = false;
    bool hitASphere  = false;
    bool hitAPlane = false;
    bool allCubesActivated = false;
    BoundingBox hitBoundingBox;
    Vector3 hitLocation = {99, 99, 99}; // Set to something ridiculous for debugging



    int pickupsCollected = 0;

    // Temporary code for testing a pickup system. I'm not sure how we are going to actually implement the pickups, if we will have an inventory or just use a state machine or what
    //-  This is mostly just for testing the collision and destruction of the object.
    Cube pickupTest;
    pickupTest.position = (Vector3){-2, 1, -3};
    pickupTest.size = (Vector3){1, 1, 1};
    pickupTest.color = GREEN;
    pickups.push_back(pickupTest);
    // This vector contains all of the bounding boxes for all objects in the level, if you create an object post first frame, you must create these yourself. Be sure not to do it in a loop, because they persist.
    vector<BoundingBox> boxes;


    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");
    int MONITOR = GetMonitorCount() == 3 ? 2 : 0;

    SetWindowMonitor(MONITOR);
    SetWindowSize(GetMonitorWidth(MONITOR), GetMonitorHeight(MONITOR));
    

    // BEGIN MAIN CHARACTER SET UP - MEG
    float mainChar_radius = 0.25;
    Vector3 mainChar_orig = (Vector3){0.0, mainChar_radius, 0.0};
    Vector3 mainChar_center = mainChar_orig;
    float mainChar_speed = 0.25;
    bool hitSomething = false;
    bool hitModel = false;

    // Faux State Machine
    bool onFloor = true;
    bool onPlatform = false;
    bool isJumping = false;
    bool isFalling = false;
    // END MAIN CHARACTER SET UP

    // BEGIN PLATFORM SET UP - MEG
    // All Platforms
    bool hitPlatform = false;
    float platformHeight = 0.5f;
    float platformThickness = 0.25f;
    float platformWidth = 2.0f;
    Vector3 platformOrigin = (Vector3){-3.0f, 0.5f, -8.0f};
    BoundingBox platform_bBox = (BoundingBox){(Vector3){platformOrigin.x - platformWidth / 2.0f, platformOrigin.y - platformThickness / 2.0f, platformOrigin.z - platformWidth / 2.0f}, (Vector3){platformOrigin.x + platformWidth / 2.0f, platformOrigin.y + platformThickness / 2.0f, platformOrigin.z + platformWidth / 2.0f}};
    // END PLATFORM SET UP

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

    // Vector to store all bounding boxes
    vector<BoundingBox> modelBoxes;

    // Voss SolidObject test (not ready - need to add more to SolidObject class)
    // unordered_map<string, SolidObject> solidObjects;
    // solidObjects["space1"] = SolidObject("space1", (Vector3){0.0f, 0.1f, -4.0f}, MODEL_OBJ_PATH + "basemodule_A.obj", MODEL_TEXTURE_PATH + "spacebits_texture.png", 0.5f, WHITE, true, true);
    // solidObjects["space2"] = SolidObject("space2", (Vector3){4.5f, 0.1f, -6.0f}, MODEL_OBJ_PATH + "basemodule_C.obj", MODEL_TEXTURE_PATH + "spacebits_texture.png", 0.75f, WHITE, true, true);
    Enemy testEnemy = Enemy("testEnemy",
                            (Vector3){0.0f, 0.1f, -4.0f},
                            MODEL_OBJ_PATH + "basemodule_A.obj",
                            MODEL_TEXTURE_PATH + "spacebits_texture.png",
                            (Vector3){0.0f, 0.0f, 0.0f},
                            (Vector3){-0.005f, 0.0f, 0.005f},
                            100,
                            10,
                            0.5f,
                            true,
                            true);

    // SPACE BITS - MEAGAN
    // Space Model 1 (space1)
    float space1_scale = 0.5;
    Model space1 = models.at(0);           // Voss
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
    modelBoxes.push_back(space1_bBox);

    // Space Model 2 (space2)
    float space2_scale = 0.75;
    Model space2 = models.at(1);           // Voss
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
    modelBoxes.push_back(space2_bBox);

    // Barbarian (For Testing)
    // Model barbarian = LoadModel ("adventurers\\Characters\\gltf\\Barbarian.glb");
    Model barbarian = models.at(2); // Voss
    Texture2D barbarian_tex = LoadTexture("adventurers\\Characters\\gltf\\barbarian_texture.png");
    // Texture2D barbarian_tex = textures.at(1); // Voss
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
    camera.position = (Vector3){0.0f, 2.0f, 4.0f};                                                                  // Camera position
    camera.position = (Vector3){mainChar_center.x, mainChar_center.y + mainChar_radius * 2, mainChar_center.z + 2}; // Camera position
    camera.target = (Vector3){mainChar_center.x, mainChar_center.y, mainChar_center.z - 2};                         // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};                                                                        // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                                                                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                                                                         // Camera projection type

    int cameraMode = CAMERA_THIRD_PERSON;

    // BEGIN MOUSE DETECTION
    Ray ray = {0};
    RayCollision clickedSomething = {0};
    bool interactable = false;
    // END MOUSE DETECTION

    
    // Begin Skybox; Load skybox model - Bruce Xiong
    // Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    // Model skybox = LoadModelFromMesh(cube);
    // bool useHDR = true;

    // // Load skybox shader and set required locations
    // // NOTE: Some locations are automatically set at shader loading
    // skybox.materials[0].shader = LoadShader(TextFormat("resources/shaders/glsl%i/skybox.vs", GLSL_VERSION),
    //                                         TextFormat("resources/shaders/glsl%i/skybox.fs", GLSL_VERSION));

    // int bob = MATERIAL_MAP_CUBEMAP;
    // int HDR_boi = useHDR ? 1 : 0;
    // SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), &bob, SHADER_UNIFORM_INT);
    // SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), &HDR_boi, SHADER_UNIFORM_INT);
    // SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), &HDR_boi, SHADER_UNIFORM_INT);

    // // Load cubemap shader and setup required shader locations
    // Shader shdrCubemap = LoadShader(TextFormat("resources/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
    //                                 TextFormat("resources/shaders/glsl%i/cubemap.fs", GLSL_VERSION));

    // int zerp = 0;
    // SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), &zerp, SHADER_UNIFORM_INT);

    // char skyboxFileName[256] = { 0 };

    // Texture2D panorama;
    // if (useHDR)
    // {
    //     //TextCopy(skyboxFileName, "resources/dresden_square_2k.hdr"); //the original sample code
    //     TextCopy(skyboxFileName, "resources/space_skybox.hdr");

    //     // Load HDR panorama (sphere) texture
    //     panorama = LoadTexture(skyboxFileName);

    //     // Generate cubemap (texture with 6 quads-cube-mapping) from panorama HDR texture
    //     // NOTE 1: New texture is generated rendering to texture, shader calculates the sphere->cube coordinates mapping
    //     // NOTE 2: It seems on some Android devices WebGL, fbo does not properly support a FLOAT-based attachment,
    //     // despite texture can be successfully created.. so using PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 instead of PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
    //     skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = GenTextureCubemap(shdrCubemap, panorama, 1024, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    //     //UnloadTexture(panorama);    // Texture not required anymore, cubemap already generated
    // }
    // else
    // {
    //     Image img = LoadImage("resources/skybox.png");
    //     skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT); // CUBEMAP_LAYOUT_PANORAMA
    //     UnloadImage(img);
    // }
    //End of Skybox - Bruce Xiong
    
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

    DisableCursor();  // Limit cursor to relative movement inside the window
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Only create the bounding boxes once on the start of the game, if you add more boxes after the first frame you must add the bounding boxes yourself.
    bool boxesCreated = false;

    BoundingBox cameraBB = {(Vector3){camera.position.x - .5f, camera.position.y - .5f, camera.position.z - .5f}, (Vector3){camera.position.x + .5f, camera.position.y + .5f, camera.position.z + .5f}};
    //boxes.push_back(cameraBB);


    bool raymond_enabled = true;
    raymond(raymond_enabled, cubes, spheres, planes);


// BEGIN MAIN GAME LOOP 
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        hitSomething = false;
        hitModel = false;

        if (IsCursorHidden())
            UpdateCamera(&camera, cameraMode);

        // Toggle camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (IsCursorHidden())
                EnableCursor();
            else
                DisableCursor();
        }

        //xiong_skybox(skybox, useHDR, shdrCubemap, skyboxFileName); //Skybox code - Bruce Xiong

        UpdateMusicStream(bgMusic);
        // rlTPCameraBeginMode3D(&orbitCam);

        // Press 'k' to play a laser sound
        if (IsKeyPressed(KEY_K))
        {
            PlaySound(laser);
        }

        // WASD Control for the Main Character
        if (IsKeyDown(KEY_W))
        {
            mainChar_center.z -= mainChar_speed;
            for (size_t i = 0; i < modelBoxes.size(); i++)
            {
                bool hitTemp = false;
                hitTemp = CheckCollisionBoxSphere(modelBoxes.at(i), mainChar_center, mainChar_radius);
                if (hitTemp)
                {
                    hitSomething = true;
                }
            }
            if (hitSomething)
            {
                mainChar_center.z += mainChar_speed;
            }
        }
        if (IsKeyDown(KEY_A))
        {
            mainChar_center.x -= mainChar_speed;
            for (size_t i = 0; i < modelBoxes.size(); i++)
            {
                bool hitTemp = false;
                hitTemp = CheckCollisionBoxSphere(modelBoxes.at(i), mainChar_center, mainChar_radius);
                if (hitTemp)
                {
                    hitSomething = true;
                }
            }
            if (hitSomething)
            {
                mainChar_center.x += mainChar_speed;
            }
        }
        if (IsKeyDown(KEY_S))
        {
            mainChar_center.z += mainChar_speed;
            for (size_t i = 0; i < modelBoxes.size(); i++)
            {
                bool hitTemp = false;
                hitTemp = CheckCollisionBoxSphere(modelBoxes.at(i), mainChar_center, mainChar_radius);
                if (hitTemp)
                {
                    hitSomething = true;
                }
            }
            if (hitSomething)
            {
                mainChar_center.z -= mainChar_speed;
            }
        }
        if (IsKeyDown(KEY_D))
        {
            mainChar_center.x += mainChar_speed;
            for (size_t i = 0; i < modelBoxes.size(); i++)
            {
                bool hitTemp = false;
                hitTemp = CheckCollisionBoxSphere(modelBoxes.at(i), mainChar_center, mainChar_radius);
                if (hitTemp)
                {
                    hitSomething = true;
                }
            }
            if (hitSomething)
            {
                mainChar_center.x -= mainChar_speed;
            }
        }
        // Jumping for Main Character
        // Very Basic
        // Only works for the ground level and actually lets you stay in flight if you hold SPACE
        // if (IsKeyPressed(KEY_SPACE))
        // {
        //     mainChar_center.y += 1.5;
        // }
        // if (IsKeyReleased(KEY_SPACE))
        // {
        //     while (mainChar_center.y > mainChar_radius)
        //     {
        //         mainChar_center.y -= 0.10;
        //         hitPlatform = CheckCollisionBoxSphere(platform_bBox, mainChar_center, mainChar_radius);
        //         if (hitPlatform)
        //         {
        //             mainChar_center.y = (platformOrigin.y + platformThickness / 2.0f + mainChar_radius);
        //             break;
        //         }
        //     }
        // }

        // BEGIN COLLISION TESTING
        if (boxesCreated){
        for (size_t i = 0; i < boxes.size(); i++)
            {

                bool hitTemp = CheckCollisionBoxSphere(boxes.at(i), mainChar_center, mainChar_radius);
                if (hitTemp)
                {
                    hitSomething = true;
                }
            }
        }

        // SHOOT A RAY FROM THE MOUSE
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!clickedSomething.hit)
            {
                ray = GetMouseRay(GetMousePosition(), camera);

                // Check collision between ray and box
                for (size_t i = 0; i < boxes.size(); i++)
                {

                    RayCollision clickTemp = GetRayCollisionBox(ray, boxes.at(i));
                    if (clickTemp.hit)
                    {
                        if (i < cubes.size()){
                            hitLocation = cubes.at(i).position;
                            hitACube = true;
                        }
                        else if (i < (cubes.size() + spheres.size())){
                            hitLocation = spheres.at(i - cubes.size()).position;
                            hitASphere = true;
                        }
                        else if (i < (cubes.size() + spheres.size() + planes.size())){
                            hitLocation = planes.at(i - (cubes.size() + spheres.size())).position;
                            hitAPlane = true;
                        }
                        
                        hitBoundingBox = boxes.at(i);
                        clickedSomething = clickTemp;
                    }
                }
            }
            else
                clickedSomething.hit = false;
        }

        if (hitSomething)
        {
            DrawText("YOU HIT SOMETHING", 500, 50, 30, BLACK);
            DrawText(TextFormat("x-Hit: %i", hitLocation.x), 500, 50, 30, BLACK);
        }

        if (clickedSomething.hit)
        {
            DrawText("YOU CLICKED SOMETHING", 500, 50, 30, BLACK);
            DrawText(TextFormat("x-Hit: %.2f", hitLocation.x), 600, 70, 30, BLACK);
            DrawText(TextFormat("y-Hit: %.2f", hitLocation.y), 700, 90, 30, BLACK);
            DrawText(TextFormat("z-Hit: %.2f", hitLocation.z), 800, 110, 30, BLACK);
        }
        // END COLLISION TESTING

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's recommended to compute camera movement manually
        // Delta movement
        // Vector3 proposedMove = (Vector3){0,0,0};
        // I'll figure this out later
        // Vector3 rotation = (Vector3){0,0,0};

        // const int MOVESPEED = 1;
        //  Update camera computes movement internally depending on the camera mode
        //  Some default standard keyboard/mouse inputs are hardcoded to simplify use
        //  For advance camera controls, it's reecommfended to compute camera movement manually
        // if(boxesCreated) CheckCollisionsSean(&camera/*, proposedMove*/, boxes);
        //
        // UpdateCamera(&camera, cameraMode); // Update camera
        //boxes.at(0).min = (Vector3){camera.position.x - 1, camera.position.y - 1, camera.position.z - 1};
        //boxes.at(0).max = (Vector3){camera.position.x + 1, camera.position.y + 1, camera.position.z + 1};
        cameraBB.min = (Vector3){camera.position.x - 1, camera.position.y - 1, camera.position.z - 1};
        cameraBB.max = (Vector3){camera.position.x + 1, camera.position.y + 1, camera.position.z + 1};

        // if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { proposedMove.y += MOVESPEED;}
        // if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { proposedMove.x -= MOVESPEED;}
        // if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { proposedMove.y -= MOVESPEED;}
        // if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {proposedMove.x += MOVESPEED;}

        // ignore the 1, zoom is disabled
        // UpdateCameraPro(&camera, proposedMove, rotation, 1);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        bool xiong_enabled = true;
        bool eggert_enabled = true;
        bool voss_enabled = true; // voss
        const bool kerney_enabled = true;
        
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

        // For the skybox; We are inside the cube, we need to disable backface culling! - Bruce Xiong
        // rlDisableBackfaceCulling();
        // rlDisableDepthMask();
        // DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
        // rlEnableBackfaceCulling();
        // rlEnableDepthMask();
        //DrawGrid(10, 1.0f);
        // End of skybox - Bruce Xiong

        xiong(xiong_enabled);
        eggert(eggert_enabled);
        voss(voss_enabled);
        kerney(kerney_enabled);
        

        // This will create bounding boxes for every object added to the cubes, spheres, and planes vectors in the first frame. everything after the first frame, you gotta create your own bounding boxes and index them properly.
        if (!boxesCreated)
        {
            boxesCreated = true;
            int count = boxes.size();
            for (size_t i = 0; i < cubes.size(); i++)
            {
                cubeLocations.push_back(cubes.at(i).position); // MEG
                cubes.at(i).boundingBoxIndex = count;
                BoundingBox temp = {(Vector3){cubes.at(i).position.x - cubes.at(i).size.x / 2, cubes.at(i).position.y - cubes.at(i).size.y / 2, cubes.at(i).position.z - cubes.at(i).size.z / 2}, (Vector3){cubes.at(i).position.x + cubes.at(i).size.x / 2, cubes.at(i).position.y + cubes.at(i).size.y / 2, cubes.at(i).position.z + cubes.at(i).size.z / 2}};
                boxes.push_back(temp);
                count++;
            }
            cout << boxes.size() << endl;
            for (size_t i = 0; i < spheres.size(); i++)
            {
                sphereLocations.push_back(spheres.at(i).position); // MEG
                spheres.at(i).boundingBoxIndex = count;
                BoundingBox temp = {(Vector3){spheres.at(i).position.x - spheres.at(i).radius / 2, spheres.at(i).position.y - spheres.at(i).radius / 2, spheres.at(i).position.z - spheres.at(i).radius / 2}, (Vector3){spheres.at(i).position.x + spheres.at(i).radius / 2, spheres.at(i).position.y + spheres.at(i).radius / 2, spheres.at(i).position.z + spheres.at(i).radius / 2}};
                boxes.push_back(temp);
                count++;
            }
            for (size_t i = 0; i < planes.size(); i++)
            {
                planeLocations.push_back(planes.at(i).position); // MEG
                planes.at(i).boundingBoxIndex = count;
                BoundingBox temp = {(Vector3){planes.at(i).position.x - planes.at(i).size.x / 2, planes.at(i).position.y - 1, planes.at(i).position.z - planes.at(i).size.y / 2}, (Vector3){planes.at(i).position.x + planes.at(i).size.x / 2, planes.at(i).position.y, planes.at(i).position.z + planes.at(i).size.y / 2}};
                boxes.push_back(temp);
                count++;
            }

            for (size_t i = 0; i < pickups.size(); i++)
            {
                pickups.at(i).boundingBoxIndex = count;
                BoundingBox temp = {(Vector3){pickups.at(i).position.x - pickups.at(i).size.x / 2, pickups.at(i).position.y - pickups.at(i).size.y / 2, pickups.at(i).position.z - pickups.at(i).size.z / 2}, (Vector3){pickups.at(i).position.x + pickups.at(i).size.x / 2, pickups.at(i).position.y + pickups.at(i).size.y / 2, pickups.at(i).position.z + pickups.at(i).size.z / 2}};
                boxes.push_back(temp);
                count++;
            }
        }




        for (Cube &c : cubes)
        {
            if (hitACube){
                if (c.position.x == hitLocation.x && c.position.y == hitLocation.y && c.position.z == hitLocation.z && c.activated == false){
                    c.activated = true;
                    
                }
            }
            if (c.activated == true){
                c.color = MAROON;
                c.Draw();
            }
            else {
                c.Draw();
            }

        }
        
        allCubesActivated = true;
        for (Cube &c : cubes) {
            
            if (c.activated == false) {
                allCubesActivated = false;
            }
        }

        for (Sphere &s : spheres)
        {
            // THIS CHECKS TO SEE IF YOU CLICKED A SPHERE, BUT WE'RE NOT USING THIS RIGHT NOW
            // if (hitASphere){
            //     if (s.position.x == hitLocation.x && s.position.y == hitLocation.y && s.position.z == hitLocation.z){
            //         s.color = DARKGREEN;
            //         s.Draw();
            //     }
            //     else {
            //         s.Draw();
            //     }
            // }
            // else {
            //     s.Draw();
            // }
            if (allCubesActivated){
                s.color = GOLD;
                s.Draw();
            }
            else {
                s.Draw();
            }
        }

        for (Plane &p : planes)
        {
            // THIS CHECKS TO SEE IF YOU CLICKED A PLANE, BUT WE'RE NOT USING THIS RIGHT NOW
            // if (hitAPlane){
            //     if (p.position.x == hitLocation.x && p.position.y == hitLocation.y && p.position.z == hitLocation.z){
            //         p.color = BEIGE;
            //         p.Draw();
            //     }
            //     else {
            //         p.Draw();
            //     }
            // }
            // else {
            //         p.Draw();
            // }

            if (allCubesActivated){
                p.color = BEIGE;
                p.Draw();
            }
            else {
                p.Draw();
            }
        }

        for (Cube &c : pickups)
        {
            BoundingBox &pickupBB = boxes.at(c.boundingBoxIndex);
            // boxes.at(0) is the camera bounding box
            bool hit;
            hit = CheckCollisionBoxes(boxes.at(0), pickupBB);
            // Camera is touching a box
            if (hit)
            {
                pickupsCollected++;
                // Instead of removing the pickup, just move it out of view.
                //-     I don't know if creating new objects is expensive or not, so I prefer pooling.
                Pickup p;
                p.pickupIndex = c.boundingBoxIndex - pickups.at(0).boundingBoxIndex;
                p.collected = true;
                p.defaultLocation = c.position;
                p.timeCollected = time(NULL);
                p.respawnDelay = 1;
                p.timerIndex = pickupTimers.size();
                pickupTimers.push_back(p);
                c.position = (Vector3){0, -90000, 0};
                pickupBB.min = (Vector3){c.position.x - c.size.x / 2, c.position.y - c.size.y / 2, c.position.z - c.size.z / 2};
                pickupBB.max = (Vector3){c.position.x + c.size.x / 2, c.position.y + c.size.y / 2, c.position.z + c.size.z / 2};
            }
            c.Draw();
        }



        for (Pickup p : pickupTimers)
        {
            if (p.canRespawn())
            {
                cout << "HERE" << endl;
                pickups.at(p.pickupIndex).position = p.defaultLocation;
                BoundingBox &bb = boxes.at(pickups.at(p.pickupIndex).boundingBoxIndex);
                Cube c = pickups.at(p.pickupIndex);
                bb.min = (Vector3){c.position.x - c.size.x / 2, c.position.y - c.size.y / 2, c.position.z - c.size.z / 2};
                bb.max = (Vector3){c.position.x + c.size.x / 2, c.position.y + c.size.y / 2, c.position.z + c.size.z / 2};
                pickupTimers.erase(pickupTimers.begin() + p.timerIndex);
            }
        }

        // just for debugging, can be commented out
        for (BoundingBox &bb : boxes)
        {
            DrawBoundingBox(bb, GOLD);
        }

        // target point
        // DrawSphere(orbitCam.CameraPosition, 0.25f, RED);


        // Draw "character" sphere at the origin
        DrawSphere(mainChar_center, 0.25, MAROON);
        DrawSphereWires(mainChar_center, mainChar_radius, 10, 20, WHITE);

        // Draw Space Bits
        DrawModel(space1, space1_pos, space1_scale, WHITE);
        DrawBoundingBox(space1_bBox, WHITE);
        DrawModel(space2, space2_pos, space2_scale, WHITE);
        DrawBoundingBox(space2_bBox, WHITE);

        // Draw Enemies
        testEnemy.draw();

        // Draw Barbarian
        // DrawModel(barbarian, barbarian_pos, 1.0f, WHITE);

        // Draw Ground
        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, BEIGE);

        // Draw Platforms
        if (clickedSomething.hit)
        {
            DrawCube(platformOrigin, platformWidth, platformThickness, platformWidth, PINK);
            DrawCubeWires(platformOrigin, platformWidth, platformThickness, platformWidth, WHITE);
            DrawBoundingBox(platform_bBox, WHITE);
        }
        else
        {
            DrawCube(platformOrigin, platformWidth, platformThickness, platformWidth, DARKGREEN);
            DrawCubeWires(platformOrigin, platformWidth, platformThickness, platformWidth, WHITE);
            DrawBoundingBox(platform_bBox, WHITE);
        }

        EndMode3D();
        // rlTPCameraEndMode3D();
        
        //Skybox code - Bruce Xiong
        // DrawTextureEx(panorama, (Vector2){ 0, 0 }, 0.0f, 0.5f, WHITE);
        // if (useHDR) DrawText(TextFormat("Panorama image from hdrihaven.com: %s", GetFileName(skyboxFileName)), 10, GetScreenHeight() - 20, 10, BLACK);
        // else DrawText(TextFormat(": %s", GetFileName(skyboxFileName)), 10, GetScreenHeight() - 20, 10, BLACK);
        // DrawFPS(10, 10);
        //Skybox code - Bruce Xiong
        
        
        // Draw info boxes (HUD)
        DrawRectangle(5, 5, 330, 100, BLACK);
        DrawRectangleLines(5, 5, 330, 100, WHITE);
        DrawText("Press 'K' to Laser", 15, 30, 15, WHITE);


        // Text for Activating a Room
        int screenWidth = GetMonitorWidth(MONITOR);
        int screenHeight = GetMonitorHeight(MONITOR);
        int boxWidth = 550;
        int boxHeight = 100;
        
        
        if (allCubesActivated) {
            // DrawText("Activated", 15, 40, 10, WHITE); // Debugging
            DrawRectangle((screenWidth/2.0 - boxWidth/2.0), screenHeight - 200, boxWidth, boxHeight, BLACK);
            DrawRectangleLines((screenWidth/2.0 - boxWidth/2.0), screenHeight - 200, boxWidth, boxHeight, WHITE);
            DrawText("You Activated This Room!", (screenWidth/2.0 - boxWidth/2.0) + 5, screenHeight - 200 + 5, 25, WHITE);
        }


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // UnloadShader(skybox.materials[0].shader); //Skybox code - Bruce Xiong
    // UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    // UnloadModel(skybox); // Unload skybox model
    

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
    //-----------------------------------s---------------------------------------------------
}
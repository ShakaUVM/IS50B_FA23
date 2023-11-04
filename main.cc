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

#define MAX_COLUMNS 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 3840;
    const int screenHeight = 2160;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");
    SetWindowMonitor(GetMonitorCount() == 3 ? 2 : 0);
    //SetWindowSize(GetMonitorWidth(2),GetMonitorHeight(2));
        
    // Load Models - Meg
    
    // Space Bits
    Model space1 = LoadModel("spaceBits\\Assets\\obj\\basemodule_A.obj");
    Texture2D space1_tex = LoadTexture("spaceBits\\Assets\\textures\\spacebits_texture.png");
    Vector3 space1_pos = {0.0f, 0.1f, -4.0f};
    space1.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = space1_tex;
    BoundingBox space1_bBox = GetMeshBoundingBox(space1.meshes[0]);
    
    // Barbarian (For Testing)
    Model barbarian = LoadModel ("adventurers\\Characters\\gltf\\Barbarian.glb");
    Texture2D barbarian_tex = LoadTexture("adventurers\\Characters\\gltf\\barbarian_texture.png");
    Vector3 barbarian_pos = {0.0, 0.1, -10.0};
  
    
    // Initialize audio device, so you can play sounds - Meg
    InitAudioDevice();
    // This is a cool laser sound
    Sound laser = LoadSound("laser.wav");
    Music bgMusic = LoadMusicStream("Space Music2.mp3");
    PlayMusicStream(bgMusic); // Play Background Music
    

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f}; // Camera position
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};   // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};       // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                           // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;        // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    // Generates some random columns
    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};

    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){(float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15)};
        colors[i] = (Color){GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255};
    }

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateMusicStream(bgMusic);

        // Press 'k' to play a laser sound
        if (IsKeyPressed(KEY_K))
        {
            PlaySound(laser);
        }
        /*
        // Update
        //----------------------------------------------------------------------------------
        // Switch camera mode
        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        // Switch camera projection
        if (IsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                // Create isometric view
                cameraMode = CAMERA_THIRD_PERSON;
                // Note: The target distance is related to the render distance in the orthographic projection
                camera.position = (Vector3){ 0.0f, 2.0f, -100.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_ORTHOGRAPHIC;
                camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
                CameraYaw(&camera, -135 * DEG2RAD, true);
                CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
            }
            else if (camera.projection == CAMERA_ORTHOGRAPHIC)
            {
                // Reset to default view
                cameraMode = CAMERA_THIRD_PERSON;
                camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }
        */

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's reecommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode); // Update camera

        /*
                // Camera PRO usage example (EXPERIMENTAL)
                // This new camera function allows custom movement/rotation values to be directly provided
                // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
                UpdateCameraPro(&camera,
                    (Vector3){
                        (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -      // Move forward-backward
                        (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
                        (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -   // Move right-left
                        (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                        0.0f                                                // Move up-down
                    },
                    (Vector3){
                        GetMouseDelta().x*0.05f,                            // Rotation: yaw
                        GetMouseDelta().y*0.05f,                            // Rotation: pitch
                        0.0f                                                // Rotation: roll
                    },
                    GetMouseWheelMove()*2.0f);                              // Move to target (zoom)
        */
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
        
        // Draw Space Bits
        DrawModel(space1, space1_pos, 1.0f, WHITE);

                
        // Draw Barbarian
        DrawModel(barbarian, barbarian_pos, 1.0f, WHITE);
        
        
        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, GOLD); // Draw ground
        /*
        DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
   `     DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall
        */


        // Draw some cubes around
        /*
        for (int i = 0; i < MAX_COLUMNS; i++)
        {
            DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
            DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
        }
        */
        
        
        /*
        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON)
        {
            DrawCube(camera.target, 0.5f, 0.5f, 0.5f, PURPLE);
            DrawCubeWires(camera.target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
        }
        */

        EndMode3D();

        // Draw info boxes
        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);
        
        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
        DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
        DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

        /*
                    DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
                    DrawRectangleLines(600, 5, 195, 100, BLUE);

                    DrawText("Camera status:", 610, 15, 10, BLACK);
                    DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
                                                      (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                                      (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                                      (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
                    DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                                            (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
                    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
                    DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
                    DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
        */
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    CloseAudioDevice();
    
    //Unload Sounds - Meg
    UnloadSound(laser);
    UnloadMusicStream(bgMusic);
    // Unload Models/Textures - Meg
    UnloadModel(space1);
    UnloadTexture(space1_tex);
    UnloadModel(barbarian);
    UnloadTexture(barbarian_tex);
    //--------------------------------------------------------------------------------------
}

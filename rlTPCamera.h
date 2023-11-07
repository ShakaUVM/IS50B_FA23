/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   TPOrbitCamera * Third Person Camera Example
*
*   LICENSE: MIT
*
*   Copyright (c) 2021 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/


#ifndef TP_CAMERA_H
#define TP_CAMERA_H

#include "raylib.h"
#include "raymath.h"

typedef enum
{
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_UP,
    TURN_DOWN,
    SPRINT,
    LAST_CONTROL
}rlTPCameraControls;

typedef struct
{
    // keys used to control the camera
    int ControlsKeys[LAST_CONTROL];

    // the speed in units/second to move 
    // X = sidestep
    // Y = jump/fall
    // Z = forward
    Vector3 MoveSpeed;

    // the speed for turning when using keys to look
    // degrees/second
    Vector2 TurnSpeed;

    // use the mouse for looking?
    bool UseMouse;
    int UseMouseButton;

    // how many pixels equate out to an angle move, larger numbers mean slower, more accurate mouse
    float MouseSensitivity;

    // how far down can the camera look
    float MinimumViewY;

    // how far up can the camera look
    float MaximumViewY;

    // the position of the base of the camera (on the floor)
    // note that this will not be the view position because it is offset by the eye height.
    // this value is also not changed by the view bobble
    Vector3 CameraPosition;

    // how far from the target position to the camera's view point (the zoom)
    float CameraPullbackDistance;

    // the Raylib camera to pass to raylib view functions.
    Camera ViewCamera;

    // the vector in the ground plane that the camera is facing
    Vector3 ViewForward;

    // the field of view in X and Y
    Vector2 FOV;

    // state for view angles
    Vector2 ViewAngles;

    // state for window focus
    bool Focused;

    //clipping planes
    // note must use BeginModeFP3D and EndModeFP3D instead of BeginMode3D/EndMode3D for clipping planes to work
    double NearPlane;
    double FarPlane;
}rlTPCamera;

// called to initialize a camera to default values
RLAPI void rlTPCameraInit(rlTPCamera* camera, float fovY, Vector3 position);

// turn the use of mouselook on/off, also updates the cursor visibility and what button to use, set button to -1 to disable mouse
RLAPI void rlTPCameraUseMouse(rlTPCamera* camera, bool useMouse, int button);

// Get the camera's position in world (or game) space
RLAPI Vector3 rlTPCameraGetPosition(rlTPCamera* camera);

// Set the camera's position in world (or game) space
RLAPI void rlTPCameraSetPosition(rlTPCamera* camera, Vector3 position);

// returns the ray from the camera through the center of the view
RLAPI Ray rlTPCameraGetViewRay(rlTPCamera* camera);

// update the camera for the current frame
RLAPI void rlTPCameraUpdate(rlTPCamera* camera);

// start drawing using the camera, with near/far plane support
RLAPI void rlTPCameraBeginMode3D(rlTPCamera* camera);

// end drawing with the camera
RLAPI void rlTPCameraEndMode3D();

#endif //TP_CAMERA_H
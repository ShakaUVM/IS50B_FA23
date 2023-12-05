#include "main.h"
#include <vector>

struct Plane
{
    Vector3 position = {1, 1, 1};
    Vector2 size = {1, 1};
    Color color = GREEN;
    int boundingBoxIndex = -1;
    int indexId = -999; // TEST IDEA FOR SPECIFIC COLLISION - MEG

    void Draw()
    {
        DrawPlane(this->position, this->size, this->color);
    }
};

struct Sphere
{
    Vector3 position = {1, 1, 1};
    // this is actually the diameter but raylib calls it a radius and I hate it.
    float radius = 1;
    Color color = RED;
    int boundingBoxIndex = -1;
    int indexId = -999; // TEST IDEA FOR SPECIFIC COLLISION - MEG

    void Draw()
    {
        DrawSphere(this->position, this->radius, this->color);
    }
};

struct Cube
{
    Vector3 position = {1, 1, 1};
    Vector3 size = {1, 1, 1};
    Color color = BLUE;
    int boundingBoxIndex = -1;
    int indexId = -999; // TEST IDEA FOR SPECIFIC COLLISION - MEG

    void Draw()
    {
        DrawCube(this->position, this->size.x, this->size.y, this->size.z, this->color);
    }
};

struct Pickup {
    int timerIndex = -1;
    //Keep track of whether or not the item has been grabbed
    bool collected = false;
    //Easy for error checking, if you get a negative index, uninitialized pickup
    int pickupIndex = -1;
    //If the time is 0, the object has not been collected yet. OR its january 1, 1970. Either or.
    long long timeCollected = 0;
    //Default of 10 seconds, but modifiable
    long long respawnDelay = 10;
    //The original location of the pickup, since the pickups are pooled currently. I don't know what the pickups will be, maybe they will need to respawn.
    //-     instead of creating new ones, I'd rather pool the existing ones and just move them offscreen. 
    Vector3 defaultLocation;

    bool operator==(Pickup& rhs) {
        return rhs.pickupIndex == pickupIndex;
    }

    bool canRespawn() {
        if(collected) {
            return (timeCollected + respawnDelay) < time(NULL);
        }
        return false;        
    }

};

void CheckCollisionsSean(Camera*camera/*, Vector3& movement*/, vector<BoundingBox> boundingBoxes) {
    
    /* Vector3 newLocation = camera->position;
    newLocation.x += movement.x;
    newLocation.y += movement.y;
    newLocation.z += movement.z; */
    
    
    
    for(size_t i = 1; i < boundingBoxes.size(); i++) {
        BoundingBox& bb = boundingBoxes.at(i);
        if(CheckCollisionBoxes(boundingBoxes.at(0), bb)) cout << "Touch" << endl;
    }
    
}
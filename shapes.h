#include "main.h"
#include <vector>

struct Plane {
        Vector3 position = {1,1,1};
        Vector2 size = {1,1};
        Color color = GREEN;
        int boundingBoxIndex = -1;
        
        void Draw() {
            DrawPlane(this->position, this->size, this->color);
        }
};

struct Sphere {
        Vector3 position = {1,1,1};
        //this is actually the diameter but raylib calls it a radius and I hate it.
        float radius = 1;
        Color color = RED;
        int boundingBoxIndex = -1;

        void Draw() {
            DrawSphere(this->position, this->radius, this->color);
        }
};

struct Cube {
        Vector3 position = {1,1,1};
        Vector3 size = {1,1,1};
        Color color = BLUE;
        int boundingBoxIndex = -1;

        
        void Draw() {
            DrawCube(this->position, this->size.x, this->size.y, this->size.z, this->color);
        }
};


void CheckCollisionsSean(Camera*camera, Vector3& movement, vector<BoundingBox> boundingBoxes) {
    
    Vector3 newLocation = camera->position;
    newLocation.x += movement.x;
    newLocation.y += movement.y;
    newLocation.z += movement.z;
    
    
    
    for(int i = 1; i < boundingBoxes.size(); i++) {
        BoundingBox& bb = boundingBoxes.at(i);
        if(CheckCollisionBoxes(boundingBoxes.at(0), bb)) cout << "Touch" << endl;
    }
    
}
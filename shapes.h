#include "main.h"

struct Plane {
        Vector3 position = {1,1,1};
        Vector2 size = {1,1};
        Color color = GREEN;
        
        void Draw() {
            DrawPlane(this->position, this->size, this->color);
        }
};

struct Sphere {
        Vector3 position = {1,1,1};
        //this is actually the diameter but raylib calls it a radius and I hate it.
        float radius = 1;
        Color color = RED;
        
        void Draw() {
            DrawSphere(this->position, this->radius, this->color);
        }
};

struct Cube {
        Vector3 position = {1,1,1};
        Vector3 size = {1,1,1};
        Color color = BLUE;
        
        void Draw() {
            DrawCube(this->position, this->size.x, this->size.y, this->size.z, this->color);
        }
};


void CheckCollisions(Camera*camera, Vector3& movement, const vector<Cube>& cubes, const vector<Sphere>& spheres, const vector<Plane>& planes) {
    for(const Cube& c : cubes) { 
        if(abs(c.position - camera->position) > 5) continue;
        BoundingBox temp = {(Vector3){c.position.x + c.size.x/2, c.position.y + c.size.y/2, c.position.z + c.size.z/2}, (Vector3){c.position.x - c.size.x/2, c.position.y - c.size.y/2, c.position.z - c.size.z/2}};
        if(CheckCollisionBoxSphere(temp, camera->position + movement, 1)) DrawText("YOU HIT SOMETHING", 500, 50, 30, BLACK);
    }
    for (const Sphere& s : spheres) {
        
    }
    for (const Plane& p : planes) {
        
    }
}
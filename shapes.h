#include "main.h"

struct Plane {
        Vector3 position;
        Vector2 size;
        Color color;
        
        void Draw() {
            DrawPlane(this->position, this->size, this->color);
        }
};

struct Sphere {
        Vector3 position;
        //this is actually the diameter but raylib calls it a radius and I hate it.
        float radius;
        Color color;
        
        void Draw() {
            DrawSphere(this->position, this->radius, this->color);
        }
};

struct Cube {
        Vector3 position;
        Vector3 size;
        Color color;
        
        void Draw() {
            DrawCube(this->position, this->size.x, this->size.y, this->size.z, this->color);
        }
};
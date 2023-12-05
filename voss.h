#pragma once
#include <stdbool.h>
#include "raylib.h"

void voss(bool enabled)
{
    float size = 32.0f;

    // Define Origin
    Vector3 origin = (Vector3){-size, 0.0f, 0.0f};

    // Draw a cylinder shape in the middle of the floor (origin + (half size, 0, half size)) with a radius of 1.0 and a height of 2.0
    DrawCylinder(origin, 1.0f, 2.0f, 10.0f, 7, RED);

    // Draw Ground
    DrawPlane(origin, (Vector2){size, size}, BLUE);

    // Draw Walls on the sides
    float wallHeight = 10.0f;
    float wallThickness = 2.0f;

    // North Wall
    Vector3 northWall_orig = (Vector3){-size, wallHeight / 2.0f, size - size / 2.0f};
    // South Wall
    Vector3 southWall_orig = (Vector3){-size, wallHeight / 2.0f, -(size / 2.0f)};
    // East Wall
    Vector3 eastWall_orig = (Vector3){-((size / 2.0f) * 3), wallHeight / 2.0f, 0.0f};

    // Draw Walls
    DrawCube(northWall_orig, size, wallHeight, wallThickness, GRAY);
    DrawCube(southWall_orig, size, wallHeight, wallThickness, GRAY);
    DrawCube(eastWall_orig, wallThickness, wallHeight, size, GREEN);
}

class Enemy
{
private:
    string name;
    Vector3 position;

    Model model;
    string modelPath;

    Texture2D texture;
    string texturePath;

    float scale;

    Vector3 speed;
    Vector3 acceleration;
    int health;
    int damage;

    bool showBoundry;
    bool solid;

public:
    Enemy() = delete; // Delete default constructor

    Enemy(
        string name,
        Vector3 position,
        string modelPath,
        string texturePath,
        Vector3 speed = Vector3{0.0f, 0.0f, 0.0f},
        Vector3 acceleration = Vector3{0.0f, 0.0f, 0.0f},
        int health = 100,
        int damage = 10,
        float scale = 1.0f,
        bool showBoundry = true,
        bool solid = true)
    {
        this->name = name;
        this->position = position;
        this->modelPath = modelPath;
        this->texturePath = texturePath;
        this->speed = speed;
        this->acceleration = acceleration;
        this->health = health;
        this->damage = damage;
        this->scale = scale;
        this->showBoundry = showBoundry;
        this->solid = solid;

        load();
    }

    ~Enemy()
    {
        UnloadModel(model);
        UnloadTexture(texture);
    }

    void load()
    {
        model = LoadModel(modelPath.c_str());
        texture = LoadTexture(texturePath.c_str());
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }

    void draw()
    {
        // Declare the CalculateBoundingBox function
        // BoundingBox CalculateBoundingBox(Mesh mesh);

        if (solid)
            DrawModel(model, position, scale, Color{255, 255, 255, 255});
        // if (showBoundry && model.meshCount > 0)
        // {
        //     BoundingBox box = CalculateBoundingBox(model.meshes[0]);
        //     DrawBoundingBox(box, Color{255, 255, 255, 255});
        // }

        // Recalculate position, and speed
        position.x += speed.x;
        position.y += speed.y;
        position.z += speed.z;

        const float maxSpeed = 0.1f;

        if (abs(speed.x) > maxSpeed)
            acceleration.x *= -1.0f;
        if (abs(speed.y) > maxSpeed)
            acceleration.y *= -1.0f;
        if (abs(speed.z) > maxSpeed)
            acceleration.z *= -1.0f;

        speed.x += acceleration.x;
        speed.y += acceleration.y;
        speed.z += acceleration.z;
    }

    // Getters
    std::string getName() { return name; }
    Vector3 getPosition() { return position; }
    Model getModel() { return model; }
    string getModelPath() { return modelPath; }
    Texture2D getTexture() { return texture; }
    string getTexturePath() { return texturePath; }
    float getScale() { return scale; }
    bool getShowBoundry() { return showBoundry; }
    bool getSolid() { return solid; }

    // Setters
    void setName(string name) { this->name = name; }
    void setPosition(Vector3 position) { this->position = position; }
    void setModel(Model model) { this->model = model; }
    void setModelPath(string modelPath) { this->modelPath = modelPath; }
    void setTexture(Texture2D texture) { this->texture = texture; }
    void setTexturePath(string texturePath) { this->texturePath = texturePath; }
    void setScale(float scale) { this->scale = scale; }
    void setShowBoundry(bool showBoundry) { this->showBoundry = showBoundry; }
    void setSolid(bool solid) { this->solid = solid; }

    // Methods
    void moveRelative(Vector3 position)
    {
        this->position.x += position.x;
        this->position.y += position.y;
        this->position.z += position.z;
    }

    void moveRelativeX(float x)
    {
        this->position.x += x;
    }

    void moveRelativeY(float y)
    {
        this->position.y += y;
    }

    void moveRelativeZ(float z)
    {
        this->position.z += z;
    }

    void moveAbsoluteX(float x)
    {
        this->position.x = x;
    }

    void moveAbsoluteY(float y)
    {
        this->position.y = y;
    }

    void moveAbsoluteZ(float z)
    {
        this->position.z = z;
    }
};
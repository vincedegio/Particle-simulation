#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include "constants.h"
#include "types.h"

class particleData
{
public:

    int id = 0;
    float Attraction = 4;
    float range = 100;
    float collisionRange = 10;
    float drag = 0.1;
    float velocityX = 0;
    float velocityY = 0;
    float densityValue = 0.1;
    Vector2 Direction = { 0, 0 };
    Vector2 CollisionAttractions = initializeVector2D;
    Vector2 position = initializeVector2D;
    float DrawRadius = 3;
    ParticleTypes type = ParticleTypes::PARTICLE3;

    particleData();
private:
    static int lastID;
};

#endif
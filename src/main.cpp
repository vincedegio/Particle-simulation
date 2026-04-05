// Build as Windows subsystem without console, keep main() entry:
#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "raylib.h"

#define initializeVector2D {0, 0}

class Time {
public:
    // Returns delta time in seconds as a float
    static float DeltaTime() {
        using clock = std::chrono::high_resolution_clock;
        static auto last = clock::now();   // persists across calls
        auto now = clock::now();
        std::chrono::duration<float> dt = now - last;
        last = now;
        return dt.count();
    }
};

static int lastID = 0;
const float maxSpeed = 10000;
Vector2 bounds = initializeVector2D;
const int numTypeOne = 000;
const int numTypeTwo = 3000;
const int numTypeThree = 3000;

class particleData
{
public:

    int id = 0;
    float Attraction = 4;
    const float range = 100;
    float collisionRange = 10;
    float drag = 0.1;
    float velocityX = 0;
    float velocityY = 0;
    float densityValue = 0.1;
    Vector2 Direction = { 0, 0 };
    Vector2 CollisionAttractions = initializeVector2D;
    Vector2 position = initializeVector2D;
    float DrawRadius = 3;
    int type = 2;

    particleData() {
        id = lastID;
        lastID++;
    }
};

//attraction randomized value = ((std::rand() % 750)) + 250
//range rondomized value = ((std::rand() % 750)) + 250
//collision range randomized value = (std::rand() % 15) + 5

particleData particles[numTypeOne + numTypeTwo + numTypeThree];

void update()
{
    float deltaTime = Time::DeltaTime();
    for (particleData& particle : particles)
    {
        particle.CollisionAttractions = initializeVector2D;

        int neighbourCount = 1;

        for (const particleData& particleDetected : particles)
        {
            if (particleDetected.id == particle.id) continue;

            float dist = std::sqrt((particleDetected.position.x - particle.position.x) * (particleDetected.position.x - particle.position.x) + (particleDetected.position.y - particle.position.y) * (particleDetected.position.y - particle.position.y));

            if (dist > particle.range) continue;

            if (dist == 0) dist = 0.0001;

            float inversDist = 1 / dist;

            neighbourCount++;

            if (dist > particle.collisionRange + particle.DrawRadius)
            {
                if (particle.type == 1 && particleDetected.type == 1)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist, (particleDetected.position.y - particle.position.y) / dist };
                }

                if (particle.type == 2 && particleDetected.type == 1)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist, (particleDetected.position.y - particle.position.y) / dist };
                }

                if (particle.type == 1 && particleDetected.type == 2)
                {
                    particle.Direction = { ((particleDetected.position.x - particle.position.x) / dist * -1), ((particleDetected.position.y - particle.position.y) / dist * -1) };
                }

                if (particle.type == 1 && particleDetected.type == 3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist, (particleDetected.position.y - particle.position.y) / dist };
                }

                if (particle.type == 2 && particleDetected.type == 3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist, (particleDetected.position.y - particle.position.y) / dist };
                }

                if (particle.type == 2 && particleDetected.type == 2)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist, (particleDetected.position.y - particle.position.y) / dist };
                }

                if (particle.type == 3 && particleDetected.type == 3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / dist * -1, (particleDetected.position.y - particle.position.y) / dist * -1 };
                }
            }
            else
            {
                particle.CollisionAttractions = { ((particleDetected.position.x - particle.position.x) / dist * -1), ((particleDetected.position.y - particle.position.y) / dist * -1) };

                particle.velocityX += particle.CollisionAttractions.x * (particleDetected.Attraction + (particle.densityValue * neighbourCount));
                particle.velocityY += particle.CollisionAttractions.y * (particleDetected.Attraction + (particle.densityValue * neighbourCount));
            }


            particle.velocityX += (particleDetected.Attraction - (particle.densityValue * neighbourCount)) * particle.Direction.x;
            particle.velocityY += (particleDetected.Attraction - (particle.densityValue * neighbourCount)) * particle.Direction.y;
        }

        particle.velocityX *= (1 - particle.drag);
        particle.velocityY *= (1 - particle.drag);

        if (particle.position.x > bounds.x)
        {
            particle.position.x = bounds.x;
            particle.velocityX -= 1000;
        }
        else if (particle.position.x < 5)
        {
            particle.position.x = 5;
            particle.velocityX += 1000;
        }

        if (particle.position.y > bounds.y)
        {
            particle.position.y = bounds.y;
            particle.velocityY -= 1000;
        }
        else if (particle.position.y < 5)
        {
            particle.position.y = 5;
            particle.velocityY += 1000;
        }

        particle.velocityX = std::clamp(particle.velocityX, -maxSpeed, maxSpeed);
        particle.velocityY = std::clamp(particle.velocityY, -maxSpeed, maxSpeed);

        particle.position.x += particle.velocityX * deltaTime / 3;
        particle.position.y += particle.velocityY * deltaTime / 3;

        if (particle.type == 1)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, GREEN);
        }
        else if (particle.type == 2)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, YELLOW);
        }
        else if (particle.type == 3)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, RED);
        }
    }
}


int main() {
    std::srand(std::time(nullptr)); // seed with current time

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "raylib fullscreen");

    for (particleData& particle : particles)
    {
        particle.position = { (float)(std::rand() % GetScreenWidth()), (float)(std::rand() % GetScreenHeight()) };

        if (particle.id == numTypeOne)
        {
            particle.type = 1;
            particle.drag = 0.1;
        }

        if (particle.id >= numTypeOne && particle.id < numTypeOne + numTypeTwo)
        {
            particle.type = 2;
        }
        if (particle.id >= numTypeOne + numTypeTwo && particle.id < numTypeOne + numTypeTwo + numTypeThree)
        {
            particle.type = 3;
            particle.DrawRadius -= 1;
        }
    }

    bounds = { (float)GetScreenWidth() - 10, (float)GetScreenHeight() - 10 };


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        if (IsKeyPressed(KEY_ESCAPE)) break;

        BeginDrawing();

        ClearBackground(BLACK);
        //DrawText("Hello, fullscreen raylib!", 40, 40, 32, RAYWHITE);
        //DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 80, RED);
        update();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
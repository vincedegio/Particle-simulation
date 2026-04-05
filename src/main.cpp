// Build as Windows subsystem without console, keep main() entry:
#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include <string>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "raylib.h"
#include "types.h"
#include "time_helpers.h"
#include "particle.h"

const float maxSpeed = 10000;
Vector2 bounds = initializeVector2D;
const int numTypeOne = 1000;
const int numTypeTwo = 1000;
const int numTypeThree = 1000;
const int numTypeFour = 250;

//attraction randomized value = ((std::rand() % 750)) + 250
//range rondomized value = ((std::rand() % 750)) + 250
//collision range randomized value = (std::rand() % 15) + 5

particleData particles[numTypeOne + numTypeTwo + numTypeThree + numTypeFour];

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

            dist = (dist == 0)
                    ? 0.0001
                    : dist;

            float inversDist = 1 / dist;

            neighbourCount++;

            if (dist > particle.collisionRange + particle.DrawRadius)
            {
                if (particle.type == ParticleTypes::PARTICLE1 && particleDetected.type == ParticleTypes::PARTICLE1)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist, (particleDetected.position.y - particle.position.y) * inversDist };
                }

                if (particle.type == ParticleTypes::PARTICLE2 && particleDetected.type == ParticleTypes::PARTICLE1)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist, (particleDetected.position.y - particle.position.y) * inversDist };
                }

                if (particle.type == ParticleTypes::PARTICLE1 && particleDetected.type == ParticleTypes::PARTICLE2)
                {
                    particle.Direction = { ((particleDetected.position.x - particle.position.x) * inversDist * -1), ((particleDetected.position.y - particle.position.y) * inversDist * -1) };
                }

                if (particle.type == ParticleTypes::PARTICLE1 && particleDetected.type == ParticleTypes::PARTICLE3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist, (particleDetected.position.y - particle.position.y) * inversDist };
                }

                if (particle.type == ParticleTypes::PARTICLE2 && particleDetected.type == ParticleTypes::PARTICLE3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist, (particleDetected.position.y - particle.position.y) * inversDist };
                }

                if (particle.type == ParticleTypes::PARTICLE2 && particleDetected.type == ParticleTypes::PARTICLE2)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist, (particleDetected.position.y - particle.position.y) * inversDist };
                }

                if (particle.type == ParticleTypes::PARTICLE3 && particleDetected.type == ParticleTypes::PARTICLE3)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) * inversDist * -1, (particleDetected.position.y - particle.position.y) * inversDist * -1 };
                }

                if(particle.type == ParticleTypes::PARTICLE4 && particleDetected.type != ParticleTypes::PARTICLE4)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / (dist * particle.DrawRadius), (particleDetected.position.y - particle.position.y) / (dist * particle.DrawRadius)};
                }

                if(particle.type == ParticleTypes::PARTICLE4 && particleDetected.type == ParticleTypes::PARTICLE4)
                {
                    particle.Direction = { (particleDetected.position.x - particle.position.x) / 10, (particleDetected.position.y - particle.position.y) / 10};
                }
            }
            else
            {
                particle.CollisionAttractions = { ((particleDetected.position.x - particle.position.x) * inversDist * -1), ((particleDetected.position.y - particle.position.y) * inversDist * -1) };

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

        if (particle.type == ParticleTypes::PARTICLE1)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, GREEN);
        }
        else if (particle.type == ParticleTypes::PARTICLE2)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, YELLOW);
        }
        else if (particle.type == ParticleTypes::PARTICLE3)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, RED);
        }
        else if (particle.type == ParticleTypes::PARTICLE4)
        {
            DrawCircle(particle.position.x, particle.position.y, particle.DrawRadius, WHITE);
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

        if (particle.id < numTypeOne)
        {
            particle.type = ParticleTypes::PARTICLE1;
            particle.drag = 0.1;
        }
        if (particle.id >= numTypeOne && particle.id < numTypeOne + numTypeTwo)
        {
            particle.type = ParticleTypes::PARTICLE2;
        }
        if (particle.id >= numTypeOne + numTypeTwo && particle.id < numTypeOne + numTypeTwo + numTypeThree)
        {
            particle.type = ParticleTypes::PARTICLE3;
            particle.DrawRadius -= 1;
        }
        if (particle.id >= numTypeOne + numTypeTwo + numTypeThree && particle.id < numTypeOne + numTypeTwo + numTypeThree + numTypeFour)
        {
            particle.type = ParticleTypes::PARTICLE4;
            particle.DrawRadius += std::rand() % 5 - 1;
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
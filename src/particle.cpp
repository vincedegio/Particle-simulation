#include "particle.h"

int particleData::lastID = 0;

particleData::particleData() 
{
    id = lastID;
    lastID++;
}
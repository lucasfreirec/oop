#include "noh.h"
#include <cmath>
#include <iomanip>
#define MY_PI 3.14159265358979323846

void Noh::heuristica(const Coord& destino)
{
    double x = abs(destino.lin - this->pos.lin), y = abs(destino.col - this->pos.col), hrot = 0;

    if(x != 0 && y != 0 && x != y) hrot = MY_PI/8;

    this->h = sqrt(2) * std::min(x, y) + abs(x - y) + hrot;
}

bool Noh::operator==(const Coord& C) const
{
    if(pos == C) return true;

    return false;
}

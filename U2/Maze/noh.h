#ifndef _NOH_H_
#define _NOH_H_

#include "coord.h"

class Noh
{
private:
    Coord pos;
    Coord ant;
    double g;
    double h;

public:
    Noh(): pos(Coord()), ant(Coord()), g(0.0), h(0.0) {}
    Noh(Coord p, Coord a, double cp): pos(p), ant(a), g(cp) {}

    void heuristica(const Coord& destino);

    Coord getPos() const {return pos;}

    Coord getAnt() const {return ant;}

    double getG() const {return g;}

    double getF() const {return g + h;}

    void setPos(const Coord& C) {pos = C;}

    void setAnt(const Coord& C) {ant = C;}

    void setG(const double& G) {g = G;}

    bool operator==(const Coord& C) const;

    bool operator<(const Noh& N) const { return getF() < N.getF(); }

    friend class Labirinto;
};

#endif // _NOH_H_

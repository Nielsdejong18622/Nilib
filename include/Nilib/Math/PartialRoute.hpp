#ifndef _PARTIAL_ROUTE_H
#define _PARTIAL_ROUTE_H

#include <vector>
#include "Simulation/State.h"

struct PartialRoute
{

    PartialRoute(std::vector<LocationID> const candidates, Coord const cost)
        : candidates(candidates), cost(cost) {
          };

    std::vector<LocationID> const candidates;
    Coord const cost;
};

#endif
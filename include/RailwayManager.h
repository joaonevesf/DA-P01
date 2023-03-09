#ifndef P1_RAILWAYMANAGER_H
#define P1_RAILWAYMANAGER_H

#include <unordered_set>
#include <string>

#include "RailwayNetwork.h"

class RailwayManager {
public:
    RailwayManager() = default;

    const std::shared_ptr<RailwayNetwork> &getRailwayNetwork() const;
    void setRailwayNetwork(const std::shared_ptr<RailwayNetwork> &railwayNetwork);
private:
    std::shared_ptr<RailwayNetwork> railway_network;
};


#endif

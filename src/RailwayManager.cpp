#include "RailwayManager.h"

const std::shared_ptr<RailwayNetwork> &RailwayManager::getRailwayNetwork() const {
    return railway_network;
}

void RailwayManager::setRailwayNetwork(const std::shared_ptr<RailwayNetwork> &railwayNetwork) {
    railway_network = railwayNetwork;
}

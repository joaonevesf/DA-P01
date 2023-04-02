#include "RailwayManager.h"

const std::shared_ptr<RailwayNetwork> &RailwayManager::getRailwayNetwork() const {
    return railway_network;
}

void RailwayManager::setRailwayNetwork(const std::shared_ptr<RailwayNetwork> &railwayNetwork) {
    railway_network = railwayNetwork;
}

void RailwayManager::run() {
    while (menu->execute());
}

const std::shared_ptr<Menu> &RailwayManager::getMenu() const {
    return menu;
}

void RailwayManager::setMenu(const std::shared_ptr<Menu> &menu) {
    RailwayManager::menu = menu;
}

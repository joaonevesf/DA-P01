#ifndef P1_RAILWAYMANAGER_H
#define P1_RAILWAYMANAGER_H

#include <unordered_set>
#include <string>

#include "Menu.h"
#include "RailwayNetwork.h"

class Menu;

class RailwayManager {
public:
    RailwayManager() = default;

    const std::shared_ptr<RailwayNetwork> &getRailwayNetwork() const;
    void setRailwayNetwork(const std::shared_ptr<RailwayNetwork> &railwayNetwork);

    const std::shared_ptr<Menu> &getMenu() const;

    void setMenu(const std::shared_ptr<Menu> &menu);

    void run();
private:
    std::shared_ptr<RailwayNetwork> railway_network;
    std::shared_ptr<Menu> menu;
};


#endif

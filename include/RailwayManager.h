#ifndef P1_RAILWAYMANAGER_H
#define P1_RAILWAYMANAGER_H

#include <unordered_set>
#include <string>

#include "Menu.h"
#include "RailwayNetwork.h"

class Menu;

/**
 * Esta classe permite usar as funcionalidades do railway network assim como gerir a interface gráfica com os diferentes menus
 */
class RailwayManager {
public:
    RailwayManager() = default;

    [[nodiscard]] const std::shared_ptr<RailwayNetwork> &getRailwayNetwork() const;

    void setRailwayNetwork(const std::shared_ptr<RailwayNetwork> &railwayNetwork);

    void setMenu(const std::shared_ptr<Menu> &menu);

    /**
     * Executa o menu atual
     */
    void run();

private:
    std::shared_ptr<RailwayNetwork> railway_network;

    std::shared_ptr<Menu> menu;
};


#endif

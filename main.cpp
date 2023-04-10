#include "FileManager.h"
#include "RailwayNetwork.h"
#include "RailwayManager.h"
#include "ReadFileMenu.h"

#include <memory>
#include <iostream>

int main() {

    std::shared_ptr<RailwayManager> rm = std::make_shared<RailwayManager>();
    std::shared_ptr<RailwayNetwork> rn = std::make_shared<RailwayNetwork>();

    std::shared_ptr<ReadFileMenu> fogo = std::make_shared<ReadFileMenu>(rm);
    std::shared_ptr<Menu> menu = std::dynamic_pointer_cast<Menu>(fogo);

    rm->setRailwayNetwork(rn);
    rm->setMenu(menu);

    rm->run();

    return 0;
}

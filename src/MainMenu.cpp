#include "MainMenu.h"

MainMenu::MainMenu(std::weak_ptr<RailwayManager> railwayManager) : Menu(railwayManager) {}

bool MainMenu::execute() {
    return false;
}

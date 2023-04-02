#include "ReadFileMenu.h"

#include <utility>
#include "Menu.h"
#include "FileManager.h"
#include "MainMenu.h"

ReadFileMenu::ReadFileMenu(std::weak_ptr<RailwayManager> railwayManager)
    : Menu(std::move(railwayManager))
{

}

bool ReadFileMenu::execute() {
    std::cout << "\nChoose an option:\n" <<
                 "1. Load and parse railway network\n" <<
                 "2. Exit\n";

    int option = Menu::readOption(2);

    std::shared_ptr rm = railwayManager_.lock();
    FileManager fm(rm->getRailwayNetwork());
    MainMenu menu(rm);
    if (option == 1) {
        rm->setMenu(std::make_shared<MainMenu>(railwayManager_));
        return true;
    }
    return false;
}
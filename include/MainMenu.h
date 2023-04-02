#ifndef P1_MAINMENU_H
#define P1_MAINMENU_H

#include "Menu.h"

class MainMenu : public Menu {
public:
    MainMenu(std::weak_ptr<RailwayManager> railwayManager);

    bool execute() override;
};

#endif
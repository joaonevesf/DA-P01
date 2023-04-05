#ifndef P1_MAINMENU_H
#define P1_MAINMENU_H

#include "ReadFileMenu.h"

class MainMenu : public ReadFileMenu {
public:
    MainMenu(std::weak_ptr<RailwayManager> railwayManager);

    bool execute() override;
};

#endif
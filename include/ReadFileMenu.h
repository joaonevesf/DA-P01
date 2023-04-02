#ifndef P1_READFILEMENU_H
#define P1_READFILEMENU_H

#include "Menu.h"

class ReadFileMenu : public Menu {
public:
    ReadFileMenu(std::weak_ptr<RailwayManager> railwayManager);
protected:
    bool execute() override;
};

#endif

#ifndef P1_BASICSERVICESMENU_H
#define P1_BASICSERVICESMENU_H

#include "Menu.h"

class BasicServicesMenu : public Menu {
public:
    BasicServicesMenu(std::weak_ptr<RailwayManager> railwayManager);

    bool execute() override;
};

#endif

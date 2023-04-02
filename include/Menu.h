#ifndef P1_MENU_H
#define P1_MENU_H

#include <string>
#include <memory>
#include "RailwayManager.h"

class RailwayManager;

class Menu {
public:
    Menu(std::weak_ptr<RailwayManager> railwayManager);

    virtual bool execute() = 0;
protected:
    std::weak_ptr<RailwayManager> railwayManager_;

    static bool getNumericalInput(int &selectedOption);
    static bool getStringInput(std::string &s);
    static int readOption(int numberOfOptions);
};

#endif

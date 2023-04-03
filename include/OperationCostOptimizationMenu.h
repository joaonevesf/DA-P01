#ifndef P1_OPERATIONCOSTOPTIMIZATIONMENU_H
#define P1_OPERATIONCOSTOPTIMIZATIONMENU_H

#include "Menu.h"

class OperationCostOptimizationMenu : public Menu {
public:
    OperationCostOptimizationMenu(std::weak_ptr<RailwayManager> railwayManager);
    bool execute() override;
};


#endif //P1_OPERATIONCOSTOPTIMIZATIONMENU_H

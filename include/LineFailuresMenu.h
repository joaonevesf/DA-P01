#ifndef P1_LINEFAILURESMENU_H
#define P1_LINEFAILURESMENU_H

#include "Menu.h"

class LineFailuresMenu: public Menu {
public:
    LineFailuresMenu(std::weak_ptr<RailwayManager> railwayManager);
    bool execute() override;
    bool deleteTrackStationMenu();
    void deleteTrack();
    void deleteStation();
    void listAffectedStations();
    bool undoActionsMenu();
};


#endif //P1_LINEFAILURESMENU_H

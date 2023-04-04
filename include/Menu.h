#ifndef P1_MENU_H
#define P1_MENU_H

#include <string>
#include <memory>
#include <vector>
#include <queue>

#include "RailwayManager.h"
#include "StationTrack.h"

class RailwayManager;
class Station;

class Menu {
public:
    Menu(std::weak_ptr<RailwayManager> railwayManager);

    virtual bool execute() = 0;
protected:
    std::weak_ptr<RailwayManager> railwayManager_;

    void printDashes(int length);

    static bool getNumericalInput(int &selectedOption);
    static bool getStringInput(std::string &s);
    static int readOption(int numberOfOptions);


    void printPaths(std::vector<std::deque<Track *>> paths);

    std::shared_ptr<Station> getStation(const std::string& name);
};

#endif

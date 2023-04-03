#ifndef P1_READFILEMENU_H
#define P1_READFILEMENU_H

#define FILES_FOLDER "../Project1Data/"

#include <fstream>

#include "Menu.h"
#include "FileManager.h"

class ReadFileMenu : public Menu {
public:
    ReadFileMenu(std::weak_ptr<RailwayManager> railwayManager);
protected:
    bool execute() override;

    bool showFileReadOptions(FileManager &fm);

    bool chooseFiles(FileManager &fm);
    bool chooseEdgesFile(FileManager &fm);
    bool chooseStationsFile(FileManager &fm);
};

#endif

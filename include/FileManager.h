#ifndef P1_FILEMANAGER_H
#define P1_FILEMANAGER_H

#include <string>
#include <memory>

#include "RailwayNetwork.h"

class FileManager {
public:
    FileManager(std::shared_ptr<RailwayNetwork> railwayNetwork);

    bool readStations(std::string filepath);

    bool readNetworkEdges(std::string filepath);

    bool readAll(std::string stations_file, std::string network_file);

private:
    std::shared_ptr<RailwayNetwork> railwayNetwork;
};

#endif

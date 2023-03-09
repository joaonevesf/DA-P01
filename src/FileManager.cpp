#include "FileManager.h"

#include <fstream>
#include <set>
#include <sstream>
#include <utility>

FileManager::FileManager(std::shared_ptr<RailwayNetwork> railwayNetwork)
    : railwayNetwork(std::move(railwayNetwork))
{

}

bool FileManager::readStations(std::string filepath) {
    std::set<std::string> stations_read;
    std::ifstream file_reader(filepath);
    if(!file_reader.is_open()) {
        return false;
    }

    std::string current_line;

    std::getline(file_reader, current_line);

    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;

    while(std::getline(file_reader, current_line)) {
        if (stations_read.find(current_line) == stations_read.end() || current_line.empty()) {
            continue;
        }

        stations_read.insert(current_line);
        std::istringstream iss(current_line);

        std::getline(iss, name, ',');
        std::getline(iss, district, ',');
        std::getline(iss, municipality, ',');
        std::getline(iss, township, ',');
        std::getline(iss, line, ',');

        std::shared_ptr<Station> station_to_add = std::make_shared<Station>(name, district, municipality, township,
                                                                            line);

        this->railwayNetwork->addStation(station_to_add);
    }

    return true;
}

bool FileManager::readNetworkEdges(std::string filepath) {
    std::ifstream file_reader(filepath);

    std::string current_line;
    std::getline(file_reader, current_line);

    std::string station_a;
    std::string station_b;
    std::string capacity_string;
    std::string service;

    while (std::getline(file_reader, current_line)) {

    }
}
#include "FileManager.h"

#include <fstream>
#include <set>
#include <sstream>
#include <utility>

#include <iostream>

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
        if (stations_read.find(current_line) != stations_read.end() || current_line.empty()) {
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

    if(!file_reader.is_open()) {
        return false;
    }

    std::set<std::string> tracks_read;

    std::string current_line;
    std::getline(file_reader, current_line);

    std::string station_a;
    std::string station_b;
    std::string capacity_string;
    std::string service;
    int cost;

    while (std::getline(file_reader, current_line)) {
        if (tracks_read.find(current_line) != tracks_read.end() || current_line.empty()) {
            continue;
        }

        tracks_read.insert(current_line);
        std::istringstream iss(current_line);

        std::getline(iss, station_a, ',');
        std::getline(iss, station_b, ',');
        std::getline(iss, capacity_string, ',');
        std::getline(iss, service, ',');

        cost = (service == "STANDARD") ? 2 : 4;

        std::shared_ptr<Station> station_left_mock = std::make_shared<Station>(station_a, "", "", "", "");
        std::shared_ptr<Station> station_right_mock = std::make_shared<Station>(station_b, "", "", "", "");

        std::shared_ptr<Station> station_left = *(this->railwayNetwork->getStationSet().find(station_left_mock));
        std::shared_ptr<Station> station_right = *(this->railwayNetwork->getStationSet().find(station_right_mock));

        this->railwayNetwork->addTrack(station_left, station_right, service, std::stod(capacity_string), cost);
    }

    return true;
}

bool FileManager::readAll(std::string stations_file, std::string network_file) {
    if(stations_file.empty() || network_file.empty()) {
        return false;
    }

    return readStations(stations_file) && readNetworkEdges(network_file);
}
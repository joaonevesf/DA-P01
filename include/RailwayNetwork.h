#ifndef RAILWAY_NETWORK
#define RAILWAY_NETWORK

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <memory>
#include <tuple>
#include <unordered_set>

#include "StationTrack.h"

struct StationHash {
    std::size_t operator()(std::shared_ptr<Station> const& station) const noexcept {
        return std::hash<std::string>()(station->getName());
    }
};

struct StationHashEquality {
    bool operator() (std::shared_ptr<Station> const& s1, std::shared_ptr<Station> const& s2) const noexcept {
        return s1->getName() == s2->getName();
    }
};

class RailwayNetwork {
public:
    ~RailwayNetwork();

    bool addStation(std::shared_ptr<Station> station);

    bool addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w);
    bool addBidirectionalTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w);

    int getNumVertex() const;
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> getStationSet() const;

    bool bfs(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    void edmondsKarp(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);

protected:
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall

    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif
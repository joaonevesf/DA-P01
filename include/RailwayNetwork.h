#ifndef RAILWAY_NETWORK
#define RAILWAY_NETWORK

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <memory>
#include <unordered_set>

#include "StationTrack.h"

struct StationHash {
    std::size_t operator()(std::shared_ptr<Station> const& station) const noexcept {
        return std::hash<std::string>{}(station->getName());
    }
};

struct StationHashEquality {
    bool operator() (std::shared_ptr<Station> const& s1, std::shared_ptr<Station> const& s2) const noexcept {
        return s1.get() == s2.get();
    }
};

class RailwayNetwork {
public:
    ~RailwayNetwork();

    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addStation(std::shared_ptr<Station> station);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge capacity (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, double w);
    bool addBidirectionalEdge(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, double w);

    int getNumVertex() const;
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> getStationSet() const;
protected:
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;    // station set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif
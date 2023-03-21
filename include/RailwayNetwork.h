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
#include <stack>

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
    bool addStation(std::shared_ptr<Station> station);

    bool addTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w);
    bool addBidirectionalTrack(std::shared_ptr<Station> station_src, std::shared_ptr<Station> station_dest, std::string service, double w);

    int getNumVertex() const;
    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> getStationSet() const;

    bool findAugmentingPath(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    double edmondsKarp(const std::shared_ptr<Station> &station_src, const std::shared_ptr<Station> &station_dest);
    static void testAndVisit(std::queue<std::shared_ptr<Station>> &queue, std::shared_ptr<Track> track,
                      const std::shared_ptr<Station>& station, double residual);
    static double findMinResidual(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest);
    static void updatePath(const std::shared_ptr<Station> &station_src, std::shared_ptr<Station> station_dest, double minRes);

    void deactivateTrack(const std::shared_ptr<Track> &track);
    void deactivateStation(const std::shared_ptr<Station> &station);
    void undoLastDeletion();
    void undoAllDeletions();

    std::shared_ptr<Station> mostAffectedStation();
protected:

    std::unordered_set<std::shared_ptr<Station>, StationHash, StationHashEquality> stationSet;
    std::stack<std::shared_ptr<Track>> inactiveTracks;
    std::stack<std::shared_ptr<Station>> inactiveStations;

    std::stack<char> deletionRecord;
};

#endif